# Development container

Creates a container to run on your development machine with all the tools to develop on raspeberry.
Contain the NFS serveur configuration for the raspberry to use as a filesystem.

## On Mac

See [this page](https://thefullsnack.com/creating-virtual-dev-environment-with-xhyve-fe501005fc6c#.vkbwxqazc) to use docker on mac.

## Run the docker

Suppose domos has been checked out in the current directory.

```
docker run -it -v <path_to>/domos:/home/jedi/domos  domos-env bash
```