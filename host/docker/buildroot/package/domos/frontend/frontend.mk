################################################################################
#
# frontend
#
################################################################################

FRONTEND_VERSION = master
FRONTEND_SITE = http://github.com/jdmichaud/domos.git
FRONTEND_SITE_METHOD = git
FRONTEND_LICENSE = Apache v2.0
FRONTEND_LICENSE_FILES = LICENSE
HOST_FRONTEND_DEPENDENCIES = host-python host-nodejs
FRONTEND_SUBDIR = rpi/frontend

$(eval $(generic-package))

define SET_OVERSCAN_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/dist/* $(TARGET_DIR)/var/www
endef
