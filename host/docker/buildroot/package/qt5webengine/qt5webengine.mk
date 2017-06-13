################################################################################
#
# qt5webengine
#
################################################################################

QT5WEBENGINE_VERSION = $(QT5_VERSION)
QT5WEBENGINE_SITE = $(QT5_SITE)
QT5WEBENGINE_SOURCE = qtwebengine-opensource-src-$(QT5WEBENGINE_VERSION).tar.xz
QT5WEBENGINE_DEPENDENCIES = qt5base qt5declarative qt5webchannel \
	libcap libglib2 opus libvpx webp ffmpeg openssl host-gperf host-python
ifeq ($(BR2_PACKAGE_QT5BASE_XCB),y)
QT5WEBENGINE_DEPENDENCIES += xlib_libXScrnSaver xlib_libXcomposite \
	xlib_libXcursor xlib_libXi xlib_libXrandr xlib_libXtst
endif
QT5WEBENGINE_INSTALL_STAGING = YES

ifeq ($(BR2_PACKAGE_QT5BASE_LICENSE_APPROVED),y)
QT5WEBENGINE_LICENSE = LGPLv2.1 with exception or LGPLv3 or GPLv2
# Source files contain references to LGPL_EXCEPTION.txt but it is not included
# in the archive.
QT5WEBENGINE_LICENSE_FILES = LICENSE.GPLv3 LICENSE.LGPLv3 LICENSE.GPLv2
else
QT5WEBENGINE_LICENSE = Commercial license
QT5WEBENGINE_REDISTRIBUTE = NO
endif

# Use system ffmpeg because the embedded FFmpeg requires to create a
# configuration scripts which is difficult to use due to architecture-dependent
# arguments and because we prefer to use system-provided versions of the
# comments.
QT5WEBENGINE_QMAKEFLAGS = WEBENGINE_CONFIG=use_system_ffmpeg

ifdef QT5WEBENGINE_QMAKE_CFLAGS
QT5WEBENGINE_QMAKEFLAGS += 'QMAKE_CFLAGS+=$(QT5WEBENGINE_QMAKE_CFLAGS)'
endif

## Debug
QT5WEBENGINE_QMAKEFLAGS += CONFIG+=debug

QT5WEBENGINE_MAKE_ENV = $(TARGET_MAKE_ENV) PATH="$(@D):$$PATH"

# $(@D) needs to be in the PATH because of the python -> python2 symbolic link
# below.
define QT5WEBENGINE_CONFIGURE_CMDS
	ln -sf $(HOST_DIR)/usr/bin/python2 $(@D)/python
	(cd $(@D); $(QT5WEBENGINE_MAKE_ENV) $(HOST_DIR)/usr/bin/qmake $(QT5WEBENGINE_QMAKEFLAGS))
endef

define QT5WEBENGINE_BUILD_CMDS
	$(QT5WEBENGINE_MAKE_ENV) $(MAKE) -C $(@D)
endef

define QT5WEBENGINE_INSTALL_STAGING_CMDS
	$(QT5WEBENGINE_MAKE_ENV) $(MAKE) -C $(@D) install
	$(QT5_LA_PRL_FILES_FIXUP)
endef

ifeq ($(BR2_PACKAGE_QT5DECLARATIVE_QUICK),y)
define QT5WEBENGINE_INSTALL_TARGET_QMLS
	cp -dpfr $(STAGING_DIR)/usr/qml/QtWebEngine $(TARGET_DIR)/usr/qml/
endef
endif

ifeq ($(BR2_PACKAGE_QT5BASE_EXAMPLES),y)
define QT5WEBENGINE_INSTALL_TARGET_EXAMPLES
	cp -dpfr $(STAGING_DIR)/usr/lib/qt/examples/webengine* $(TARGET_DIR)/usr/lib/qt/examples/
endef
endif

ifneq ($(BR2_STATIC_LIBS),y)
define QT5WEBENGINE_INSTALL_TARGET_LIBS
	cp -dpf $(STAGING_DIR)/usr/lib/libQt5WebEngine*.so.* $(TARGET_DIR)/usr/lib
	cp -dpf $(STAGING_DIR)/usr/libexec/QtWebEngineProcess $(TARGET_DIR)/usr/libexec/
	cp -dpfr $(STAGING_DIR)/usr/resources/ $(TARGET_DIR)/usr/
	cp -dpfr $(STAGING_DIR)/usr/translations/qtwebengine_locales $(TARGET_DIR)/usr/translations/
	-cp -dpf $(STAGING_DIR)/usr/icudtl.dat $(TARGET_DIR)/usr/
endef
endif

define QT5WEBENGINE_INSTALL_TARGET_CMDS
	$(QT5WEBENGINE_INSTALL_TARGET_LIBS)
	$(QT5WEBENGINE_INSTALL_TARGET_QMLS)
	$(QT5WEBENGINE_INSTALL_TARGET_EXAMPLES)
endef

$(eval $(generic-package))
