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
433UTILS_DEPENDENCIES = wiringpi rcswitch
433UTILS_SUBDIR = RPi_utils

$(eval $(cmake-package))
