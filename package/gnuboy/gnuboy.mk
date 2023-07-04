################################################################################
#
# gnuboy
#
################################################################################

GNUBOY_VERSION = pocketpill
GNUBOY_SITE_METHOD = git
GNUBOY_SITE = https://github.com/pp-pocketpill/gnuboy
GNUBOY_LICENSE = GPL-2.0
GNUBOY_LICENSE_FILES = COPYING

GNUBOY_DEPENDENCIES = sdl sdl_image sdl_mixer sdl_ttf zlib

GNUBOY_CFLAGS = $(TARGET_CFLAGS) $(subst $\",,$(BR2_TARGET_OPTIMIZATION)) -ffast-math -funsafe-math-optimizations

GNUBOY_CFLAGS += -ggdb -O3

GNUBOY_CONF_OPTS += CFLAGS="$(GNUBOY_CFLAGS)"
GNUBOY_CONF_OPTS += --prefix=$(TARGET_DIR)/usr/local --bindir=$(TARGET_DIR)/usr/games
GNUBOY_CONF_OPTS += --without-fb \
		    --without-svgalib \
		    --without-x \
		    --without-sdl2 \
			--with-sdl

GNUBOY_CONF_ENV += SDL_CONFIG="$(STAGING_DIR)/usr/bin/sdl-config"

$(eval $(autotools-package))