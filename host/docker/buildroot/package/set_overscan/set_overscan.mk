###############################################################################
#
# set_overscan
#
###############################################################################

SET_OVERSCAN_VERSION = e3adae8adffa2c3b0bcccccd95fe818458bef3f5
SET_OVERSCAN_SITE = http://github.com/ukscone/set_overscan
SET_OVERSCAN_SITE_METHOD = git
SET_OVERSCANLICENSE = GPL v2.0
SET_OVERSCAN_INSTALL_STAGING = NO
SET_OVERSCAN_INSTALL_TARGET = YES

$(eval $(generic-package))

define SET_OVERSCAN_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define SET_OVERSCAN_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/overscan $(TARGET_DIR)/usr/sbin/
endef
