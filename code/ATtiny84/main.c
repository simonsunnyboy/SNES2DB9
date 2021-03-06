/**
 * @mainpage
 * 
 * SNES to DB9 Joystick converter (ATtiny84 implementation)
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    main.c
 * @brief   implements the ATtiny84 implementation of the SNES2DB9 project
 * @details
 *
 */
 
/** 
 * @addtogroup SNES2DB_ATtiny84 
 * @{
 */ 

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#include "snes2db9.h"
#include "attiny84-gpio.h"

#define NR_200US_TICKS_PER_MS (5)          /**< number of 200µs ticks per ms */
#define DB9_UPDATE_TASK_CYCLE_IN_MS (16)   /**< number of ms for update of DB9 state */
#define NR_200US_TICKS_DB9_UPDATE_TASK (NR_200US_TICKS_PER_MS * DB9_UPDATE_TASK_CYCLE_IN_MS)  /**< number of 200µs ticks until DB9 update is triggered */
#define STARTUP_TIME_IN_MS (3000)          /**< startup duration in ms, SNES input is ignored during startup to avoid flickery signals */

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


static SNESReader Reader;                    /**< SNES gamepad reader instance, services the SNES CLOCK, LATCH pins and reads the DATA pin */
static SNESMapper Mapper;                    /**< SNES mapper instance, translates SNES gamepad button presses to DB9 joystick signals */
static uint16_t   SNESGamepadState;          /**< internal SNES gamepad state used by the application, bitcoded */
static uint8_t    DB9State;                  /**< internal DB9 joystick state outputed via the DB9 pins, bitcoded */
static uint16_t   startup_time_in_ms;        /**< startup time in ms, suppresses button presses during this period */


static TaskFlags TaskReadiness = { 0, 0 };   /**< readiness state of tasks */

/**
 * @brief hardware abstraction layer function to set ATtiny84 pins to a given state from SNES2DB9 core software
 * @param pin
 * @param state
 */
static void SetPin ( SNES2DB9_Pin pin, SNES2DB9_Pinstate state )
{
	static const uint8_t pin_mask[] =
	{
		LATCH_PIN,	  //SNES_LATCH
		CLOCK_PIN,	  //SNES_CLK
		DATA_PIN,     //SNES_DATA
		DB9UP_PIN,    //DB9_UP
		DB9DOWN_PIN,  //DB9_DOWN
		DB9LEFT_PIN,  //DB9_LEFT
		DB9RIGHT_PIN, //DB9_RIGHT
		DB9FIRE_PIN,  //DB9_FIRE
	};

	if ( pin <= DB9_FIRE )
	{
		if ( state == SNES2DB9_PIN_HIGHZ )
		{
			CLEAR_BIT ( DDRA, pin_mask[pin]);
		}
		else if ( state == SNES2DB9_PIN_HIGH )
		{
			SET_BIT ( DDRA, pin_mask[pin]);
			SET_BIT ( PORTA, pin_mask[pin] );
		}
		else
		{
			SET_BIT ( DDRA, pin_mask[pin]);
			CLEAR_BIT ( PORTA, pin_mask[pin] );
		}
	}
}

/**
 * @brief hardware abstraction layer function to read ATtiny84 pin state for use by SNES2DB9 core software
 * @param pin
 * @return pinstate, either LOW or HIGH
 */
static SNES2DB9_Pinstate ReadPin ( SNES2DB9_Pin pin )
{
	SNES2DB9_Pinstate pinstate = SNES2DB9_PIN_HIGH;

	if ( pin == SNES_DATA )
	{
		if ( READ_DATA == 0 )
		{
			pinstate = SNES2DB9_PIN_LOW;
		}
	}

	return pinstate;
}


/**
 * @brief initialize TIMER0 of ATTiny84 to ~200µs ticks with internal oscillator
 */
static void InitTimer0 ( void )
{
	cli();
	// Clear registers
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT0 = 0;
	// 5000 Hz (4000000/((99+1)*8))
	OCR0A = 99;
	// CTC
	TCCR0A |= ( 1 << WGM01 );
	// Prescaler 8
	TCCR0B |= ( 1 << CS01 );
	// Output Compare Match A Interrupt Enable
	TIMSK0 |= ( 1 << OCIE0A );
	sei();
}

/**
 * @brief   interrupt service routine to process 200µs updates
 * @details Tasks are scheduled for execution from the main loop via the task readiness flags.
 *          Flags are primed when the associated task is due.
 */
ISR ( TIM0_COMPA_vect )
{
	static uint16_t ticks_to_db9_update = 0;
	TaskReadiness.reader_update_ready++;
	ticks_to_db9_update ++;

	if ( ticks_to_db9_update >= NR_200US_TICKS_DB9_UPDATE_TASK )
	{
		ticks_to_db9_update = 0;
		TaskReadiness.db9_update_ready ++;
	}
}

/**
 * @brief   inits the SNES2DB9 application and the data instances
 * @details Button mapping and autofire timing are configured here.
 */
static void InitAppl ( void )
{
	SNESMapperButtonMasks button_config;
	/* initialize mapper instance */
	button_config.fire_mask = SNES_BTNMASK_B;
	button_config.jump_mask = SNES_BTNMASK_A;
	button_config.autofire_mask = SNES_BTNMASK_Y;
	SNESMapper_Init ( &Mapper, &button_config );
	SNESMapper_SetAutofireDuration ( &Mapper, DB9_UPDATE_TASK_CYCLE_IN_MS );
	/* initialize reader instance */
	SNESReader_Init ( &Reader, SetPin, ReadPin );
	SNESGamepadState = 0;
	/* initialize DB9 handler instance */
	DB9State = 0;
}

/**
 * @brief   updates the SNES gamepad state
 * @details The gamepad state is processed by the DB9UpdateTask()
 */
static void ReaderTask ( void )
{
	SNESGamepadState = SNESReader_Update ( &Reader );
}

/**
 * @brief   updates the DB9 joystick state from SNES game pad state
 * @details The SNES reading cycle is restarted from this task.
 */
static void DB9UpdateTask ( void )
{
	/* handle startup time delay to avoid DB9 flicker on plugin of device: */
	if ( startup_time_in_ms <= STARTUP_TIME_IN_MS )
	{
		startup_time_in_ms += DB9_UPDATE_TASK_CYCLE_IN_MS;
		DB9State = 0;
	}
	else
	{
		DB9State = SNESMapper_Update ( &Mapper, SNESGamepadState, DB9_UPDATE_TASK_CYCLE_IN_MS );
	}

	DB9_SetPins ( DB9State, SetPin );
	SNESReader_BeginRead ( &Reader );
}

/**
 * @brief main routine
 * @details The necessary peripherals are updated and scheduled tasks are dispatched from the endless main loop.
 */
int main ( void )
{
	// configure internal clock to 4 instead of 1Mhz by changing the prescaler
	clock_prescale_set ( clock_div_2 );
	InitPorts();
	InitAppl();
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

/** @} */

