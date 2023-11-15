################################################################################
#
# retrofe
#
################################################################################

RETROFE_VERSION = pocketpill-compat
RETROFE_SITE_METHOD = git
RETROFE_SITE = https://github.com/pp-pocketpill/RetroFE
RETROFE_DEPENDENCIES = gstreamer1 gst1-plugins-base sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx libglib2 sqlite zlib
RETROFE_LICENSE = GPL-3.0
RETROFE_LICENSE_FILES = LICENSE.txt

RETROFE_SUBDIR = RetroFE/Source
RETROFE_SUPPORTS_IN_SOURCE_BUILD = NO
RETROFE_CONF_OPTS += -DVERSION_MAJOR=0 -DVERSION_MINOR=0 -DVERSION_BUILD=0

ifeq ($(BR2_PACKAGE_LIBMIKMOD),y)
RETROFE_DEPENDENCIES += libmikmod
RETROFE_CONF_OPTS += -DLIBMIKMOD=1
endif

define RETROFE_INSTALL_TARGET_CMDS
	$(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/games
	$(INSTALL) -m 0755 $(@D)/RetroFE/Build/retrofe $(TARGET_DIR)/usr/games/retrofe
endef

TARGET_CFLAGS += -O3

$(eval $(cmake-package))