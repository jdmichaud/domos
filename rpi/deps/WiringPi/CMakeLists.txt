cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
project(wiringPi)

set(WIRINGPI_FOLDER WiringPi/wiringPi)

#Includes
include_directories(${WIRINGPI_FOLDER})

#-----------------------------------------------------------------------
# Setup Library
#-----------------------------------------------------------------------
add_library(wiringPi STATIC
  ${WIRINGPI_FOLDER}/wiringPi.c
  ${WIRINGPI_FOLDER}/wiringSerial.c
  ${WIRINGPI_FOLDER}/wiringShift.c
  ${WIRINGPI_FOLDER}/piHiPri.c
  ${WIRINGPI_FOLDER}/piThread.c
  ${WIRINGPI_FOLDER}/wiringPiSPI.c
  ${WIRINGPI_FOLDER}/wiringPiI2C.c
  ${WIRINGPI_FOLDER}/softPwm.c
  ${WIRINGPI_FOLDER}/softTone.c
  ${WIRINGPI_FOLDER}/mcp23008.c
  ${WIRINGPI_FOLDER}/mcp23016.c
  ${WIRINGPI_FOLDER}/mcp23017.c
  ${WIRINGPI_FOLDER}/mcp23s08.c
  ${WIRINGPI_FOLDER}/mcp23s17.c
  ${WIRINGPI_FOLDER}/sr595.c
  ${WIRINGPI_FOLDER}/pcf8574.c
  ${WIRINGPI_FOLDER}/pcf8591.c
  ${WIRINGPI_FOLDER}/mcp3002.c
  ${WIRINGPI_FOLDER}/mcp3004.c
  ${WIRINGPI_FOLDER}/mcp4802.c
  ${WIRINGPI_FOLDER}/mcp3422.c
  ${WIRINGPI_FOLDER}/max31855.c
  ${WIRINGPI_FOLDER}/max5322.c
  ${WIRINGPI_FOLDER}/ads1115.c
  ${WIRINGPI_FOLDER}/sn3218.c
  ${WIRINGPI_FOLDER}/drcSerial.c
  ${WIRINGPI_FOLDER}/wpiExtensions.c
)
