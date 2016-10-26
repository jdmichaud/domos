The RPi GPIO pinout (http://pi4j.com/pins/model-b-plus.html)

```
Wiring Pi        Name    No.       No. Name             WiringPi
          3.3 V Power    1   □  o  2   5.0V Power
 8        GPIO 8 SDA1    3   o  o  4   5.0V Power
 9        GPIO 9 SCL1    5   o  o  6   GND
 7        GPIO 7 GPCLK0  7   o  o  8   GPIO 15 TX>      15
          GND            9   o  o  10  GPIO 16 RX<      16
 0        GPIO 0         11  o  o  12  GPIO 1 ????      1
 2        GPIO 2         13  o  o  14  GND
 3        GPIO 3         15  o  o  16  GPIO 4           4
          3.3 V Power    17  o  o  18  GPIO 5           5
 12       GPIO 12 MOSI   19  o  o  20  GND
 13       GPIO 13 MISO   21  o  o  22  GPIO 6           6
 14       GPIO 14 SCLK   23  o  o  24  GPIO 10 CE0      10
          GND            25  o  o  26  GPIO 11 CE1      11
 30       SDA 0          27  o  o  28  SCL 0            31
 21       GPIO 21 GPCLK1 29  o  o  30  GND
 22       GPIO 22 GPCLK2 31  o  o  32  GPIO 26 PWM0     26
 23       GPIO 23 PWM1   33  o  o  34  GND
 24       GPIO 24 ????   35  o  o  36  GPIO 27          27
 25       GPIO 25        37  o  o  38  GPIO 28 PCM_DIN  28
          GND            39  o  o  40  GPIO 29 PCM_DOUT 29
```