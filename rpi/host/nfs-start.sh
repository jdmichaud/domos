#!/bin/sh

set -xe

modprobe nfs
modprobe nfsd
rpcbind
service nfs-kernel-server restart
bash

