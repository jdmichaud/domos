cmake_minimum_required(VERSION 2.8.12.2)
project(rf_protocol_test)

################################
# GTest
################################
enable_testing()
include_directories(/usr/src/gmock/gtest/include/
                    ../../src/)

################################
# Unit Tests
################################
# Add test cpp file
set(SOURCES ../main.cc)
add_executable(rf_protocol_test ${SOURCES})
# Link test executable against gtest & gtest_main
target_link_libraries(rf_protocol_test gtest)
add_test(rf_protocol_test, rf_protocol_test)
