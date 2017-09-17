#!/bin/sh

IFACE=$1

echo "TOTO"

if [[ $IFACE != "wlan0" && $IFACE != "eth0" ]];
then
  echo "$0: interface is $IFACE: ignored"
  exit 0
fi

# Wait for the interface to actually start to start
while [ 1 ]; do
  if [ $(ip add sh dev $IFACE | grep 'inet ' | wc -l) -ne 0 ]; then
     break
  fi
  echo "$0: Waiting for $IFACE to get an IP"
  sleep 1
done

IP_ADDR=$(ifconfig $IFACE | grep 'inet addr' | cut -c 21-33 | tr -d [:space:])
echo "$0: Address to broadcast images: ${IP_ADDR}:8000"

cd /root/www/ &&
rm -fr playlist.m3u8 segment* &&
GS_DEBUG=2 LD_PRELOAD=libEGL.so:libGLESv2.so gst-launch-1.0 -vv \
  v4l2src ! \
  videoconvert ! \
  video/x-raw,width=640,height=480,framerate=30/1,format=I420 ! \
  omxh264enc inline-header=true periodicty-idr=90 target-bitrate=300000 control-rate=variable ! \
  video/x-h264,profile=high,width=640,height=480,framerate=30/1 ! \
  h264parse ! \
  mpegtsmux ! \
  hlssink max-files=50 playlist-root=http://${IP_ADDR}:8000 target-duration=1

