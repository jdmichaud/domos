###############################################################################
#
# RF24
#
###############################################################################

RF24_VERSION = RF24v1.2.0
RF24_SITE = http://github.com/nRF24/RF24.git
RF24_SITE_METHOD = git
RF24_LICENSE = GPL v2.0
RF24_INSTALL_STAGING = YES
RF24_INSTALL_TARGET = NO

$(eval $(cmake-package))
