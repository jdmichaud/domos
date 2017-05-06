###############################################################################
#
# receiver
#
###############################################################################

RECEIVER_VERSION = master
RECEIVER_SITE = git://github.com/jdmichaud/domos.git
RECEIVER_SITE_METHOD = git
RECEIVER_LICENSE = Apache v2.0
RECEIVER_LICENSE_FILES = LICENSE
RECEIVER_DEPENDENCIES = libhttp rcswitch librfprotocol
RECEIVER_SUBDIR = rpi/receiver/buildroot

$(eval $(cmake-package))
