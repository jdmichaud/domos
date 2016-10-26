# Build with
# docker build -t arduino-dev .

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
	python-setuptools

# pip
RUN easy_install pip

# Install platformio
RUN pip install platformio

USER jedi

#
# Arduino IDE
#
RUN cd /tmp && \
	wget https://downloads.arduino.cc/arduino-1.6.8-linux64.tar.xz && \
	tar xf arduino-1.6.8-linux64.tar.xz && \
	mv arduino-1.6.8 /home/jedi && \
	echo "export PATH=$PATH:$HOME/arduino-1.6.8" >> /home/jedi/.bashrc

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
RUN echo 'export PS1="`echo $PS1 | sed s/@.h/@arduino-build/g` "' >> /home/jedi/.profile
