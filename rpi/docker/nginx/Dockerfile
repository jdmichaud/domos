# Image used to run nginx on the rpi controller
# Build:
# docker build -t domos-web-server .
# Run:
# docker run --rm -it --net=host -v /path/to/nginx/store:/tmp/nginx domos-web-server
# Debug:
# docker run --rm -it --net=host -v /path/to/nginx/store:/tmp/nginx domos-web-server bash --login


#FROM sdhibit/rpi-raspbian
FROM ubuntu
MAINTAINER novidee
ENV APPLICATION domos-web-server

# explicitly set user/group IDs
RUN useradd jedi --create-home --password jedi && echo "jedi:jedi" | chpasswd && adduser jedi sudo

#
# Install some packages
#
USER root
RUN apt-get update
# Some utilities
RUN apt-get install -y build-essential \
  nano \
  vim \
  unzip \
  curl \
  git \
  sudo \
  nginx

# Certificate generation
RUN mkdir -p /etc/ssl/certs && \
  mkdir -p /etc/ssl/private && \
  chown root:root /etc/ssl/private && \
  chmod 700 /etc/ssl/private && \
  cd /etc/ssl/ && \
  openssl req -x509 -sha256 -nodes -days 365 -newkey rsa:2048 -keyout server-private.key -out server-certificate.csr -subj "/C=/ST=/L=/O=/OU=/CN=." && \
  chmod 700 server-private.key && \
  mv server-private.key /etc/ssl/private/ && \
  mv server-certificate.csr /etc/ssl/certs/

# Configure nginx
ADD nginx.conf /etc/nginx/
EXPOSE 443

# Create a user friendly environment
USER jedi
# Git configuration
RUN git config --global user.email "jean.daniel.michaud@gmail.com" && \
  git config --global user.name "JD"
# Force color prompt
RUN sed -i 's/#force_color_prompt=yes/force_color_prompt=yes/g' /home/jedi/.bashrc
# Set prompt with image name
RUN echo 'export PS1="`echo $PS1 | sed s/@.h/@domos-nginx/g` "' >> /home/jedi/.profile

USER root
CMD mkdir -p /tmp/nginx/log && service nginx start
