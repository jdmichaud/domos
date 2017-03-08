# Prepare your raspberry

## Install the raspbian image on the SD card

reference: [https://www.raspberrypi.org](https://www.raspberrypi.org/documentation/installation/installing-images/linux.md)

Download the latest raspbian lite image from adafruit:
```
wget https://downloads.raspberrypi.org/raspbian_lite_latest
```

and unzip it:
```
unzip raspbian_lite_latest
```

A `.img` file is generated.

Plug the SD card reader on a USB port and insert the micro SD card in it.
You will have to identify the device name. For this, look in dmesg:
```
dmesg | less
```

At the end of the file, you should see the new USB device message. Look for something like:
```
[243234.903292] sd 16:0:0:0: [sdd] Attached SCSI removable disk
```
The card is mounted on /dev/sdd here. Identify all the partitions (they should
be detail in the dmesg logs after the aforementioned log).

Unmount all those partitions (we assume sdd as the device and partition number 1 & 2):
```
sudo umount /dev/sdd1
sudo umount /dev/sdd2
```

Copy the image on a free micro SD card.
:warning: Everything on the card will be erased!
```
dd bs=4M if=2016-03-18-raspbian-jessie.img of=/dev/sdd
```

## Partition the remaining part of the SD card

reference: [http://elinux.org](http://elinux.org/RPi_Resize_Flash_Partitions)

Open `parted`:
```
sudo parted /dev/sdd
```

Change to a more precise unit:
```
(parted) unit chs
```

Display the free space on your card:
```
(parted) print free
Model: TS-RDF5 SD Transcend (scsi)
Disk /dev/sdd: 482,18,47
Sector size (logical/physical): 512B/512B
BIOS cylinder,head,sector geometry: 482,255,63.  Each cylinder is 8225kB.
Partition Table: msdos
Disk Flags:

Number  Start      End        Type     File system  Flags
        0,1,0      0,130,1             Free Space
 1      0,130,2    8,40,31    primary  fat16        lba
 2      8,40,32    482,12,54  primary  ext4
        482,12,55  482,18,47           Free Space
```
You will resize your root partition to the end of the SD card. First remove the root partition
(here partition 2)and then extend it.

```
(parted) rm 2
(parted) mkpart primary 8,40,32 482,18,47
```

Your partition now is extended all the way to the end:
```
Number  Start    End        Type     File system  Flags
        0,1,0    0,130,1             Free Space
 1      0,130,2  8,40,31    primary  fat16        lba
 2      8,40,32  482,18,47  primary               lba
```

Clean and resize the root partition:
```
sudo e2fsck -f /dev/sdd2
sudo resize2fs /dev/sdd2
```

Remove the SD card for the reader and plug it in the raspberry.

# Configure the image

## Connect with the USB/TTL cable (Need a USB/TTL cable)

Plug the 4 PIN this way:

RED -> 5V

GREEN (TX) -> RX

WHITE (RX) -> TX

BLACK -> GND

Check the Raspberry pin out [here](http://pinout.xyz/).

Plug the USB cable to your computer.
The 5V pin is to input AND output 5V.
:warning: This will provide 5V directly to the raspberry. No fuse or voltage regulator on the 5V pin.
A less risky way is to not to plug the RED cable and use another USB cable to provide for power.
It requires an additional cable though.

Once plugged, install screen on the host:
```
sudo apt-get install -y screen
```

And connect to the USB/Serial device:
```
sudo screen /dev/ttyUSB0 115200
```

## Change root password

Once connected on the raspberry, change the root password:
```
sudo passwd
```

then change the pi password:
```
passwd
```

### Remove uncheck access to root

To remove the possibility to sudo without a password from the `pi` account:
```
sudo visudo
```

change:
```
pi ALL=(ALL) NOPASSWD: ALL
```
to
```
pi ALL=(ALL) ALL
```

## Configure the wifi (need a Wifi dongle)

To configure the Wifi, plug the dongle. Then edit `/etc/network/interfaces`. Configure wlan0 this way:
```
allow-hotplug wlan0
auto wlan0
iface wlan0 inet dhcp
  wpa-ssid "YOURSSID"
  wpa-psk "YOURPASSWD"

```

From the same file, remove manual configuration of wlan0.

Then `reboot`.

## Update your packages

```
sudo apt-get update
```

## Configure the SSH server

Enter:
```
sudo raspi-config
```
Then go to `Advanced Options` and then `SSH`.

## Install the linux headers (for sshfs)

:warning: this below download linux for the specific version of your raspberry. Don't know how to translate this into linux headers with modules.dep.bin and all...

```
FIRMWARE_VERSION=`zcat /usr/share/doc/raspberrypi-bootloader/changelog.Debian.gz | head | grep -Po -m 1 'firmware as of \K\w+?(?=$)'`
HASH=`wget --quiet https://raw.github.com/raspberrypi/firmware/FIRMWARE_VERSION/extra/git_hash -O -`
git clone https://github.com/raspberrypi/linux
```

# Install Docker


[//]: # (To install docker on raspbian - Deprecated:)
[//]: # (```)
[//]: # (curl -sSL http://downloads.hypriot.com/docker-hypriot_1.10.3-1_armhf.deb >/tmp/docker-hypriot_1.10.3-1_armhf.deb)
[//]: # (sudo dpkg -i /tmp/docker-hypriot_1.10.3-1_armhf.deb)
[//]: # (rm -f /tmp/docker-hypriot_1.10.3-1_armhf.deb)
[//]: # (```)

To install docker on raspbian:
```
curl -sSL get.docker.com | sh
```

Make sure your user had access to docker
```
sudo sh -c 'usermod -aG docker $SUDO_USER'
```

Make docker start at boot:
```
sudo systemctl enable docker.service
```

To start it for the session:
```
sudo systemctl start docker.service
```

# Docker business

There is a Docker image (`host/Dockerfile`) to cross-compile program on your
desktop for your raspberry-pi.

There is a Docker image (`rpi/docker/Dockerfile`) to mount the domos folder
from your desktop to your raspberry-pi in order to test it. Use the script
`mount.home` of the raspberry Docker container.

# Controller operations

The controller is the raspberry pi centralizing your home automation system.

Here are the main components:
* A receiver program, listening to the radio waves and posting REST request when
receiving data from sensors.
* A database with a REST API (Eve+MongiDB) to store the data
* A Node.js web server to serve the web client

