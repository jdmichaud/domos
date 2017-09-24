#!/bin/sh

. /root/commonconf

while true
do
  rm -rf /tmp/receiver-camera-pipe && mkfifo /tmp/receiver-camera-pipe
  nc -l 0.0.0.0:$CAMERA_REST_API_PORT < /tmp/receiver-camera-pipe | receiver-camera.sh > /tmp/receiver-camera-pipe
done

