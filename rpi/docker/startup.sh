#!/bin/bash

# Script fails on first error
set -e

# Change local gpio group GID to the host gpio group GID so use jedi has access to /sys/class/gpio
sed -r "s/gpio:([^:]*):[0-9]*/gpio:\1:$(grep gpio ~/host/etc/group | awk -F ':' '{ print $3 }')/g" /etc/group > /tmp/group
mv /tmp/group /etc/group
