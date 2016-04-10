# Build with
# docker build -t buildroot .

# Run with
# user$ docker run -it --rm --privileged --cap-add=ALL -v /lib/modules:/lib/modules buildroot
# `--rm --privileged --cap-add=ALL` is to modprobe nfs and nfsd

FROM ubuntu

# For the proxy
#ENV http_proxy http://http-proxy-1.em.health.ge.com:9400
#ENV https_proxy http://http-proxy-1.em.health.ge.com:9400
#ENV no_proxy localhost,127.0.0.1,ge.com

# Create user
RUN useradd jedi --create-home --password jedi

RUN apt-get update
# Some utilities
RUN apt-get install -y build-essential vim unzip curl wget git lsb-release minicom

# For nfs
RUN apt-get install -y nfs-kernel-server
RUN apt-get install -y module-init-tools
# Create root nfs folder
RUN mkdir -p /nfs/rpi
# Import the nfs configuration file
RUN echo "/nfs/rpi 192.168.1.0/24(rw,fsid=1,sync,no_root_squash,no_subtree_check)" >> /etc/exports
# Port used by nfs
EXPOSE 111/udp 2049/tcp

# For buildroot
RUN apt-get install -y libncurses-dev
RUN apt-get install -y python python-dev python-setuptools

# SSH server
#RUN apt-get install -y openssh-server

# Download buildroot
USER jedi
RUN git clone https://github.com/buildroot/buildroot.git /home/jedi/buildroot

# Copy necessary files
COPY nfs-start.sh /home/jedi/nfs-start.sh

# Login as root by default
USER root
RUN chmod +x /home/jedi/nfs-start.sh

# Initialize the NFS server
ENTRYPOINT ["/home/jedi/nfs-start.sh"]

