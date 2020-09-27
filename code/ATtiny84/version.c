/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    version.c
 * @brief   Software information version build into .elf file
 * @details This compiles software version information and fuse settings into the binary.
 *
 */
 
#include "version.h"
 
/* provide copyright message in FLASH: */
static const char Copyright[] __attribute__((used,progmem))  = COPYRIGHTSTR;
