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
RUN apt-get install -y file

# Git
RUN apt-get install -y git

# To compile wiringPi
RUN apt-get install -y sudo
RUN apt-get install -y make
RUN apt-get install -y gcc

# Fix premission /sys/class/gpio
# http://stackoverflow.com/questions/30938991/access-gpio-sys-class-gpio-as-non-root/30940526#30940526
RUN echo 'SUBSYSTEM=="gpio*", PROGRAM="/bin/sh -c \'chown -R root:gpio /sys/class/gpio && chmod -R 770 /sys/class/gpio; chown -R root:gpio /sys/devices/virtual/gpio && chmod -R 770 /sys/devices/virtual/gpio\'' | cat > /etc/udev/rules.d/99-com.rules

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

# When compiled with the toolchain, a binray will look gor /lib/ld-linux.so.3
# which is named /lib/ld-linux-armhf.so.3 on the pi.
RUN ln -s /lib/ld-linux-armhf.so.3 /lib/ld-linux.so.3

# Download and compile wiringPi in order to enable GPIO lines and not have to
# run programs that read/write GPIO with sudo
RUN cd ~/ && git clone git://git.drogon.net/wiringPi && \
  cd wiringPi && \
  git checkout b0a60c3302973ca1878d149d61f2f612c8f27fac && \
  ./build

USER jedi
# Configure pin 2
RUN gpio export 2 in

USER root
