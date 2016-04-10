# To build the image

On your raspberry, in this folder, type:
```
docker build -t domos .
```

# To run the container

```
docker run -it --privileged domos bash
```

`--privileged` is necessary to be able to use sshfs.
