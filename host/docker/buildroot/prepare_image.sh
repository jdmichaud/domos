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
  ln --verbose --symbolic --force `pwd`/buildroot/package/qt5webengine ${destination}/package/qt5/
  ln --verbose --symbolic --force `pwd`/buildroot/package/qt5webview ${destination}/package/qt5/
  ln --verbose --symbolic --force `pwd`/buildroot/package/uclibc/uClibc-ng.config ${destination}/package/uclibc/
  ln --verbose --symbolic --force `pwd`/buildroot/package/set_overscan ${destination}/package/
    # patch the main Config.in to add the domos package
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
        source "package/qt5/qt5webview/Config.in"\
        source "package/set_overscan/Config.in"\
        source "package/domos/libhttp/Config.in"\
        source "package/domos/librfprotocol/Config.in"\
        source "package/domos/receiver/Config.in"\
        source "package/domos/kiosk/Config.in"\
        source "package/domos/frontend/Config.in"\
endmenu\
\
' ${destination}/package/Config.in
  # Patch the qt5 Config.in to add qtwebengine
  patch -d ${destination}/package/qt5/ -p1 << EOF
--- a/Config.in     2017-09-02 16:43:28.069165894 +0000
+++ b/Config.in     2017-08-08 18:55:14.932852395 +0000
@@ -81,2 +81,3 @@
 source "package/qt5/qt5webkit/Config.in"
+source "package/qt5/qt5webengine/Config.in"
 source "package/qt5/qt5websockets/Config.in"
EOF
  # wiringPi has some trouble to build in parallel
  sed --in-place 's/$(MAKE)/$(MAKE1)/g' ${destination}/package/wiringpi/wiringpi.mk
  # No board script to be overridden
}

# Prepare the domos camera.
# Assuming a RPi Zero W.
function camera() {
  echo "Preparing controller image..."
  local destination
  destination=$1

  ln --verbose --symbolic --force `pwd`/buildroot/configs/raspberrypi0_defconfig ${destination}/configs/

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
