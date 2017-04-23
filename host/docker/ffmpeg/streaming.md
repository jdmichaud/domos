
Capture video with `ffmpeg` (`cvlc` crashes with floating point, looking at vlc bug
thread, there isn't much maintenance going on...).

### Using UDP

capture command line:
```
ffmpeg -i /dev/video0 -f mpegts udp://127.0.0.1:1234
```

`cvlc` can be used to display the video:
```
cvlc udp://@:1234
```

### Using RTP

```
ffmpeg -i /dev/video0 -vsync 2 -f rtp rtp://127.0.0.1:1234
# or for better quality
ffmpeg -i /dev/video0 -vcodec copy -f rtp rtp://127.0.0.1:1234
```

framerate can be adjusted with `-framerate 25`

resolution can be adjusted with `-video_size 320x240`

ffmpeg produce an SDP file in its standard output. Copy it in a file and then:
```
cvlc sdp.file
```

ffplay seems to work better in rdp mode:
```
ffplay -i sdp.file -fflags nobuffer -flags low_delay -framedrop -strict experimental
```

### RPi hardware acceleration

First cross-compile ffmpeg using the Dockerfile in this folder.
Then, on the raspberry
```
./ffmpeg -i /dev/video0 -s 960x720 -vcodec h264_omx -f rtp rtp://192.168.0.101:1234
```

For a better quality, increase the bitrate:
```
./ffmpeg -i /dev/video0 -b:v 1M -s 960x720 -vcodec h264_omx -f rtp rtp://192.168.0.101:1234
```

### Resources

https://trac.ffmpeg.org/wiki/StreamingGuide

# Gstreamer

ffmpeg being unable to instruct the hardware encoder to produce intermediary key
frames in the stream, we have to resort to gstreamer:
```
GS_DEBUG=2 LD_PRELOAD=libEGL.so:libGLESv2.so gst-launch-1.0 -vv \
    v4l2src ! \
    videoscale ! \
    videorate  ! \
    videoconvert ! \
    video/x-raw,width=320,height=240,framerate=30/1,format=I420 ! \
    omxh264enc inline-header=true periodicty-idr=1 target-bitrate=300000 control-rate=variable ! \
    video/x-h264,profile=high,width=320,height=240,framerate=30/1 ! \
    h264parse ! \
    mpegtsmux ! \
    tcpserversink host=0.0.0.0 port=8090
```

This can be read by a VLC but not by a browser, which do not support mpeg2
transport layer containers. Use flv instead:

```
GS_DEBUG=2 LD_PRELOAD=libEGL.so:libGLESv2.so gst-launch-1.0 -vv \
    v4l2src ! \
    videoscale ! \
    videorate  ! \
    videoconvert ! \
    video/x-raw,width=320,height=240,framerate=30/1,format=I420 ! \
    omxh264enc inline-header=true periodicty-idr=1 target-bitrate=300000 control-rate=variable ! \
    video/x-h264,profile=high,width=320,height=240,framerate=30/1 ! \
    h264parse ! \
    flvmux streamable=true ! \
    tcpserversink host=0.0.0.0 port=8090
```

But this does not work yet...
