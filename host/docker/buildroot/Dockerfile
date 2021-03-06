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
RUN apt-get install -y \
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
 RUN apt-get install -y \
   libncurses5-dev \
   bzr \
   cvs \
   git \
   mercurial \
   subversion \
   graphviz \
   python-matplotlib

# Necessary to compile qtwebengine
# See here: https://bugreports.qt.io/browse/QTBUG-61873
RUN apt-get install -y \
  g++-multilib

COPY configs /home/jedi/configs
COPY overlays /home/jedi/overlays
# Give ownership of ADDED files to the user. ADD always create the user as owned by root...
USER root
RUN chown jedi /home/jedi/configs
RUN chown -R jedi /home/jedi/overlays
USER jedi

# Download buildroot package
RUN cd /home/jedi && \
  curl -sLO https://buildroot.org/downloads/buildroot-2017.08.tar.gz && \
  tar zxf buildroot-2017.02.tar.gz
# Next: use prepare_image to setup the proper patches

