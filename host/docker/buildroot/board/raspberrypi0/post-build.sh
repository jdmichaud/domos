#!/bin/sh

set -u
set -e

# Add a console on tty1
if [ -e ${TARGET_DIR}/etc/inittab ]; then
    grep -qE '^tty1::' ${TARGET_DIR}/etc/inittab || \
	sed -i '/GENERIC_SERIAL/a\
tty1::respawn:/sbin/getty -L  tty1 0 vt100 # HDMI console' ${TARGET_DIR}/etc/inittab
fi

CONFIG_FILE=${TARGET_DIR}/../images/rpi-firmware/config.txt

# Increase the memory allocated to the VideoCore chip
sed -i s/gpu_mem_512=100/gpu_mem_512=256/ ${CONFIG_FILE}

# Activate the console
if grep -q enable_uart=1 ${CONFIG_FILE}
then
  echo "uart already configured"
else
  echo "enable_uart=1" | cat >> ${CONFIG_FILE}
fi
