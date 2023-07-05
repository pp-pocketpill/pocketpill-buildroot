################################################################################
#
# retrofe
#
################################################################################

VBA_VERSION = pocketpill
VBA_SITE_METHOD = git
VBA_SITE = https://github.com/pp-pocketpill/visualboyadvance-m
VBA_DEPENDENCIES = sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx zlib

VBA_SUPPORTS_IN_SOURCE_BUILD = NO
VBA_CONF_OPTS += -DENABLE_SDL=YES -DENABLE_OPENGL=NO -DENABLE_LINK=NO -DENABLE_WX=NO

define VBA_INSTALL_TARGET_CMDS
	$(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/games
	$(INSTALL) -m 0755 $(@D)/visualboyadvance-m/build/vbam $(TARGET_DIR)/usr/games/vbam
endef

TARGET_CFLAGS += -O3 -lgomp

$(eval $(cmake-package))