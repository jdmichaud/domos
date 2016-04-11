#!/bin/bash

# set verbose
set -x

# stop at first issue
set -e

mkdir dependencies
cd dependencies
pushd .

# WiringPi
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
git checkout 9a8f8bee5df60061645918231110a7c2e4d3fa6b
cd wiringPi
make static

popd
pushd .

#RCSwitch
git clone https://github.com/sui77/rc-switch.git
cd rc-switch
git checkout 884bbd7e9fc2340d29c47f5e42b3bf809af6539b

popd
