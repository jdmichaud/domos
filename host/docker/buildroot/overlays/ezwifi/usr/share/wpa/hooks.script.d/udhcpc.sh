#!/bin/sh

IFACE=$1
STATUS=$2

case "$STATUS" in
    CONNECTED)
        echo "Connected to AP through interface $IFACE" > /dev/kmsg
        udhcpc --interface $IFACE --pidfile /var/run/udhcpc.pid
        ;;
    DISCONNECTED)
        echo "Disconnected from AP" > /dev/kmsg
        kill -9 $(cat /var/run/udhcpc.pid)
        ;;
esac
