# Development container

Creates a container to run on your development machine with all the tools to develop on arduino.

## Run the docker

Suppose domos has been checked out in the current directory.

```
docker run -it -v <path_to>/domos/:/home/jedi/domos --device /dev/ttyACM0:/dev/ttyACM0 arduino-dev bash --login
```

`/dev/ttyACM0` is the device on which the pololu programmer is mapped to. Your mileage may vary...