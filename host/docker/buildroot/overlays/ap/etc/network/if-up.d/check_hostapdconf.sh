#!/bin/bash
#
# This script is launched everytime the wlan0 interface is started.
# It will check is a wifi network is configured and if it is not, will
# will execute set_ssid.sh.
#

FILE=/etc/hostapd.conf
SUBCOMMAND=set_ssid_ap

# Sanity check
if [[ ! -f $FILE ]]
then
  echo "Error: No interface file $FILE"
  exit 1
fi

# If the interface file contains {{SSID}}, it means we need to configure it
grep {{SSID}} $FILE > /dev/null
if [[ $? -ne 0 ]]
then
  echo "AP already configured"
  exit 0
fi

bash -c $SUBCOMMAND
