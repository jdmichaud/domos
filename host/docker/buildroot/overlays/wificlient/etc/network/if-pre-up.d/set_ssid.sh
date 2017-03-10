#!/bin/bash
#
# Will prompt for a SSID and password and modify the WiFi configuration file.
#

# Define some colors
NC='\[\033[0m\]' # No Color
RED='\[\033[0;31m\]'

INTERFACE_FILE=/etc/network/interfaces
WPA_FILE=/etc/wpa_supplicant.conf

# Sanity check
if [[ ! -f $INTERFACE_FILE ]]
then
  echo "Error: No interface file $INTERFACE_FILE"
  exit 1
fi
if [[ ! -f $WPA_FILE ]]
then
  echo "Error: No wpa supplicant configuration file $WPA_FILE"
  exit 1
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

# Check our replacement will work
grep 'psk=' $WPA_FILE > /dev/null
if [[ $? -ne 0 ]]
then
  echo "${RED}Error:${NC} $WPA_FILE do not contain a psk field"
  exit 1
fi

# Replace the password
sed -ir s/psk=.*/psk=$KEY/ $WPA_FILE

echo "Wifi configuration to SSID $SSID done"
