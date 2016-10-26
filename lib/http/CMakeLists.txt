cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(http)

#-----------------------------------------------------------------------
# Setup Library
#-----------------------------------------------------------------------
add_library(http STATIC
  http.c
)
