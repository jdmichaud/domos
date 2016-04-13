# Build with
# docker build -t buildroot .

## # Run with
## # user$ docker run -it --rm --privileged --cap-add=ALL -v /lib/modules:/lib/modules buildroot
## # `--rm --privileged --cap-add=ALL` is to modprobe nfs and nfsd

FROM ubuntu

# For the proxy

# Create user
RUN useradd jedi --create-home --password jedi

RUN apt-get update
# Some utilities
RUN apt-get install -y build-essential vim unzip curl wget git lsb-release minicom

# Python
RUN apt-get install -y python python-dev python-setuptools

# pip
RUN easy_install pip
# to avoid urllib warning, we need pyopenssl which needs libssl-dev + libffi-dev
RUN apt-get install -y libffi-dev libssl-dev
RUN pip install pyopenssl ndg-httpsclient pyasn1

# C++
RUN apt-get install -y clang
# CMake
RUN apt-get install -y cmake
# To use the RPi cross-compilation toolchain,
# which are composed of 32bits executables
RUN apt-get install -y lib32z1 lib32ncurses5 lib32bz2-1.0 libc6-i386 lib32stdc++6

# Install platformio
RUN pip install platformio

USER jedi
# Vim configuration
RUN git clone https://github.com/jdmichaud/dotfiles /home/jedi/
RUN ln -s /home/jedi/dotfiles/.vimrc /home/jedi/.vimrc
RUN ln -s /home/jedi/dotfiles/.vimrc.local /home/jedi/.vimrc.local
RUN ln -s /home/jedi/dotfiles/.vimrc.local.bundles /home/jedi/.vimrc.local.bundles

USER jedi
# RPi tool chain
RUN cd ~/
RUN git clone https://github.com/raspberrypi/tools.git
RUN mv tools rpi-toolchain
RUN cd rpi-toolchain
RUN git checkout b65595ffb74e5853ba61916f49bdbccfc54f1300
RUN echo "export PI_TOOLS_HOME=$HOME/rpi-toolchain" >> ~/.bashrc

# Arduino IDE
RUN cd /tmp
RUN wget https://downloads.arduino.cc/arduino-1.6.8-linux64.tar.xz
RUN tar xf arduino-1.6.8-linux64.tar.xz
RUN mv arduino-1.6.8 ~/
RUN echo "export PATH=$PATH:$HOME/arduino-1.6.8" >> ~/.bashrc

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

