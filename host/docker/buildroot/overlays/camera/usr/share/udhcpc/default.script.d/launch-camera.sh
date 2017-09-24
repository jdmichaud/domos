#!/bin/sh

case "$1" in
  bound)
		# Only works on wlan0 for now
		/etc/init.d/camera start wlan0
  ;;
esac

