###############################################################################
#
# kiosk
#
###############################################################################

KIOSK_VERSION = master
KIOSK_SITE = git://github.com/jdmichaud/kiosk
KIOSK_SITE_METHOD = git
KIOSK_LICENSE = MIT
KIOSK_DEPENDENCIES = qt5base qt5declarative qt5webengine

$(eval $(cmake-package))
