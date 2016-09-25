## How to build the library

To build the library for raspberry, follow the instruction from
(here)[../../rpi/cmakets/README.md]

To build the library for arduino (or any Atmel MCU), include the files in the
arduino IDE. See arduinot IDE's documentation (here)[https://www.arduino.cc/en/Guide/HomePage].

# Compilation for raspberry

```
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/domos/rpi/cmaketc/Toolchain-raspberrypi-arm.cmake
make
```

# Compilation for the Desktop

define GTEST_ROOT first (see CMakeLists.txt). Then:

```
cmake CMakeLists.txt
make
```

For example, to compile a Debug version and put the CMake files in a specific folder, do:
```
mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../
make
```

