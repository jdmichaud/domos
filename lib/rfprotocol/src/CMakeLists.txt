cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
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

set_target_properties(rfprotocol PROPERTIES PUBLIC_HEADER "protocol.h")

install(
	TARGETS rfprotocol
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)
