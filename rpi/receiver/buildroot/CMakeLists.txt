cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(receiver)

# Packages
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)

# External dependencies (but still build by us...)
find_library(WIRINGPI_LIBRARY NAMES libwiringPi.a wiringPi)
find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)
find_library(RCSWITCH_LIBRARY NAMES librc-switch.a rc-switch)
find_path(RCSWITCH_INCLUDE_DIRS NAMES RCSwitch.h)

# Internal dependencies
find_library(HTTP_LIBRARY NAMES libhttp.a http)
find_path(HTTP_INCLUDE_DIRS http.h)
find_library(RFPROTOCOL_LIBRARY NAMES librfprotocol.a rfprotocol)
find_path(RFPROTOCOL_INCLUDE_DIRS NAMES protocol.h)

if (DEFINED ENV{WIRINGPI_LIBRARY-NOTFOUND})
	message(FATAL_ERROR "wiringPi library not found")
else()
	message(STATUS "wiringPi library found ${WIRINGPI_LIBRARY}")
	message(STATUS "wiringPi headers found ${WIRINGPI_INCLUDE_DIRS}")
endif()

if (DEFINED ENV{RCSWITCH_LIBRARY-NOTFOUND})
	message(FATAL_ERROR "rc-switch library not found")
else()
	message(STATUS "rc-switch library found ${RCSWITCH_LIBRARY}")
	message(STATUS "rc-switch headers found ${RCSWITCH_INCLUDE_DIRS}")
endif()

if (DEFINED ENV{HTTP_LIBRARY-NOTFOUND})
	message(FATAL_ERROR "http library not found")
else()
	message(STATUS "http library found ${HTTP_LIBRARY}")
	message(STATUS "http headers found ${HTTP_INCLUDE_DIRS}")
endif()

if (DEFINED ENV{RFPROTOCOL_LIBRARY-NOTFOUND})
	message(FATAL_ERROR "rfprotocol library not found")
else()
	message(STATUS "rfprotocol library found ${RFPROTOCOL_LIBRARY}")
	message(STATUS "rfprotocol headers found ${RFPROTOCOL_INCLUDE_DIRS}")
endif()

add_definitions(-DRPI)

include_directories(
	${CURL_INCLUDE_DIRS}
	${WIRINGPI_INCLUDE_DIRS}
	${RCSWITCH_INCLUDE_DIRS}
	${HTTP_INCLUDE_DIRS}
	${RFPROTOCOL_INCLUDE_DIRS}
)

set(EXE_NAME receiver433)
add_executable(${EXE_NAME}
	../src/main.cpp
	../src/RF33_adapter.cpp
	../src/HTTPBackend.cpp
	../src/HTTPBackend.h
	../src/Backend.h
	../src/verbose.h
)
add_definitions(-std=c++11 -Wfatal-errors)
target_link_libraries(${EXE_NAME}
	${CMAKE_THREAD_LIBS_INIT}
	${CURL_LIBRARIES}
	${WIRINGPI_LIBRARY}
	${HTTP_LIBRARY}
	${RFPROTOCOL_LIBRARY}
	${RCSWITCH_LIBRARY}
)

install(
	TARGETS ${EXE_NAME}
	RUNTIME DESTINATION bin
)
