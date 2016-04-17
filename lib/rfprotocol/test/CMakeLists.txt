cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
project(rfprotocol)

# for gtest
find_package (Threads)

#Includes
include_directories(/usr/src/gmock/gtest/include/)
include_directories(../src)
include_directories(./)

#-----------------------------------------------------------------------
# Setup Library
#-----------------------------------------------------------------------
add_executable(testrfprotocol main.cc)
target_link_libraries(testrfprotocol rfprotocol gtest gmock ${CMAKE_THREAD_LIBS_INIT})