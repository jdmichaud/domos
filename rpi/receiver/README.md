# Plug the receiver

To plug the 433Mhx receiver to the raspberry, look into this [schematic](https://github.com/jdmichaud/snippets/blob/master/raspberry/simple_RF_receiver/simple_RF_receiver.fzz).

# Compilation

## Dependencies

execute:
```
./prepare_dependencies.sh
```
To download necessary libraries and the raspberry toolchain for cross-compilation.

## Create the Makefile

Follow instructions in cmake's [README](cmake/README.md) file.

## Compile

```
make
```
