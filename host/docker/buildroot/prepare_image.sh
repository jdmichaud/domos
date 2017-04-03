#!/bin/bash

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
  echo "the script assume the buildroot source folder to be 'buildroot'"
  echo -n "available images: "
  for image in "${!images[*]}"
  do
    echo "$image"
  done
}

# Prepare the domos controller image.
# Assuming a RPi 2 for now...
function controller() {
  echo "... $1 ..."
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
${images[camera]} $2
echo "$1 image prepared."
