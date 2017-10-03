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

define FRONTEND_BUILD_CMDS
    $(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)/rpi/frontend all
endef

define FRONTEND_INSTALL_TARGET_CMDS
	cp -r $(@D)/rpi/frontend/dist/* $(TARGET_DIR)/var/www
endef

$(eval $(generic-package))
