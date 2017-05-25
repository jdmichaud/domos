cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(rfprotocol)

# Define GTEST_ROOT to the root of your gtest static library
if (NOT DEFINED ENV{GTEST_ROOT})
  message(WARNING ""
    "To run tests, the GTEST_ROOT environment variable shall be defined"
    "i.e.: export GTEST_ROOT=/usr/src/gmock/gtest/")
  message(WARNING "Tests will not be run")
else()
  # for gtest
  enable_testing()
  find_package(GTest REQUIRED)
  find_package(Threads REQUIRED)

  #Includes
  include_directories(${GTEST_INCLUDE_DIRS})
  include_directories(../src)
  include_directories(./)

  #-----------------------------------------------------------------------
  # Setup Library
  #-----------------------------------------------------------------------
  add_executable(testrfprotocol main.cc)
  target_link_libraries(testrfprotocol rfprotocol ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()
