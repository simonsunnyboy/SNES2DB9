/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    main.c
 * @brief   implements the Attiny84 implementation of the SNES2DB9 project
 * @details
 *
 */

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <avr/timer.h>

#include "snes2db9.h"

#define NR_200US_TICKS_PER_MS (5)          /**< number of 200µs ticks per ms */
#define DB9_UPDATE_TASK_CYCLE_IN_MS (16)   /**< number of ms for update of DB9 state */

/**
 * @brief   readiness flags for timed tasks
 * @details Flags do
 *          - count forward from associated timer tick for given timing
 *          - reset to 0 on execution of associated task from main loop
 */
typedef struct
{
	volatile uint8_t reader_update_ready;    /**< reader updates occur with 200µs timer increments */
	volatile uint8_t db9_update_ready;       /**< DB9 state update occurs with given interval in ms derived from 200µs timer */
} TaskFlags;

static TaskFlags TaskReadiness;  /**< readiness state of tasks */

int main ( void )
{
	for ( ;; )
	{
		if ( TaskReadiness.reader_update_ready )
		{
			TaskReadiness.reader_update_ready = 0;
		}

		if ( TaskReadiness.db9_update_ready )
		{
			TaskReadiness.db9_update_ready = 0;
		}
	}

	return 0;
}
