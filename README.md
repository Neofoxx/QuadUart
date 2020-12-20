# QuadUart
A Quad UART to USB converter, based on PIC32.

## Overview

This project uses a PIC32MX564F128H micro to implement a quad UART-USB converter. The idea is to use an open-source toolchain to create a useful project, and one can **never** have too many UART converters

At least 4x 115200bps is supported, with plans to go to 4x 1Mbps, speed of the micro permitting.

Exposed pins are TX, RX, and DTR.


## Toolchain

The entire toolchain is open source:
- MCU definitions from the [SpicaStack](https://gitlab.com/spicastack/pic32-parts-free) project
- Compiler is GCC for mipsel crosscompiling. A ready-to-go package can be found [here](https://build.opensuse.org/package/show/home:neofoxx/gcc-mipsel-elf) (Ubuntu, Fedora, OpenSuse packges currently built)
- The USB stack is [M-stack](https://github.com/signal11/m-stack)
- PCB is provided as a set of Gerber files, and as a KiCAD project.

## TODO

- Add fiducails to PCB
- Add pinout to Readme
- License