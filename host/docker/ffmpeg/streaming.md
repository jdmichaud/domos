
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
