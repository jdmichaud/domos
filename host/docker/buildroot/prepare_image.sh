#!/bin/bash

# This script configures the buildroot environment for a
# particular target configuration.
# It will:
# (i)   Copy the configuration to the buildroot destination folder
# (ii)  Copy the packages to the buildroot destination folder
# (iii) Modify the packages/Config.in to include the copied packages
# (iv)  Overwrite the post-* scripts in the board/raspberry* folder
# The script has to be run in the docker container configured with the proper
# compilation environment for the buildroot make process.

set -e

declare -A images
images=(
  ["controller"]=controller
  ["camera"]=camera
)

# Check if a key exist in an associative array
function exists() {
  if [ "$2" != in ]; then
    return
  fi
  eval '[ ${'$3'[$1]+muahaha} ]'
}

# Display usage message with list of images
function usage() {
  echo "usage: $0 <image-name> <dest-builroot-folder>"
  echo "the script assume the buildroot source folder (from the git repo) to be "
  echo "'./buildroot'"
  echo -n "available images: "
  for image in "${!images[*]}"
  do
    echo "$image"
  done
}

# Prepare the domos controller image.
# Assuming a RPi 2 for now...
function controller() {
  echo "Preparing controller image..."
  local destination
  destination=$1

  ln --verbose --symbolic --force `pwd`/buildroot/configs/raspberrypi2_defconfig ${destination}/configs/
  ln --verbose --symbolic --force `pwd`/buildroot/package/domos ${destination}/package/
  ln --verbose --symbolic --force `pwd`/buildroot/package/rcswitch ${destination}/package/
  ln --verbose --symbolic --force `pwd`/buildroot/package/RF24 ${destination}/package/
  ln --verbose --symbolic --force `pwd`/buildroot/package/433Utils ${destination}/package/
  if [ -f ${destination}/package/Config.in.orig ]
  then
  	# If the Config.in has already been modified, reinitialize it with the original
    cp --verbose --remove-destination \
      ${destination}/package/Config.in.orig ${destination}/package/Config.in
  else
	  # otherwise, back it up
    cp --verbose --no-clobber \
      ${destination}/package/Config.in ${destination}/package/Config.in.orig
    fi
  sed --in-place '$imenu "Domos packages"\
        source "package/rcswitch/Config.in"\
        source "package/433Utils/Config.in"\
        source "package/RF24/Config.in"\
        source "package/domos/libhttp/Config.in"\
        source "package/domos/librfprotocol/Config.in"\
        source "package/domos/receiver/Config.in"\
        source "package/domos/kiosk/Config.in"\
endmenu\
\
' ${destination}/package/Config.in
  # wiringPi has some trouble to build in parallel
  patch --forward --reject-file=- ${destination}/package/wiringpi/wiringpi.mk << 'EOF'
--- a/wiringpi.mk       2017-05-13 18:46:12.251577789 +0000
+++ b/wiringpi.mk       2017-05-13 18:46:37.943713759 +0000
@@ -37,8 +37,8 @@
 endef

 define WIRINGPI_INSTALL_TARGET_CMDS
-	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)/wiringPi $(WIRINGPI_LIB_INSTALL_TARGETS) DESTDIR=$(TARGET_DIR) PREFIX=/usr LDCONFIG=true
-	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)/devLib $(WIRINGPI_LIB_INSTALL_TARGETS) DESTDIR=$(TARGET_DIR) PREFIX=/usr LDCONFIG=true
+	$(TARGET_MAKE_ENV) $(MAKE1) -C $(@D)/wiringPi $(WIRINGPI_LIB_INSTALL_TARGETS) DESTDIR=$(TARGET_DIR) PREFIX=/usr LDCONFIG=true
+	$(TARGET_MAKE_ENV) $(MAKE1) -C $(@D)/devLib $(WIRINGPI_LIB_INSTALL_TARGETS) DESTDIR=$(TARGET_DIR) PREFIX=/usr LDCONFIG=true
	$(INSTALL) -D -m 0755 $(@D)/gpio/gpio $(TARGET_DIR)/usr/bin/gpio
	$(INSTALL) -D -m 0755 $(@D)/gpio/pintest $(TARGET_DIR)/usr/bin/pintest
 endef
EOF
  # No board script to be overridden
}

# Prepare the domos camera.
# Assuming a RPi Zero W.
function camera() {
  echo "... $1 ..."
}

# Check the number of argument
if [[ $# -ne 2 ]]
then
  usage
  exit 1
fi

# Check the image name exist
if ! exists $1 in images
then
  echo "No such image" $1
  usage
  exit 1
fi

# Call the image
${images[$1]} $2
echo "$1 image prepared."
