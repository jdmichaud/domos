#!/bin/sh

. /root/commonconf

case "$1" in
  bound)
    ACCESS_POINT_IP=$(netstat -rn | grep UG | awk -F' ' '{ print $2 }')
    echo "Access point IP is $ACCESS_POINT_IP"

    IP=$(ip addr | awk '/inet/ && /wlan0/{sub(/\/.*$/,"",$2); print $2}')
    MAC=$(cat /sys/class/net/wlan0/address)

    curl -sL \
      -X POST \
      -d '{ "ip": "'$IP'", "mac": "'$MAC'" }' \
      -H 'Content-type: application/json' \
      http://$ACCESS_POINT_IP:$CAMERA_REST_API_PORT/api/camera/
  ;;
esac
