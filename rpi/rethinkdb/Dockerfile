# vim:set ft=dockerfile:
FROM ubuntu:xenial

MAINTAINER novidee

# Set the application name
ENV APPLICATION rethinkdb-compilation-env

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
  sudo \
  bc

 RUN libboost-dev libssl-dev python libncurses5-dev libcurl4-openssl-dev protobuf-compiler libprotobuf-dev

# RethinkDB dependencies
RUN apt-get install -y \
  make \
  debhelper \
  ccache \
  m4 \
  g++ \
  --no-install-recommends

RUN curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.32.0/install.sh | bash

#
# RPi tool chain
#
USER jedi
# rethinkgb can't compile node-0.12.2 on ARM apparently so installing it with nvm
RUN nvm install 0.12.2
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
  rm -rf external/v8* && \
  sed -i 's/rfakeroot $(DEBUILD_SIGN_OPTIONS)/rfakeroot $(DEBUILD_SIGN_OPTIONS) -d/g' mk/packaging.mk && \
#  sed -i 's@./configure@./configure --allow-fetch --ccache@g' packaging/debian/rules && \
  sed -i 's@./configure@./configure --allow-fetch@g' packaging/debian/rules && \
  # https://github.com/rethinkdb/rethinkdb/issues/3312
  sed -i 's@$build_dir/third_party/icu" ./configure@$build_dir/third_party/icu/source" ./configure@g' mk/support/pkg/v8.sh && \
  sed -i 's/--enable-static "$@"/--enable-static --disable-layout "$@"/g' mk/support/pkg/v8.sh && \
#  ./configure --allow-fetch --ccache && \
  ./configure --allow-fetch && \
  # https://github.com/rethinkdb/rethinkdb/issues/5776#issuecomment-220168597
  make build-openssl -j `echo $(nproc) - 1 | bc` && \
  UBUNTU_RELEASE=trusty SIGN_PACKAGE=0 make build-deb -j `echo $(nproc) - 1 | bc` SHELL="/bin/bash -x"

USER root
