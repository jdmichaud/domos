# vim:set ft=dockerfile:
FROM ubuntu:xenial

MAINTAINER jdmichaud

# Set the application name
ENV APPLICATION builroot

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
  less \
  sudo

# From https://buildroot.org/downloads/manual/manual.html#requirement-mandatory
RUN apt install -y \
  which \
  sed \
  make \
  binutils \
  build-essential \
  gcc \
  g++ \
  bash \
  patch \
  gzip \
  bzip2 \
  perl \
  tar \
  cpio \
  python \
  unzip \
  rsync \
  file \
  bc \
  wget

# From https://buildroot.org/downloads/manual/manual.html#requirement-optional
RUN apt install -y \
  ncurses5 \
  bazaar \
  cvs \
  git \
  mercurial \
  rsync \
  scp \
  subversion \
  javac \
  jar \
  asciidoc \
  graphviz \
  python-matplotlib

USER jedi

# Download buildroot package
RUN cd /home/jedi && \
  curl -sLO https://buildroot.org/downloads/buildroot-2017.02.tar.gz && \
  tar zxf buildroot-2017.02.tar.gz

