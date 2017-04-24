#!/bin/sh

set -u
set -e

CONFIG_FILE=${TARGET_DIR}/../images/rpi-firmware/config.txt

# Disable overscan
sed -i s/disable_overscan=1/#disable_overscan=1/ ${CONFIG_FILE}
