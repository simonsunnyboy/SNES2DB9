# SNES2DB9

(c) 2020 by Matthias Arndt <marndt@asmsoftware.de>

The MIT License applies. See LICENSE for details.


## Abstract

SNES2DB9 is a software package to allow implementation of a converter
to connect a SNES style gamepad with shift register to a system using
the Atari DB9 joystick standard (Atari 2600, C64, Amiga, Atari ST, etc.)


## Features

- reusable software control, independant of microcontroller and hardware
- configurable button mapping for fire, autofire and jump mapping
- sample implementation with Arduino Nano
- sample implementation with ATtiny84 microcontroller
- full user requirement and system requirement specifications

## System components

- spec contains user requirement and system requirement specifications
  including basic schematics
- code/common contains the reusable software core written in C99
- code/Arduino contains test programs and sample implementation
  as Arduino sketches
- code/ATtiny84 contains sample implementation with ATtiny84 controller
- doc contains infrastructure and output for Doxygen
- unittest contains a PC based unittest for the reusable software core

## ATtiny84 implementation

The ATtiny84 implementation can be build via Cmake. The GNU toolchain
for AVR must be installed.

This provides a full implementation of the user requirements.

The controller is suppossed to be powered by the DB9 connector +5V 
supply.

### Pin mappings

The implementation was build with perforated board.
Pin connections reflect the layout of this particular prototype.

### Pullup and pulldown resistor configuration

- SNES data and clock pins use the internal pullup resistor of the AVR.
- SNES latch is idle low and this connected with an external pulldown
  resistor of 10K to GND

## Arduino sketches

The Arduino sketches have been used on Arduino Nano.
Pin configurations shall be adapted if necessary.

The used parts of the reusable software core and symlinked into the 
sketch directories to share the main source.

If your development platform does not support symlinks, you may need
to copy over the corresponding files into the sketch directories.

### Power supply

All Arduino sketches work when powered from the DB9 +5V pin.
Connect it to the corresponding line on the Arduino.

### Sketch ''DB9JoystickSimu''

Sketch to test giving Atari style joystick inputs to a target system.
Active directions or the firebutton are pulled to low.

### Sketch ''SNESReader''

Sketch to test and debug the SNESReader implementation with a slow 
polling cycle on real hardware.

Nice to observe the serial read process with a logic analyzer.

### Sketch ''SNES2DB9Prototype''

Sketch with early prototype implementation displaying SNES gamepad 
button states via the Arduino serial port.

The polling cycle is slow so buttons have to be pressed for a few
seconds to see a reaction.

### Sketch ''SNES2DB9''

This sketch implements the basic user requirements and may be used as
a starting point for your own hardware design.

The implementation is functional as an Atari style joystick.

## Unittest

The unittest can be build with CMake on any PC. Support for code
coverage reporting via gcov can be enabled.

Each class of the reusable software core is tested through its own
test driver. A HTML test report is generated on stdout.
