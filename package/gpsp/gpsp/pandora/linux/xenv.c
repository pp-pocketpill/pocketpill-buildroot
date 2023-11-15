/*
 * (C) Gražvydas "notaz" Ignotas, 2009-2011
 *
 * This work is licensed under the terms of any of these licenses
 * (at your option):
 *  - GNU GPL, version 2 or later.
 *  - GNU LGPL, version 2.1 or later.
 * See the COPYING file in the top-level directory.
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <dlfcn.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <linux/kd.h>

#define PFX "xenv: "

#define FPTR(f) typeof(f) * p##f
#define FPTR_LINK(xf, dl, f) { \
	xf.p##f = dlsym(dl, #f); \
	if (xf.p##f == NULL) { \
		fprintf(stderr, "missing symbol: %s\n", #f); \
		goto fail; \
	} \
}

struct xstuff {
	Display *display;
	FPTR(XCreateBitmapFromData);
	FPTR(XCreatePixmapCursor);
	FPTR(XFreePixmap);
	FPTR(XOpenDisplay);
	FPTR(XDisplayName);
	FPTR(XCloseDisplay);
	FPTR(XCreateSimpleWindow);
	FPTR(XChangeWindowAttributes);
	FPTR(XSelectInput);
	FPTR(XMapWindow);
	FPTR(XNextEvent);
	FPTR(XCheckTypedEvent);
	FPTR(XUnmapWindow);
	FPTR(XGrabKeyboard);
	FPTR(XPending);
	FPTR(XLookupKeysym);
	FPTR(XkbSetDetectableAutoRepeat);
};

static struct xstuff g_xstuff;

static Cursor transparent_cursor(struct xstuff *xf, Display *display, Window win)
{
	Cursor cursor;
	Pixmap pix;
	XColor dummy;
	char d = 0;

	memset(&dummy, 0, sizeof(dummy));
	pix = xf->pXCreateBitmapFromData(display, win, &d, 1, 1);
	cursor = xf->pXCreatePixmapCursor(display, pix, pix,
			&dummy, &dummy, 0, 0);
	xf->pXFreePixmap(display, pix);
	return cursor;
}

static int x11h_init(void)
{
	unsigned int display_width, display_height;
	Display *display;
	XSetWindowAttributes attributes;
	Window win;
	Visual *visual;
	void *x11lib;
	int screen;

	memset(&g_xstuff, 0, sizeof(g_xstuff));
	x11lib = dlopen("libX11.so.6", RTLD_LAZY);
	if (x11lib == NULL) {
		fprintf(stderr, "libX11.so load failed:\n%s\n", dlerror());
		goto fail;
	}
	FPTR_LINK(g_xstuff, x11lib, XCreateBitmapFromData);
	FPTR_LINK(g_xstuff, x11lib, XCreatePixmapCursor);
	FPTR_LINK(g_xstuff, x11lib, XFreePixmap);
	FPTR_LINK(g_xstuff, x11lib, XOpenDisplay);
	FPTR_LINK(g_xstuff, x11lib, XDisplayName);
	FPTR_LINK(g_xstuff, x11lib, XCloseDisplay);
	FPTR_LINK(g_xstuff, x11lib, XCreateSimpleWindow);
	FPTR_LINK(g_xstuff, x11lib, XChangeWindowAttributes);
	FPTR_LINK(g_xstuff, x11lib, XSelectInput);
	FPTR_LINK(g_xstuff, x11lib, XMapWindow);
	FPTR_LINK(g_xstuff, x11lib, XNextEvent);
	FPTR_LINK(g_xstuff, x11lib, XCheckTypedEvent);
	FPTR_LINK(g_xstuff, x11lib, XUnmapWindow);
	FPTR_LINK(g_xstuff, x11lib, XGrabKeyboard);
	FPTR_LINK(g_xstuff, x11lib, XPending);
	FPTR_LINK(g_xstuff, x11lib, XLookupKeysym);
	FPTR_LINK(g_xstuff, x11lib, XkbSetDetectableAutoRepeat);

	//XInitThreads();

	g_xstuff.display = display = g_xstuff.pXOpenDisplay(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "cannot connect to X server %s, X handling disabled.\n",
				g_xstuff.pXDisplayName(NULL));
		goto fail2;
	}

	visual = DefaultVisual(display, 0);
	if (visual->class != TrueColor)
		fprintf(stderr, PFX "warning: non true color visual\n");

	printf(PFX "X vendor: %s, rel: %d, display: %s, protocol ver: %d.%d\n", ServerVendor(display),
		VendorRelease(display), DisplayString(display), ProtocolVersion(display),
		ProtocolRevision(display));

	screen = DefaultScreen(display);

	display_width = DisplayWidth(display, screen);
	display_height = DisplayHeight(display, screen);
	printf(PFX "display is %dx%d\n", display_width, display_height);

	win = g_xstuff.pXCreateSimpleWindow(display,
			RootWindow(display, screen),
			0, 0, display_width, display_height, 0,
			BlackPixel(display, screen),
			BlackPixel(display, screen));

	attributes.override_redirect = True;
	attributes.cursor = transparent_cursor(&g_xstuff, display, win);
	g_xstuff.pXChangeWindowAttributes(display, win, CWOverrideRedirect | CWCursor, &attributes);

	g_xstuff.pXSelectInput(display, win, ExposureMask | FocusChangeMask | KeyPressMask | KeyReleaseMask);
	g_xstuff.pXMapWindow(display, win);
	g_xstuff.pXGrabKeyboard(display, win, False, GrabModeAsync, GrabModeAsync, CurrentTime);
	g_xstuff.pXkbSetDetectableAutoRepeat(display, 1, NULL);
	// XSetIOErrorHandler

	return 0;
fail2:
	dlclose(x11lib);
fail:
	g_xstuff.display = NULL;
	fprintf(stderr, "x11 handling disabled.\n");
	return -1;
}

static int x11h_update(int *is_down)
{
	XEvent evt;

	while (g_xstuff.pXPending(g_xstuff.display))
	{
		g_xstuff.pXNextEvent(g_xstuff.display, &evt);
		switch (evt.type)
		{
			case Expose:
				while (g_xstuff.pXCheckTypedEvent(g_xstuff.display, Expose, &evt))
					;
				break;

			case KeyPress:
				*is_down = 1;
				return g_xstuff.pXLookupKeysym(&evt.xkey, 0);

			case KeyRelease:
				*is_down = 0;
				return g_xstuff.pXLookupKeysym(&evt.xkey, 0);
				// printf("press %d\n", evt.xkey.keycode);
		}
	}

	return NoSymbol;
}

static struct termios g_kbd_termios_saved;
static int g_kbdfd;

static int tty_init(void)
{
	struct termios kbd_termios;
	int mode;

	g_kbdfd = open("/dev/tty", O_RDWR);
	if (g_kbdfd == -1) {
		perror(PFX "open /dev/tty");
		return -1;
	}

	if (ioctl(g_kbdfd, KDGETMODE, &mode) == -1) {
		perror(PFX "(not hiding FB): KDGETMODE");
		goto fail;
	}

	if (tcgetattr(g_kbdfd, &kbd_termios) == -1) {
		perror(PFX "tcgetattr");
		goto fail;
	}

	g_kbd_termios_saved = kbd_termios;
	kbd_termios.c_lflag &= ~(ICANON | ECHO); // | ISIG);
	kbd_termios.c_iflag &= ~(ISTRIP | IGNCR | ICRNL | INLCR | IXOFF | IXON);
	kbd_termios.c_cc[VMIN] = 0;
	kbd_termios.c_cc[VTIME] = 0;

	if (tcsetattr(g_kbdfd, TCSAFLUSH, &kbd_termios) == -1) {
		perror(PFX "tcsetattr");
		goto fail;
	}

	if (ioctl(g_kbdfd, KDSETMODE, KD_GRAPHICS) == -1) {
		perror(PFX "KDSETMODE KD_GRAPHICS");
		tcsetattr(g_kbdfd, TCSAFLUSH, &g_kbd_termios_saved);
		goto fail;
	}

	return 0;

fail:
	close(g_kbdfd);
	g_kbdfd = -1;
	return -1;
}

static void tty_end(void)
{
	if (g_kbdfd <= 0)
		return;

	if (ioctl(g_kbdfd, KDSETMODE, KD_TEXT) == -1)
		perror(PFX "KDSETMODE KD_TEXT");

	if (tcsetattr(g_kbdfd, TCSAFLUSH, &g_kbd_termios_saved) == -1)
		perror(PFX "tcsetattr");

	close(g_kbdfd);
	g_kbdfd = -1;
}

int xenv_init(void)
{
	int ret;

	ret = x11h_init();
	if (ret == 0)
		return 0;

	ret = tty_init();
	if (ret == 0)
		return 0;

	fprintf(stderr, PFX "error: both x11h_init and tty_init failed\n");
	return -1;
}

int xenv_update(int *is_down)
{
	if (g_xstuff.display)
		return x11h_update(is_down);

	// TODO: read tty?
	return -1;
}

void xenv_finish(void)
{
	// TODO: cleanup X?
	tty_end();
}
