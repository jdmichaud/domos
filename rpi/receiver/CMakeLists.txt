cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
project(domos)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(LIB_PATH ../../lib/)
set(EXTERNAL_LIB_PATH ../../deps/)
set(RPI_EXTERNAL_LIB_PATH ../deps/)

add_subdirectory(${RPI_EXTERNAL_LIB_PATH}/WiringPi
                 ${CMAKE_CURRENT_BINARY_DIR}/wiringPi)
add_subdirectory(${LIB_PATH}/rfprotocol
                 ${CMAKE_CURRENT_BINARY_DIR}/rfprotocol)
add_subdirectory(${LIB_PATH}/http
                 ${CMAKE_CURRENT_BINARY_DIR}/http)

# For WiringPi
find_package (Threads)

# For RCSwitch
add_definitions(-DRPI)

find_package(CURL)

#Includes
include_directories(${CURL_INCLUDE_DIRS}
                    ${RPI_EXTERNAL_LIB_PATH}/WiringPi/wiringPi/wiringPi
                    ${EXTERNAL_LIB_PATH}/rc-switch
                    ${LIB_PATH}/rfprotocol/src
                    ${LIB_PATH}/)

# Libraries
#add_library(wiringPi STATIC IMPORTED)
set_property(TARGET wiringPi PROPERTY
             IMPORTED_LOCATION ${RPI_EXTERNAL_LIB_PATH}/WiringPi/WiringPi/wiringPi/libwiringPi.a)

set(SOURCES src/main.cpp src/RF33_adapter.cpp
            ${EXTERNAL_LIB_PATH}/rc-switch/RCSwitch.cpp
            ${LIB_PATH}/rfprotocol/src/bitop.c
            ${LIB_PATH}/rfprotocol/src/protocol.c
            ${LIB_PATH}/http/http.c
            src/HTTPBackend.cpp src/HTTPBackend.h src/Backend.h)
#-----------------------------------------------------------------------
# Setup executable
#-----------------------------------------------------------------------

add_definitions(-std=c++11)

set(EXE_NAME receiver433)
add_executable(${EXE_NAME} ${SOURCES})
add_definitions(-std=c++11 -Wfatal-errors)
target_link_libraries(${EXE_NAME} wiringPi ${CMAKE_THREAD_LIBS_INIT} ${CURL_LIBRARIES})
