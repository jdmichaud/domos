#!/bin/bash
#
# This script is launched everytime the wlan0 interface is started.
# It will check is a wifi network is configured and if it is not, will
# will execute set_ssid.sh.
#

INTERFACE_FILE=/etc/network/interfaces
SUBCOMMAND=set_ssid

# Sanity check
if [[ ! -f $INTERFACE_FILE ]]
then
  echo "Error: No interface file $INTERFACE_FILE"
  exit 1
fi

# If the interface file contains {{SSID}}, it means we need to configure it
grep {{SSID}} $INTERFACE_FILE > /dev/null
if [[ $? -ne 0 ]]
then
  echo "WiFi already configured"
  exit 0
fi

bash -c $SUBCOMMAND
