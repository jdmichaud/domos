# Plug the receiver

To plug the 433Mhx receiver to the raspberry, look into this [schematic](https://github.com/jdmichaud/snippets/blob/master/raspberry/simple_RF_receiver/simple_RF_receiver.fzz).

# Compilation

```
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/Toolchain-raspberrypi-arm.cmake
make
```

The Toolchain file is most probably at the root of your home directory. Check
the Dockerfile of your container if in doubt.