cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(http)

find_package(CURL)

#Includes
include_directories(${CURL_INCLUDE_DIRS})

add_definitions(-std=c99)

#-----------------------------------------------------------------------
# Setup Library
#-----------------------------------------------------------------------
add_library(http STATIC
  http.c
)

set_target_properties(http PROPERTIES PUBLIC_HEADER "http.h")

install(TARGETS http
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include)

