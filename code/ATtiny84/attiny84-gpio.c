/**
 * Generated port pin initialization
 * Portpin mapper for AVRs
 * @file attiny84-gpio.c
 * @brief Pin mapping for SNES2DB9 implementation on ATtiny84
 * @author Matthias Arndt <marndt@asmsoftware.de>
 * @see http://www.final-memory.org/?p=2687
 */
/* User preprocessor directives: */

/** 
 * @addtogroup SNES2DB_ATtiny84_GPIO
 * @{
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "attiny84-gpio.h"

/**
 * @brief initialize port pins to defined states
 * @details Initial states:
 * - Signal DATA: PB2 as INPUT    SNES gamepad serial data input
 * - Signal CLOCK: PA7 as HIGH    SNES gamepad clock signal
 * - Signal LATCH: PA6 as LOW    SNES gamepad latch signal
 * - Signal DB9UP: PA5 as HIGH    Atari joystick up
 * - Signal DB9DOWN: PA4 as HIGH    Atari joystick down
 * - Signal DB9LEFT: PA3 as HIGH    Atari joystick left
 * - Signal DB9RIGHT: PA2 as HIGH    Atari joystick right
 * - Signal DB9FIRE: PA1 as HIGH    Atari joystick fire
 * - Signal UNUSED_A0: PA0 as HIGHZ    unused pin
 * - Signal UNUSED_B0: PB0 as HIGHZ    unused pin
 * - Signal UNUSED_B1: PB1 as HIGHZ    unused pin
 * - Signal UNUSED_B3: PB3 as HIGHZ    unused pin

 */
void InitPorts ( void )
{
	/* port inits: */
	PORTA = INIT_PORTA;
	DDRA = INIT_DDRA;
	PORTB = INIT_PORTB;
	DDRB = INIT_DDRB;
	return;
}

/** @} */
