cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
project(domos)

set(DEPENDENCIES_PATH ../dependencies)

# Defining some macros for RPi cross compilation
# WARNING ! Works only in the command line. See README.md.
#set(CMAKE_TOOLCHAIN_FILE ../../cmaketc/Toolchain-raspberrypi-arm.cmake)

# For RCSwitch
add_definitions(-DRPI)

#Includes
include_directories(${DEPENDENCIES_PATH}/WiringPi/wiringPi ${DEPENDENCIES_PATH}/rc-switch)

# Libraries
add_library(wiringPi STATIC IMPORTED)
set_property(TARGET wiringPi PROPERTY
             IMPORTED_LOCATION ${DEPENDENCIES_PATH}/WiringPi/wiringPi/libwiringPi.a)

set(Sources ${DEPENDENCIES_PATH}/rc-switch/RCSwitch.cpp)
set(Sources ../src/main.cpp)
#-----------------------------------------------------------------------
# Setup executable
#-----------------------------------------------------------------------

set(EXE_NAME receiver433)
add_executable(${EXE_NAME} ${Sources})
target_link_libraries(${EXE_NAME} wiringPi)
