###############################################################################
#
# 433Utils
#
###############################################################################

433UTILS_VERSION = e29912dc0f14f70f33b7cf8ad531d82bed8ee49c
433UTILS_SITE = git://github.com/ninjablocks/433Utils.git
433UTILS_SITE_METHOD = git
433UTILS_LICENSE = MIT
433UTILS_LICENSE_FILE = LICENSE
433UTILS_DEPENDENCIES = wiringpi
433UTILS_SUBDIR = RPi_utils

define 433UTILS_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/RPi_utils/RFSniffer $(TARGET_DIR)/usr/lib
	$(INSTALL) -D -m 0755 $(@D)/RPi_utils/send $(TARGET_DIR)/usr/lib
	$(INSTALL) -D -m 0755 $(@D)/RPi_utils/codesend $(TARGET_DIR)/usr/lib
endef

$(eval $(generic-package))
