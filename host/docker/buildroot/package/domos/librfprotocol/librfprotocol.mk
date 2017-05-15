################################################################################
#
# librfprotocol
#
################################################################################

LIBRFPROTOCOL_VERSION = master
LIBRFPROTOCOL_SITE = http://github.com/jdmichaud/domos.git
LIBRFPROTOCOL_SITE_METHOD = git
LIBRFPROTOCOL_LICENSE = Apache v2.0
LIBRFPROTOCOL_INSTALL_STAGING = YES
LIBRFPROTOCOL_INSTALL_TARGET = NO
LIBRFPROTOCOL_SUBDIR = lib/rfprotocol/src

$(eval $(cmake-package))
