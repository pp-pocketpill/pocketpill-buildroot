################################################################################
#
# ppkeys package
#
################################################################################

PPKEYS_VERSION = 1.0
PPKEYS_SITE = $(PPKEYS_PKGDIR)src
PPKEYS_SITE_METHOD = local

define PPKEYS_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define PPKEYS_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/ppkeys  $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))