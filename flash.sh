#! /bin/bash
# arm-none-eabi-objcopy -O binary -S build/throw_test.elf build/throw_test.bin
CubeProgrammer -c port=JLINK -w build/throw_test.bin 0x8000000 -c port=JLINK reset=SWrst
