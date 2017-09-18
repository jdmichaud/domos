#!/bin/sh

ACCESS_POINT_IP=$(netstat -rn | grep UG | awk -F' ' '{ print $2 }')
echo "Access point IP is $ACCESS_POINT_IP"

IP=$(ip addr | awk '/inet/ && /wlan0/{sub(/\/.*$/,"",$2); print $2}')
MAC=$(cat /sys/class/net/wlan0/address)

curl -sL \
  -X POST \
  -d '{ "ip": "'$IP'", "mac": "'$MAC'" }' \
  -H 'Content-type: application/json' \
  $ACCESS_POINT_IP:12000/api/camera/

