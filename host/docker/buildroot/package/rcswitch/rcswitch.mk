################################################################################
#
# rswitch
#
################################################################################

RCSWITCH_VERSION = 2.6.2
RCSWITCH_SITE = https://github.com/sui77/rc-switch
RCSWITCH_SITE_METHOD = git
RCSWITCH_LICENSE = LGPL v2.1
RCSWITCH_LICENSE_FILES = CSwitch.h
RCSWITCH_DEPENDENCIES = wiringpi
RCSWITCH_INSTALL_STAGING = YES
RCSWITCH_INSTALL_TARGET = NO

$(eval $(cmake-package))
