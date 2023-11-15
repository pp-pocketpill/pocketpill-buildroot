################################################################################
#
# gpsp
#
################################################################################

GPSP_VERSION = pocketpill
GPSP_SITE_METHOD = git
GPSP_SITE = https://github.com/pp-pocketpill/gpsp
GPSP_LICENSE = GPL-2.0
GPSP_LICENSE_FILES = COPYING.DOC

GPSP_DEPENDENCIES = sdl sdl_image sdl_mixer sdl_ttf zlib

GPSP_CFLAGS = $(TARGET_CFLAGS) $(subst $\",,$(BR2_TARGET_OPTIMIZATION)) -ffast-math -funsafe-math-optimizations 

GPSP_SDL_CFLAGS += $(shell $(STAGING_DIR)/usr/bin/sdl-config --cflags)
GPSP_SDL_LIBS += $(shell $(STAGING_DIR)/usr/bin/sdl-config --libs)

GPSP_CFLAGS += -ggdb -O3
GPSP_CFLAGS += -DARM_ARCH -DCHIP_BUILD
GPSP_CFLAGS += $(GPSP_SDL_CFLAGS)

GPSP_LIBS += $(GPSP_SDL_LIBS)
GPSP_LIBS += -lSDL_ttf -lSDL_image -ldl -lpthread -lz

define GPSP_BUILD_CMDS
	(cd $(@D)/chip; \
	make \
	CROSS_COMPILE=$(TARGET_CROSS) \
	CFLAGS='$(GPSP_CFLAGS)' \
	LIBS='$(GPSP_LIBS)' \
	)
endef

define GPSP_INSTALL_TARGET_CMDS
	$(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/games
	$(INSTALL) -m 0755 $(@D)/chip/gpsp $(TARGET_DIR)/usr/games/gpsp
	$(INSTALL) -m 0644 $(@D)/game_config.txt $(TARGET_DIR)/usr/games/game_config.txt
endef

$(eval $(generic-package))