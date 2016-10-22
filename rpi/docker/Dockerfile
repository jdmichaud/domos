# vim:set ft=dockerfile:

# Instructions
# Run that image with --privileged option for mount.home to work
# e.g.: docker -it --privileged domos bash

FROM sdhibit/rpi-raspbian

MAINTAINER novidee

# Set the application name
ENV APPLICATION domos

# explicitly set user/group IDs
RUN useradd jedi --create-home --password jedi && echo "jedi:jedi" | chpasswd && adduser jedi sudo

#
# Install some packages
#
USER root
RUN apt-get update

# A few utilisy for debugging purposes
RUN apt-get install -y \
  nano \
  less \
  lsof \
  net-tools \
  inetutils-ping \
  tmux \
  vim \
  curl \
  file \
  git \
  build-essential \
  gcc \
  python \
  python-dev \
  python-pip \
  strace \
  --no-install-recommends

# To compile wiringPi
RUN apt-get install -y \
  sudo \
  make \
  gcc \
  --no-install-recommends

# Fix premission /sys/class/gpio
# http://stackoverflow.com/questions/30938991/access-gpio-sys-class-gpio-as-non-root/30940526#30940526
RUN groupadd gpio
RUN usermod -a -G gpio root && usermod -a -G gpio jedi
RUN echo -n 'SUBSYSTEM=="gpio*", PROGRAM="/bin/sh -c ' | cat >> /etc/udev/rules.d/99-com.rules
RUN echo -n "'chown -R root:gpio /sys/class/gpio && chmod -R 770 /sys/class/gpio; chown -R root:gpio /sys/devices/virtual/gpio && chmod -R 770 /sys/devices/virtual/gpio'" | cat >> /etc/udev/rules.d/99-com.rules
RUN echo '"' | cat >> /etc/udev/rules.d/99-com.rules

# Configure sshfs
RUN apt-get install -y sshfs && \
  chmod 666 /dev/fuse && \
  groupadd fuse && \
  adduser jedi fuse && \
  chown root:fuse /dev/fuse

# Copy the script used to mount home from host
ADD mount.home /home/jedi/
RUN chown jedi:jedi /home/jedi/mount.home && chmod +x /home/jedi/mount.home

# When compiled with the toolchain, a binray will look gor /lib/ld-linux.so.3
# which is named /lib/ld-linux-armhf.so.3 on the pi.
RUN ln -s /lib/ld-linux-armhf.so.3 /lib/ld-linux.so.3

# Download and compile wiringPi in order to enable GPIO lines and not have to
# run programs that read/write GPIO with sudo
RUN cd ~/ && git clone git://git.drogon.net/wiringPi && \
  cd wiringPi && \
  git checkout b0a60c3302973ca1878d149d61f2f612c8f27fac && \
  ./build

# Clean up
RUN rm -rf /var/lib/apt/lists/*

# Force color prompt
RUN sed -i 's/#force_color_prompt=yes/force_color_prompt=yes/g' /home/jedi/.bashrc

# Set prompt with image name
RUN echo 'export PS1="`echo $PS1 | sed s/@.h/@domos/g` "' >> /home/jedi/.profile

ADD startup.sh /root/
RUN chmod +x /root/startup.sh
CMD bash -c /root/startup.sh; bash
