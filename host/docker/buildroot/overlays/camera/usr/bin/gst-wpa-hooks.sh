#!/bin/bash

IFACE=$1
STATUS=$2

case "$STATUS" in
  CONNECTED)
    echo "Connected to Access Point" > /dev/kmsg

    # Kill all previously running gst-launch if any
    ps | grep gst-launch | grep -v grep | awk -F' ' '{ print $1 }' | xargs kill -9 2>&1 > /dev/null
    # The system needs some time to settle
    sleep 5
    # Start camera straimg
    echo "Starting camera on $IFACE" > /dev/kmsg
    trials=0
    until [[ $trials -lt 3 && $(gst-launch.sh $IFACE > /var/log/gst-launch.log 2>&1) ]]
    do
      echo "Camera failed, trying again" > /dev/kmsg
      trials=$(($trials + 1))
      sleep 5
    done
    ;;
  DISCONNECTED)
    echo "Disconnected from AP" > /dev/kmsg
    ;;
  *)
    echo "Error with $0" > /dev/kmsg
    ;;
esac
