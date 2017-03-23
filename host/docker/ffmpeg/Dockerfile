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
  bc \
# for FFmpeg
  yasm

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
ENV PI_TOOL arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64
# Retrieve the CMake toolchain configuration file
ADD https://raw.githubusercontent.com/jdmichaud/domos/master/rpi/cmaketc/Toolchain-raspberrypi-arm.cmake /home/jedi/Toolchain-raspberrypi-arm.cmake
# Give ownership of ADDED files to the user. ADD always create the user as owned by root...
USER root
RUN chown jedi /home/jedi/Toolchain-raspberrypi-arm.cmake
USER jedi
# Set cross compiler environment
ENV CROSS=${PI_TOOLS_HOME}/${PI_TOOL}/bin/arm-linux-gnueabihf
ENV CC=$CROSS-gcc
ENV CXX=$CROSS-g++
ENV LD=$CROSS-ld
ENV AR=$CROSS-ar
ENV RANLIB=$CROSS-ranlib

#
# Download, compile and package ffmpeg
#
RUN cd /home/jedi && \
  ## userland
  # Userland contains libmmal used by ffmpeg to access the GPU h264 encoder
  git clone https://github.com/raspberrypi/userland.git --depth=1 && \
  cd userland && \
  # Use our own Toolchain configration
  sed -ri 's@../../../makefiles/cmake/toolchains/arm-linux-gnueabihf.cmake@../../../../Toolchain-raspberrypi-arm.cmake@' buildme && \
  # Compile userland
  ./buildme && \
  cd .. && \
  ## libvpx
  git clone https://chromium.googlesource.com/webm/libvpx && \
  cd libvpx && \
  ./configure && \
  make -j `nproc` && \
  cd .. && \
  ## FFmpeg
  git clone https://github.com/FFmpeg/FFmpeg.git --depth=1 && \
  cd FFmpeg && \
  # Set the library PATH
  export LD_LIBRARY_PATH=/home/jedi/rpi-toolchain/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/libc/usr/lib/arm-linux-gnueabihf/ && \
  # This is for cc1 used by gcc. No other place to specify in the command line so we change the PATH
  export PATH=$PATH:${PI_TOOLS_HOME}/${PI_TOOL}/libexec/gcc/arm-linux-gnueabihf/4.8.3/ && \
  ./configure --enable-cross-compile --arch=armel --cpu=cortex-a7 --target-os=linux --sysroot=${PI_TOOLS_HOME}/${PI_TOOL}/arm-linux-gnueabihf/libc --sysinclude=${PI_TOOLS_HOME}/${PI_TOOL}/lib/gcc/arm-linux-gnueabihf/4.8.3/include --cross-prefix=${PI_TOOLS_HOME}/${PI_TOOL}/bin/arm-linux-gnueabihf- --prefix=/tmp/ --enable-mmal --enable-omx-rpi --enable-omx --enable-libvpx --extra-cflags="-I../userland/ -I../userland/interface/vmcs_host/khronos/IL/ -I../userland/host_applications/linux/libs/bcm_host/include/" --extra-ldflags="-L../userland/build/lib/" --extra-libs="-lmmal_core -lmmal -lmmal_util -lvcos -lcontainers -lbcm_host -lmmal_vc_client -lmmal_components -lvchiq_arm -lvcsm" && \
  make -j `nproc`
