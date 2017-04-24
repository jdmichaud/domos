### Maniupate the framebuffer

To clear the framebuffer:
```
dd if=/dev/zero of=/dev/fb0
```

To display noise:
```
dd if=/dev/urandom of=/dev/fb0
```

To do it a little fasater first dump /dev/urandom in a file the size of the
framebuffer and then dd it to `/dev/fdb0`. You can check the number of bits per
poxel with:
```
cat /sys/class/graphics/fb0/bits_per_pixel
```

### Manipulate `/sys/class`

Query the bits per pixels:
```
cat /sys/class/graphics/fb0/bits_per_pixel
```

the resolution:
```
cat /sys/class/graphics/fb0/virtual_size
```

Wake up from the screen saver:
```
echo 0 > /sys/class/graphics/fb0/blank
```

Go to screen save mode:
```
echo 1 > /sys/class/graphics/fb0/blank
```

### Useful link

`http://www.armadeus.org/wiki/index.php?title=Framebuffer`
