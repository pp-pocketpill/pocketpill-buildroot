################################################################################
#
# ppgfxtest package
#
################################################################################

PPGFXTEST_VERSION = 1.0
PPGFXTEST_SITE = $(PPGFXTEST_PKGDIR)src
PPGFXTEST_DEPENDENCIES = sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx libglib2
PPGFXTEST_SITE_METHOD = local

define PPGFXTEST_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define PPGFXTEST_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/ppgfxtest  $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))