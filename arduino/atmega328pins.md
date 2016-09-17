The arduino/atmega328 pinout (http://www.chicoree.fr/w/Arduino_sur_ATmega328P)

```
arduino equiv.            atmega328 pinout                      arduino equiv.
                               ------
        reset       PC6   1   | o    |  15    PC5               analog input 5
digital pin 0 (RX>) PD0   2   |      |  16    PC4               analog input 4
digital pin 1 (<TX) PD1   3   |      |  17    PC3               analog input 3
digital pin 2       PD2   4   |      |  18    PC2               analog input 2
digital pin 3 PMW   PD3   5   |      |  19    PC1               analog input 1
digital pin 4       PD4   6   |      |  20    PC0               analog input 0
          VCC       VCC   7   |      |  21    GND               GND
          GND       GND   8   |      |  22    AREF              analog reference
      crystal       PB6   9   |      |  23    AVCC              AVCC
      crystal       PB7   10  |      |  24    PB5 |  SCK  |     digital pin 13
digital pin 5 PMW   PD5   11  |      |  25    PB4 | MISO> |     digital pin 12
digital pin 6 PMW   PD6   12  |      |  26    PB3 | <MOSI | PMW digital pin 11
digital pin 7 PMW   PD7   13  |      |  27    PB2 |       | PMW digital pin 10
digital pin 8       PB0   14  |      |  28    PB1 |   ^   | PMW digital pin 9
                               ------                 |
                                                   When using
                                                   programmer
```