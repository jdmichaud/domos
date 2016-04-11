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
# Creating CMakeLists.txt for wiringPi cross compilation
cat > CMakeLists.txt << EOF
cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
project(wiringPi)

#Includes
include_directories(./)

#-----------------------------------------------------------------------
# Setup Library
#-----------------------------------------------------------------------
add_library(wiringPi STATIC
  wiringPi.c
  wiringSerial.c wiringShift.c
  piHiPri.c piThread.c
  wiringPiSPI.c wiringPiI2C.c
  softPwm.c softTone.c
  mcp23008.c mcp23016.c mcp23017.c
  mcp23s08.c mcp23s17.c
  sr595.c
  pcf8574.c pcf8591.c
  mcp3002.c mcp3004.c mcp4802.c mcp3422.c
  max31855.c max5322.c ads1115.c
  sn3218.c
  drcSerial.c
  wpiExtensions.c
)
EOF
# Compiling
cmake -DCMAKE_TOOLCHAIN_FILE=$HOME/embedded/domos/rpi/cmaketc/Toolchain-raspberrypi-arm.cmake
make

popd
pushd .

#RCSwitch
git clone https://github.com/sui77/rc-switch.git
cd rc-switch
git checkout 884bbd7e9fc2340d29c47f5e42b3bf809af6539b

popd
