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
	lsb-release \
	minicom \
	silversearcher-ag \
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
	google-mock

# pip
RUN easy_install pip
RUN pip install pyopenssl ndg-httpsclient pyasn1

# Google Test/Mock to compile with x86 as target
RUN cd /usr/src/gmock && cmake CMakeLists.txt && make

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
# Retrieve the CMake toolchain configuration file
ADD https://raw.githubusercontent.com/jdmichaud/domos/master/rpi/cmaketc/Toolchain-raspberrypi-arm.cmake /home/jedi/Toolchain-raspberrypi-arm.cmake

#
# Google Test/Mock
#
# Download google test/mock and compile it
RUN cd /home/jedi && wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz
USER root
# Give ownership of ADDED files to the user. ADD always create the user as owned by root...
RUN chown jedi /home/jedi/Toolchain-raspberrypi-arm.cmake
RUN chown jedi /home/jedi/release-1.8.0.tar.gz
USER jedi
RUN cd /home/jedi && tar zxvf release-1.8.0.tar.gz
RUN cd /home/jedi/googletest-release-1.8.0 && \
	cmake -DCMAKE_TOOLCHAIN_FILE=/home/jedi/Toolchain-raspberrypi-arm.cmake && \
# To install the library in the toolchain sysroot.
# Look at the toolchain comfiguration file (), arm-bcm2708hardfp-linux-gnueabi-gcc
# is the configured compiler.
	make DESTDIR=`/home/jedi/rpi-toolchain/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi-gcc --print-sysroot` install
# So CMake can find gtest when using the toolchain
RUN echo '' | cat >> /home/jedi/Toolchain-raspberrypi-arm.cmake
RUN echo 'SET (ENV{GTEST_ROOT} $ENV{PI_TOOLS_HOME}/${PiToolsDir}/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr/local/)' | cat >> /home/jedi/Toolchain-raspberrypi-arm.cmake

#
# Arduino IDE
#
USER jedi
RUN cd /tmp && \
	wget https://downloads.arduino.cc/arduino-1.6.8-linux64.tar.xz && \
	tar xf arduino-1.6.8-linux64.tar.xz && \
	mv arduino-1.6.8 /home/jedi && \
	echo "export PATH=$PATH:$HOME/arduino-1.6.8" >> /home/jedi/.bashrc

## # For nfs
## RUN apt-get install -y nfs-kernel-server
## RUN apt-get install -y module-init-tools
## # Create root nfs folder
## RUN mkdir -p /nfs/rpi
## # Import the nfs configuration file
## RUN echo "/nfs/rpi 192.168.1.0/24(rw,fsid=1,sync,no_root_squash,no_subtree_check)" >> /etc/exports
## # Port used by nfs
## EXPOSE 111/udp 2049/tcp
## # Copy necessary files
## COPY nfs-start.sh /home/jedi/nfs-start.sh
## RUN chmod +x /home/jedi/nfs-start.sh
## # Initialize the NFS server
## ENTRYPOINT ["/home/jedi/nfs-start.sh"]

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
  ln -s /home/jedi/dotfiles/sh /home/jedi/sh

# Set prompt with image name
RUN echo 'export PS1="`echo $PS1 | sed s/@.h/@rpi-build/g` "' >> /home/jedi/.profile
