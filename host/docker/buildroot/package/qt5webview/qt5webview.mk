################################################################################
#
# qt5webVIEW
#
################################################################################

QT5WEBVIEW_VERSION = $(QT5_VERSION)
QT5WEBVIEW_SITE = $(QT5_SITE)
QT5WEBVIEW_SOURCE = qtwebview-opensource-src-$(QT5WEBVIEW_VERSION).tar.xz
QT5WEBVIEW_DEPENDENCIES = qt5webengine
QT5WEBVIEW_INSTALL_STAGING = YES

ifeq ($(BR2_PACKAGE_QT5BASE_LICENSE_APPROVED),y)
QT5WEBVIEW_LICENSE = LGPLv2.1 with exception or LGPLv3 or GPLv2
# Source files contain references to LGPL_EXCEPTION.txt but it is not included
# in the archive.
QT5WEBVIEW_LICENSE_FILES = LICENSE.GPLv3 LICENSE.LGPLv3 LICENSE.GPLv2
else
QT5WEBVIEW_LICENSE = Commercial license
QT5WEBVIEW_REDISTRIBUTE = NO
endif

ifdef QT5WEBVIEW_QMAKE_CFLAGS
QT5WEBVIEW_QMAKEFLAGS += 'QMAKE_CFLAGS+=$(QT5WEBVIEW_QMAKE_CFLAGS)'
endif

QT5WEBVIEW_MAKE_ENV = $(TARGET_MAKE_ENV) PATH="$(@D):$$PATH"

# $(@D) needs to be in the PATH because of the python -> python2 symbolic link
# below.
define QT5WEBVIEW_CONFIGURE_CMDS
	ln -s $(HOST_DIR)/usr/bin/python2 $(@D)/python
	(cd $(@D); $(QT5WEBVIEW_MAKE_ENV) $(HOST_DIR)/usr/bin/qmake $(QT5WEBVIEW_QMAKEFLAGS))
endef

define QT5WEBVIEW_BUILD_CMDS
	$(QT5WEBVIEW_MAKE_ENV) $(MAKE) -C $(@D)
endef

define QT5WEBVIEW_INSTALL_STAGING_CMDS
	$(QT5WEBVIEW_MAKE_ENV) $(MAKE) -C $(@D) install
	$(QT5_LA_PRL_FILES_FIXUP)
endef

ifeq ($(BR2_PACKAGE_QT5DECLARATIVE_QUICK),y)
define QT5WEBVIEW_INSTALL_TARGET_QMLS
	cp -dpfr $(STAGING_DIR)/usr/qml/QtWebView $(TARGET_DIR)/usr/qml/
endef
endif

ifeq ($(BR2_PACKAGE_QT5BASE_EXAMPLES),y)
define QT5WEBVIEW_INSTALL_TARGET_EXAMPLES
	cp -dpfr $(STAGING_DIR)/usr/lib/qt/examples/webview* $(TARGET_DIR)/usr/lib/qt/examples/
endef
endif

ifneq ($(BR2_STATIC_LIBS),y)
define QT5WEBVIEW_INSTALL_TARGET_LIBS
	cp -dpf $(STAGING_DIR)/usr/lib/libQt5WebView*.so.* $(TARGET_DIR)/usr/lib
endef
endif

define QT5WEBVIEW_INSTALL_TARGET_CMDS
	$(QT5WEBVIEW_INSTALL_TARGET_LIBS)
	$(QT5WEBVIEW_INSTALL_TARGET_QMLS)
	$(QT5WEBVIEW_INSTALL_TARGET_EXAMPLES)
endef

$(eval $(generic-package))
