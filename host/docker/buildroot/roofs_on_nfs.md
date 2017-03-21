### Prepare the rootfs folder

As explained in the buildroot manual, don't try to use the `output/target`
folder directly, but mount an image in `output/images`

First create a loop device pointing to the filesystem image file (/dev/loop0 is
assumed to be available)
```
losetup /dev/loop0 /path/to/filesysyem/image
```
Tip: For buildroot, the filesystem is probably something like
`output/image/root_ext4`

Then mount that loop device to a local folder (create the folder first)
```
mkdir -p /path/to/roots
mount /dev/loop0 /path/to/roots
```

### Setup the NFS server

install nfs_kernel_server
```
apt install -y nfs_kernel_server
```

Ensure the proper *module* are loaded:
```
modprobe nfsd
modprobe nfs
```

Ensure the proper *service* are loaded:
```
service rpcbind start
service nfs-kernel-server start
```

Create the export file:
```
cat >> /etc/exports
/path/to/roots 192.168.0.0/24(rw,no_root_squash)
```

Make nfs server reread the config
```
exportfs -rv
```

Try to connect
```
mount -t nfs -o proto=tcp,port=2049 <server-ip>:/path/to/roots /mnt/somewhere/
```

### Modify the RPi config to mount a nfsed root filesystem.

Modify the `cmdline.txt`
```
root=/dev/nfs ip=dhcp rootwait console=tty1 console=ttyAMA0,115200 nfsroot=<server-ip>:/path/to/roots
```

### If file shall be change

After a change to the file, you'll have to unmount, un loop, reloop and remount
```
umount /path/to/roots
losetup -d /dev/loop0
losetup /dev/loop0 /path/to/filesysyem/image
mount /dev/loop0 /path/to/roots
```
