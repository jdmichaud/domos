cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(receiver)

# Packages
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)

# External dependencies (but still build by us...)
find_library(WIRINGPI_LIBRARIY NAMES wiringPi)
find_path(WIRINGPI_INCLUDE_DIR NAMES wiringPi.h)
find_library(RCSWITCH_LIBRARY NAMES rc-switch)
find_path(RCSWITCH_INCLUDE_DIR NAMES RCSwitch.h)

# Internal dependencies
find_library(HTTP_LIBRARY NAMES http)
find_path(HTTP_INCLUDE_DIR http.h)
find_library(RFPROTOCOL_LIBRARY NAMES rfprotocol)
find_path(RFPROTOCOL_INCLUDE_DIR NAMES protocol.h)

include_directories(
	${CURL_INCLUDE_DIRS}
	${WIRINGPI_INCLUDE_DIR}
	${RCSWITCH_INCLUDE_DIR}
	${HTTP_INCLUDE_DIR}
	${RFPROTOCOL_INCLUDE_DIR}
)

set(EXE_NAME receiver433)
add_executable(${EXEC_NAME}
	../src/main.cpp
	../src/RF33_adapter.cpp
	../src/HTTPBackend.cpp
	../src/HTTPBackend.h
	../src/Backend.h
)
add_definitions(-std=c++11 -Wfatal-errors)
target_link_libraries(${EXE_NAME} 
	${CMAKE_THREAD_LIBS_INIT} 
	${CURL_LIBRARIES}
	wiringPi
	http
	rfprotocol
	rc-switch	
)
