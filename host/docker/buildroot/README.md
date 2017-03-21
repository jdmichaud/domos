This folder contains:
- Dockerfile: A docker container descriptor which facilitate the installation and use of buildroot
- overlays: All the folder and files to reapply on the target image produced by buildroot
- configs: the custom configs used by domos
- boards: Contains the post-build or post-image scripts for each boards
- package: Contains some specific package configuration or patch

TODO: Make the container take a buildroot configuration file.

## Docker

The Docker is used to build images for raspberry using:
 - A stable buildroot (TODO: as of now buildroot master branch instead of stable
   branch for raspberry pi zero W)
 - Apply overlays, patch and configs from this buildroot directory (see the
   buildroot manual) mounted as a volume in the container as it is started
 - Export the target filesystem to a particular folder to be used as a nfs
   mounting point for the rapsberry board to ease up development and testing

To launch the container, you have to :
0. Run the container as interactive
1. Specify a volume in the container pointing to this directory, in order for the
   buildroot process to have access to the overlays, configs, etc... The buildroot
   configs expect this buildroot folder to be at `../` from the checked out folder
2. Specify an output folder, which will allow the host to provide it as an NFS
   folder to the raspberry board (TODO: embed and NFS in the container to avoid
   this step)
3. Start a bash login shell

```
docker run -it \
  -v /path/to/this/buildroot:/home/jedi/buildroot \
  -v /tmp/rpi_rootfs:/home/jedi/output \
  buildroot bash --login
```

Once in the container, to build an image, go in the `buildroot-20XX.XX` folder,
and copy the config file from the buildroot imported volume and checkout that config:
```
cd /home/jedi/buildroot-20XX.XX
cp ../buildroot/configs/raspberryX_defconfig
make O=/home/jedi/output raspberryX_defconfig
```

The `raspberryX_defconfig` config is now ready to compile. Go to the output
directory and launch the compilation

```
cd ../output
make
```

Go take a tea.

The patch and config from `../package` will be applied, once the compile is done,
the overlays from `../overlays` will be applied, the post-build script from
`../boards` will execute.

In `/home/jedi/output/images/` an `sdimage.img` will be created.

dd it on a flash card:

```
sudo dd bs=4M if=sdcard.img of=/dev/sdX
```

Stick in in the RPi and go!

You can also boot the raspberry [from nfs](roofs_on_nfs.md).

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
dhcpd (isc-dhcp-server) will provide the dhcp services to WiFi client and
is launched by inittab (to confirm that point):

hostapd provide the AP part and is launched by `ifup` (`/etd/network/interfaces`)

This overlay can also configure the board as client.

It also contains a couple of scripts for easy configuration. These script will
ask the user the mode (client or AP) and then the SSID and password.
Then they will use the templates in `/etc/ezwifi` and reconfigure the various
configuration file of:
 - ifup/ifdown to condigure the interface
 - hostapd/dhcpd for the AP configuration
 - wpa_supplicant for the client configuration

`wifi_configure` is the tool to launch to orchestrate those script. Simply launch:
```
wifi_configure
```

On first login, `wifi_configure` will be called.
