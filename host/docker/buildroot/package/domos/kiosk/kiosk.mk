###############################################################################
#
# kiosk
#
###############################################################################

KIOSK_VERSION = master
KIOSK_SITE = http://github.com/jdmichaud/kiosk
KIOSK_SITE_METHOD = git
KIOSK_LICENSE = MIT
KIOSK_DEPENDENCIES = qt5base qt5declarative qt5webengine qt5webview

KIOSK_CONF_OPTS = -DCMAKE_BUILD_TYPE=debug

$(eval $(cmake-package))
