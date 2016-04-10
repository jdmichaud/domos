# vim:set ft=dockerfile:

# Instructions
# Run that image with --privileged option for mount.home to work
# e.g.: docker -it --provoleged domos bash

FROM sdhibit/rpi-raspbian

MAINTAINER novidee

# Set the application name
ENV APPLICATION domos

# explicitly set user/group IDs
RUN useradd -ms /bin/bash jedi
RUN chown -R jedi:jedi /home/jedi/

#
# Install some packages
#
USER root
RUN apt-get update

# A few utilisy for debugging purposes
RUN apt-get install -y nano
RUN apt-get install -y lsof
RUN apt-get install -y net-tools
RUN apt-get install -y inetutils-ping
RUN apt-get install -y tmux
RUN apt-get install -y vim
RUN apt-get install -y curl
# Git
RUN apt-get install -y git

# Configure sshfs
RUN apt-get install -y sshfs
RUN chmod 666 /dev/fuse
RUN groupadd fuse
RUN adduser jedi fuse
RUN chown root:fuse /dev/fuse

# Copy the script used to mount home from host
ADD mount.home /home/jedi/
RUN chown jedi:jedi /home/jedi/mount.home
RUN chmod +x /home/jedi/mount.home
