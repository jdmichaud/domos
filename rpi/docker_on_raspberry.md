To install docker on raspbian:
```
curl -sSL http://downloads.hypriot.com/docker-hypriot_1.8.1-1_armhf.deb >/tmp/docker-hypriot_1.8.1-1_armhf.deb
sudo dpkg -i /tmp/docker-hypriot_1.8.1-1_armhf.deb
rm -f /tmp/docker-hypriot_1.8.1-1_armhf.deb
sudo sh -c 'usermod -aG docker $SUDO_USER'
sudo systemctl enable docker.service
```
