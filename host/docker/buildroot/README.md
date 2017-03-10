This folder contains:
- Dockerfile: A docker container descriptor which facilitate the installation and use of buildroot
- overlays: All the folder and files to reapply on the target image produced by buildroot
- configs: the custom configs used by domos

TODO: Make the container take a buildroot configuration file.


## Configs

### raspberry2_defconfig

Contains the necessary configuration for the raspberry pi 2. Including 
`wpa_supplicant`, the realtek driver for the wifi dongle and `mdev`.

To do: To configure as AP, will need
- hostapd
- isc-dhcp-server (enable 'Show package that are provided by Busybox').


## Overlays

### Wifi client (wificlient)

This overlay is used to configure the board as a WiFi client. It will try to
connect to the provided SSID and retrieve an IP via DHCP.
Configure `interfaces` (`/etc/network/interfaces`) to not load eth0 
(speed up boot) and load wlan0 using `wpa_supplicant`. When wlan0 is being 
brought up by `ifup`, a script, stored in the `if-up.d` folder is called. 
It will check if `interfaces` is configured with an SSID. If not, it will
prompt the use for the current SSID and password and will configure
`interfaces` and `wpa_supplicant.conf` with the parameters provided.

Required package: 
- `wpa_supplicant` for the secure wifi connection
- the appropriate driver for your wifi interface
- `mdev` for executing modprobe with your driver when the interface is detected
- `bash` for the script

### AP (ap)

This overlay is used to configure the board as a wifi Access Point. It will
configure the wlan0 interface with a static address.
Two daemons are launched by inittab (to confirm that point):
- hostapd will manage the WiFi connection of cient
- dhcpd (isc-dhcp-server) will provide the dhcp services to WiFi client
