# vim:set ft=dockerfile:
FROM ubuntu:xenial

MAINTAINER jdmichaud

# Set the application name
ENV APPLICATION ffmpeg-compilation-enc

# explicitly set user/group IDs
RUN useradd jedi --create-home --password jedi && echo "jedi:jedi" | chpasswd && adduser jedi sudo

#
# Install some packages
#
USER root
RUN apt-get update

# A few utility for debugging purposes
RUN apt-get install -y \
  nano \
  lsof \
  tmux \
  vim \
  curl \
  file \
  git \
  build-essential \
  gcc \
 # python \
 # python-dev \
 # python-pip \
  less \
  sudo \
  bc

# RethinkDB dependencies
RUN apt-get install -y \
  make \
  cmake \
#  debhelper \
  ccache \
  m4 \
  g++ \
  --no-install-recommends

# Replace shell with bash so we can source files
RUN rm /bin/sh && ln -s /bin/bash /bin/sh

USER jedi

#
# RPi tool chain
#
# Preparing the cross-compilation environment
RUN cd /home/jedi && \
  git clone https://github.com/raspberrypi/tools.git && \
  mv tools rpi-toolchain && \
  cd rpi-toolchain && \
  git checkout b65595ffb74e5853ba61916f49bdbccfc54f1300 && \
  echo "export PI_TOOLS_HOME=$HOME/rpi-toolchain" >> ~/.bashrc
ENV PI_TOOLS_HOME /home/jedi/rpi-toolchain/
# Retrieve the CMake toolchain configuration file. Do we need that???
ADD https://raw.githubusercontent.com/jdmichaud/domos/master/rpi/cmaketc/Toolchain-raspberrypi-arm.cmake /home/jedi/Toolchain-raspberrypi-arm.cmake
# Setup the environment for cross-compiling
ENV CROSS=/home/jedi/rpi-toolchain/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf
ENV CC=$CROSS-gcc
ENV CXX=$CROSS-g++
ENV LD=$CROSS-ld
ENV AR=$CROSS-ar
ENV RANLIB=$CROSS-ranlib

#
# Download, compile and package ffmpeg
#
RUN cd /home/jedi && \
  # Userland contains libmmal used by ffmpeg to access the GPU h264 encoder
  git clone https://github.com/raspberrypi/userland.git --depth=1 \
  git clone https://github.com/FFmpeg/FFmpeg.git --depth=1 \
  cd FFmpeg

