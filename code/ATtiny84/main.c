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

static TaskFlags TaskReadiness = { 0, 0 };  /**< readiness state of tasks */

/**
 * @brief initialize TIMER0 of ATTiny84 to ~200µs ticks with internal oscillator
 */
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

/**
 * @brief   interrupt service routine to process 200µs updates
 * @details Tasks are scheduled for execution from the main loop via the task readiness flags.
 *          Flags are primed when the associated task is due.
 */
ISR(TIM0_COMPA_vect)
{
    static uint16_t ticks_to_db9_update = 0;
    TaskReadiness.reader_update_ready++;

    ticks_to_db9_update ++;

    if( ticks_to_db9_update >= NR_200US_TICKS_DB9_UPDATE_TASK)
    {
        ticks_to_db9_update = 0;
        TaskReadiness.db9_update_ready ++;
    }
}

/**
 * @brief   updates the SNES gamepad state
 * @details The gamepad state is processed by the DB9UpdateTask()
 */
static void ReaderTask( void )
{

}

/**
 * @brief   updates the DB9 joystick state from SNES game pad state
 * @details The SNES reading cycle is restarted from this task.
 */
static void DB9UpdateTask( void )
{

}

/**
 * @brief main routine
 * @details The necessary peripherals are updated and scheduled tasks are dispatched from the endless main loop.
 */
int main ( void )
{
    InitPorts();
    InitTimer0();

	for ( ;; )
	{
		if ( TaskReadiness.reader_update_ready )
        {
			ReaderTask();
			TaskReadiness.reader_update_ready = 0;
		}

		if ( TaskReadiness.db9_update_ready )                                    
		{
			DB9UpdateTask();
			TaskReadiness.db9_update_ready = 0;
		}
	}

	return 0;
}
