# Compile and load

Arduino code is compiled and load using:
* platformio: a builder tool for embedded system
* the pololu programmer

1. First, plug the pololu programmer to your board (see Table of Contents for  Practical AVR Microcontrollers: Games, Gadgets, and Home Automation with the Microcontroller Used in Arduino, chapter 2).
2. Then, plug it using a mini-USB to USB cable to your computer.
3. Then power the board with 5v (using a battery or a USB-to-TTL circuit)
4. Identify the port on which the pololu driver is plugged ([reference](https://www.pololu.com/docs/0J36/5.b)):
  a. on OS X, type
  ``` shell
  ls /dev/cu.usb*
  ```
  THe lower number indicates the port on which the programmer is connected
  b. on Linux: TODO
  c. On both platform you can also use:
  ``` shell
  platformio device list
  ```
5. Configure the PLATFORMIO_UPLOAD_PORT environment variable:
``` shell
export PLATFORMIO_UPLOAD_PORT=/dev/cu....
```
Note that platformio can also automatically detect the port, but don't rely on it.
6. In the project folder (i.e. in magsensor folder) Upload the program:
``` shell
make pololu
```

Bonus: To get the status of the programmer, run `make pololu`, and copy-paste
the avrdude command without the `-U firmware` option.

# Serial communication

When pluging a USB-to-TTL serial line to the Atmega328 (with Atmega RX to
USB-to-TTL RX and Atmega TX to USB-to-TTL RX), connect to the serial port:
1. On OS X, using:
``` shell
screen /dev/????? 9600
```
speed (9600) can vary depending on the atmega configuration.
