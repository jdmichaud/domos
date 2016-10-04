# vim:set ft=dockerfile:
FROM ubuntu:xenial

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

# A few utility for debugging purposes
RUN apt-get install -y \
  nano \
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
  less \
  sudo

# RethinkDB dependencies
RUN apt-get install -y \
  make \
  debhelper \
  ccache \
  --no-install-recommends

#
# RPi tool chain
#
USER jedi
RUN cd /home/jedi && \
  git clone https://github.com/raspberrypi/tools.git && \
  mv tools rpi-toolchain && \
  cd rpi-toolchain && \
  git checkout b65595ffb74e5853ba61916f49bdbccfc54f1300 && \
  echo "export PI_TOOLS_HOME=$HOME/rpi-toolchain" >> ~/.bashrc
ENV PI_TOOLS_HOME /home/jedi/rpi-toolchain/
# Retrieve the CMake toolchain configuration file
ADD https://raw.githubusercontent.com/jdmichaud/domos/master/rpi/cmaketc/Toolchain-raspberrypi-arm.cmake /home/jedi/Toolchain-raspberrypi-arm.cmake
# Setup the environment for cross-compiling
ENV CROSS=/home/jedi/rpi-toolchain/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf
ENV CC=$CROSS-gcc
ENV CXX=$CROSS-g++
ENV LD=$CROSS-ld
ENV AR=$CROSS-ar
ENV RANLIB=$CROSS-ranlib

#
# Download and package rethinkdb
#
RUN cd /home/jedi && \
  curl -L https://github.com/rethinkdb/rethinkdb/archive/v2.3.5.tar.gz -o rethinkdb-2.3.5.tar.gz && \
  tar zxvf rethinkdb-2.3.5.tar.gz && \
  cd rethinkdb-2.3.5 && \
  rm -rf external/v8*
#  ./configure --allow-fetch --with-system-malloc --ccache && \
#  UBUNTU_RELEASE=trusty SIGN_PACKAGE=0 make build-deb -j `nproc`

USER root
