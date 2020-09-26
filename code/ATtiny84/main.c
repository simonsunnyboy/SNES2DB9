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

#include <avr/io.h>
#include <avr/interrupt.h>

#include "snes2db9.h"
#include "attiny84-gpio.h"

#define NR_200US_TICKS_PER_MS (5)          /**< number of 200µs ticks per ms */
#define DB9_UPDATE_TASK_CYCLE_IN_MS (16)   /**< number of ms for update of DB9 state */
#define NR_200US_TICKS_DB9_UPDATE_TASK (NR_200US_TICKS_PER_MS * DB9_UPDATE_TASK_CYCLE_IN_MS)  /** number of 200µs ticks until DB9 update is triggered */

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

static void InitTimer0(void)
{
  cli();
  // Clear registers
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;

  // 5000 Hz (1000000/((24+1)*8))
  OCR0A = 24;
  // CTC
  TCCR0A |= (1 << WGM01);
  // Prescaler 8
  TCCR0B |= (1 << CS01);
  // Output Compare Match A Interrupt Enable
  TIMSK0 |= (1 << OCIE0A);
  sei();
}

ISR(TIM0_COMPA_vect)
{
    static uint16_t ticks_to_db9_update = 0;
    TaskReadiness.reader_update_ready++;

    SET_LATCH;

    ticks_to_db9_update ++;

    if( ticks_to_db9_update >= NR_200US_TICKS_DB9_UPDATE_TASK)
    {
        ticks_to_db9_update = 0;
        TaskReadiness.db9_update_ready ++;
    }
}

int main ( void )
{
    InitPorts();
    InitTimer0();

    uint8_t st = 0;


	for ( ;; )
	{
		if ( TaskReadiness.reader_update_ready )
        {
			CLEAR_LATCH;
			TaskReadiness.reader_update_ready = 0;
		}

		if ( TaskReadiness.db9_update_ready )                                    
		{
			switch(st)
			{
			case 0:
				CLEAR_DB9UP;
				SET_DB9FIRE;
				st = 1;
				break;
			case 1:
				SET_DB9UP;
				CLEAR_DB9DOWN;
				st = 2;
				break;
			case 2:
				SET_DB9DOWN;
				CLEAR_DB9LEFT;
				st = 3;
				break;
			case 3:
				SET_DB9LEFT;
				CLEAR_DB9RIGHT;
				st = 4;
				break;
			case 4:
				SET_DB9RIGHT;
				CLEAR_DB9FIRE;
				st = 0;
				break;
			default:
				st = 0;
				break;
			};

			TaskReadiness.db9_update_ready = 0;
		}
	}

	return 0;
}
