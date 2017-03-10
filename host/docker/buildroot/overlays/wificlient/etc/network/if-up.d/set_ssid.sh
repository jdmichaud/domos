#!/bin/bash

#
# This script is launched everytune the wlan0 interface is started.
# It will check is a wifi network is condifured and if it is not, will ask
# for a SSID and a password and configure it.
#

INTERFACE_FILE=/etc/network/interfaces
WPA_FILE=/etc/wpa_supplicant.conf

echo $INTERFACE_FILE

# If the interface file contains {{SSID}}, it means we need to configure it
grep {{SSID}} $INTERFACE_FILE > /dev/null
if [[ $? -ne 0 ]]
then
  echo "WiFi already configured"
  exit 0
fi

# Prompt the user for SSID and password
SSID=
while [[ $SSID = "" ]]; do
	read -p "ssid: " SSID
done
read -p "wifi password (won't be stored): " -s PASSWORD

# Replace the SSID
sed -i s/{{SSID}}/Kramer2/g $INTERFACE_FILE
sed -i s/{{SSID}}/Kramer2/g $WPA_FILE

# Hash the password
KEY=`wpa_passphrase $SSID $PASSWORD | egrep "[^#]psk" | awk -F'=' '{ print $2 }'`

# Replace the password
sed -ir s/psk=.*/psk=$KEY/ $WPA_FILE

