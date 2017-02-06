# Build with
# docker build -t domos-env .

## # Run with
## # user$ docker run -it --rm --privileged --cap-add=ALL -v /lib/modules:/lib/modules buildroot
## # `--rm --privileged --cap-add=ALL` is to modprobe nfs and nfsd

FROM ubuntu:xenial

# For the proxy

# Create user
RUN useradd jedi --create-home --password jedi && echo "jedi:jedi" | chpasswd && adduser jedi sudo

RUN apt-get update
# Some utilities
RUN apt-get install -y build-essential \
	sudo \
	nano \
	vim \
	unzip \
	curl \
	wget \
	git \
  tig \
	lsb-release \
	minicom \
	silversearcher-ag \
  gdb \
# Python
	python \
	python-dev \
	python-setuptools \
# to avoid urllib warning, we need pyopenssl which needs libssl-dev + libffi-dev
	libffi-dev \
	libssl-dev \
# C++
	clang \
	cmake \
	google-mock \
# For X11 application (i.e. Clion)
  libxrender1 \
  libxtst6 \
  libxi6 \
  libfreetype6 \
  libfontconfig1

# pip
RUN easy_install pip
RUN pip install pyopenssl ndg-httpsclient pyasn1

# To use the RPi cross-compilation toolchain,
# which are composed of 32bits executables
RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install -y libz1:i386 libncurses5:i386 libbz2-1.0:i386 libc6:i386 libstdc++6:i386

# Install platformio
RUN pip install platformio

USER jedi
#
# RPi tool chain
#
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
ENV CROSS=/home/jedi/rpi-toolchain/${PI_TOOL}/bin/arm-linux-gnueabihf
ENV CC=$CROSS-gcc
ENV CXX=$CROSS-g++
ENV LD=$CROSS-ld
ENV AR=$CROSS-ar
ENV RANLIB=$CROSS-ranlib

#
# Google Test/Mock
#
### ARM
# Download google test/mock and compile it
RUN cd /home/jedi && wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz
USER root
RUN chown jedi /home/jedi/release-1.8.0.tar.gz
USER jedi
RUN cd /home/jedi && tar zxf release-1.8.0.tar.gz && \
  cd /home/jedi/googletest-release-1.8.0 && \
  cmake -DCMAKE_TOOLCHAIN_FILE=/home/jedi/Toolchain-raspberrypi-arm.cmake && \
  make DESTDIR=${PI_TOOLS_HOME}/${PI_TOOL} install && \
  echo '' | cat >> /home/jedi/Toolchain-raspberrypi-arm.cmake && \
  echo 'SET (ENV{GTEST_ROOT} $ENV{PI_TOOLS_HOME}/$ENV{PI_TOOL}/)' | cat >> /home/jedi/Toolchain-raspberrypi-arm.cmake
### amd64
# Google Test/Mock to compile with x86 as target
RUN cd /usr/src/gmock && cmake CMakeLists.txt && make

#
# libcurl
#
# ARM
RUN cd /home/jedi/ && \
  curl -LO https://curl.haxx.se/download/curl-7.51.0.tar.gz && \
  tar zxf curl-7.51.0.tar.gz && \
  cd curl-7.51.0 && \
  ./configure --host=arm-bcm2708hardfp-linux-gnueabi --prefix=${PI_TOOLS_HOME}/${PI_TOOL} --with-ssl && \
  make -j `$(nproc)` && \
  make install
### amd64
# Installed with apt-get

#
# Finalize image
#
USER jedi

# Force color prompt
RUN sed -i 's/#force_color_prompt=yes/force_color_prompt=yes/g' /home/jedi/.bashrc

# Git configuration
RUN git config --global user.email "jean.daniel.michaud@gmail.com" && \
  git config --global user.name "JD"

# Vim configuration
# dotfile configuration
RUN cd /home/jedi && \
  git clone https://github.com/jdmichaud/dotfiles && \
  rm -fr /home/jedi/.mybashrc && \
  ln -s /home/jedi/dotfiles/.mybashrc /home/jedi/.mybashrc && \
  ln -s /home/jedi/dotfiles/.vimrc /home/jedi/.vimrc && \
  ln -s /home/jedi/dotfiles/.vimrc.local /home/jedi/.vimrc.local && \
  ln -s /home/jedi/dotfiles/.vimrc.local.bundles /home/jedi/.vimrc.local.bundles && \
  ln -s /home/jedi/dotfiles/.tmux.conf /home/jedi/.tmux.conf && \
  ln -s /home/jedi/dotfiles/.git /home/jedi/.git && \
  git clone https://github.com/jdmichaud/sh

# Set prompt with image name
RUN echo 'export PS1="`echo $PS1 | sed s/@.h/@rpi-build/g` "' >> /home/jedi/.profile
