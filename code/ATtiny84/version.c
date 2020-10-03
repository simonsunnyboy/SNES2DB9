/**
 * SNES to DB9 Joystick converter (ATtiny84 implementation)
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
 
/** 
 * @addtogroup SNES2DB_ATtiny84 
 * @{
 */ 
 
#include "version.h"
#include <avr/io.h>

/**
 *  @brief add fuse information to ELF file
 *  @details The fuse information can be extracted with $ avr-objdump -s -j .fuse ELF-file
 */
FUSES = 
{
    .low = 0x62,
    .high = 0xdf,
    .extended = 0xff,
};
 
/**
 * @brief provide copyright message in FLASH
 */
static const char Copyright[] __attribute__((used,progmem))  = COPYRIGHTSTR;

/** @} */
