################################################################################
#
# libhttp
#
################################################################################

LIBHTTP_VERSION = master
LIBHTTP_SITE = git://github.com/jdmichaud/domos.git
LIBHTTP_SITE_METHOD = git
LIBHTTP_LICENSE = Apache v2.0
LIBHTTP_LICENSE_FILES = LICENSE
LIBHTTP_DEPENDENCIES = libcurl
# Need to install the library in output/staging so that we can link with it
LIBHTTP_INSTALL_STAGING = YES
# We don't need the library in the final image, as we won't compile
# anything on the raspberry
LIBHTTP_INSTALL_TARGET = NO
# This is where the library sources are in the domos git repo
LIBHTTP_SUBDIR = lib/http

$(eval $(cmake-package))
