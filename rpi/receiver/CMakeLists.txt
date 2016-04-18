cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
project(domos)

set(LIB_PATH ../../lib/)
set(EXTERNAL_LIB_PATH ../../deps/)
set(RPI_EXTERNAL_LIB_PATH ../deps/)

add_subdirectory(${RPI_EXTERNAL_LIB_PATH}/WiringPi
                 ${CMAKE_CURRENT_BINARY_DIR}/wiringPi)

add_subdirectory(${LIB_PATH}/rfprotocol
                 ${CMAKE_CURRENT_BINARY_DIR}/rfprotocol)

# For WiringPi
find_package (Threads)

# For RCSwitch
add_definitions(-DRPI)

#Includes
include_directories(${RPI_EXTERNAL_LIB_PATH}/WiringPi/WiringPi/wiringPi ${EXTERNAL_LIB_PATH}/rc-switch)

# Libraries
#add_library(wiringPi STATIC IMPORTED)
set_property(TARGET wiringPi PROPERTY
             IMPORTED_LOCATION ${RPI_EXTERNAL_LIB_PATH}/WiringPi/WiringPi/wiringPi/libwiringPi.a)

set(SOURCES ../src/main.cpp ${EXTERNAL_LIB_PATH}/rc-switch/RCSwitch.cpp)
#-----------------------------------------------------------------------
# Setup executable
#-----------------------------------------------------------------------

set(EXE_NAME receiver433)
add_executable(${EXE_NAME} ${SOURCES})
target_link_libraries(${EXE_NAME} wiringPi ${CMAKE_THREAD_LIBS_INIT})