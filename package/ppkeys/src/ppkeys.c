#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <time.h>

#define POWERDOWN_PRESS_TIME 3

#define KEYCODE_START 28
#define KEYCODE_SELECT 57
#define KEYCODE_A 30
#define KEYCODE_B 48
#define KEYCODE_X 45
#define KEYCODE_Y 21
#define KEYCODE_LEFT 105
#define KEYCODE_RIGHT 106
#define KEYCODE_UP 103
#define KEYCODE_DOWN 108

struct key_fields {
    unsigned int _: 6;
    unsigned int start: 1;
    unsigned int select: 1;
    unsigned int A: 1;
    unsigned int B: 1;
    unsigned int X: 1;
    unsigned int Y: 1;
    unsigned int left: 1;
    unsigned int right: 1;
    unsigned int up: 1;
    unsigned int down: 1;
};

typedef union pressed_keys {
    uint16_t raw;
    struct key_fields pressed;
} pressed_keys_t;

#define keyDown(keys, prevKeys) ((keys) && !(prevKeys))
#define keyUp(keys, prevKeys) (!(keys) && (prevKeys))
#define keyHold(keys, prevKeys) ((keys) && (prevKeys))

int main(int argc, char **argv) {
	struct input_event ev;
    
    fd_set rfds;
    struct timeval tv;
	int fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open input device");
        return 1;
    }

    fcntl(fd, F_SETFL, O_NONBLOCK);

    pressed_keys_t prevKeys;
    pressed_keys_t keys;
    prevKeys.raw = 0;
    keys.raw = 0;

    time_t now;

    time_t startSelectDown;

	while(1) {
        prevKeys = keys;

        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int retval = select(fd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1) {
            perror("select()");
            break;
        }
        else if (retval) {
            if (FD_ISSET(fd, &rfds)) {
		        ssize_t bytes = read(fd, &ev, sizeof(ev));
                if (bytes > 0 && ev.type == EV_KEY) {
                    printf("KEY EVENT: %d, VALUE: %d\n", ev.code, ev.value);

                    if (ev.code == KEYCODE_START) keys.pressed.start = ev.value;
                    if (ev.code == KEYCODE_SELECT) keys.pressed.select = ev.value;
                    if (ev.code == KEYCODE_A) keys.pressed.A = ev.value;
                    if (ev.code == KEYCODE_B) keys.pressed.B = ev.value;
                    if (ev.code == KEYCODE_X) keys.pressed.X = ev.value;
                    if (ev.code == KEYCODE_Y) keys.pressed.Y = ev.value;
                    if (ev.code == KEYCODE_LEFT) keys.pressed.left = ev.value;
                    if (ev.code == KEYCODE_RIGHT) keys.pressed.right = ev.value;
                    if (ev.code == KEYCODE_UP) keys.pressed.up = ev.value;
                }
            }
        }
        
        now = time(NULL);

        if (keyDown(keys.pressed.start && keys.pressed.select, prevKeys.pressed.start && prevKeys.pressed.select)) {
            printf("Start + Select down @ %ld\n", now);
            startSelectDown = now;
        }
        if (keyHold(keys.pressed.start && keys.pressed.select, prevKeys.pressed.start && prevKeys.pressed.select)) {
            printf("Start + Select hold @ %ld\n", now);
            if (now - startSelectDown >= POWERDOWN_PRESS_TIME) {
                system("/bin/gpdown");
            }
        }
        if (keyUp(keys.pressed.start && keys.pressed.select, prevKeys.pressed.start && prevKeys.pressed.select)) {
            printf("Start + Select up @ %ld\n", now);
        }

        usleep(10000);

	}
	close(fd);
}