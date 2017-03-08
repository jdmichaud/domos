#!/bin/bash

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

read -p "ssid: " SSID
read -p "wifi password (won't be stored): " -s PASSWORD

# Replace the SSID
sed -i s/{{SSID}}/Kramer2/g $INTERFACE_FILE
sed -i s/{{SSID}}/Kramer2/g $WPA_FILE

# Hash the password
KEY=`wpa_passphrase $SSID $PASSWORD | egrep "[^#]psk" | awk -F'=' '{ print $2 }'`

# Replace the password
sed -ir s/psk=.*/psk=$KEY/ $WPA_FILE

