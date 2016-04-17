cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
project(rfprotocol)

#Includes
include_directories(./)

#-----------------------------------------------------------------------
# Setup Library
#-----------------------------------------------------------------------
add_library(rfprotocol STATIC
  bitop.c
  protocol.c
)
