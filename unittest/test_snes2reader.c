/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    test_snes2db9_reader.c
 * @brief   unittest implementation for SNESReader
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "snes2db9.h"   /* object to test */

#include "unittest.h"      /* unittest framework access */

static uint16_t unittest_pinpattern = 0;
static uint32_t unittest_nr_read_pins = 0;
static SNES2DB9_Pin unittest_pin_state[DB9_FIRE+1];

static void unittest_set_pin ( SNES2DB9_Pin pin, SNES2DB9_Pinstate state )
{
	if ( pin <= DB9_FIRE )
	{
		unittest_pin_state[pin] = state;
	}
	else
	{
		UT_Test ( false, "unittest_set_pin() - pin not allowed" );
	}
}

static SNES2DB9_Pinstate unittest_get_pin ( SNES2DB9_Pin pin )
{
	if ( pin <= DB9_FIRE )
	{
		unittest_nr_read_pins++;
		return unittest_pin_state[pin];
	}
	else
	{
		UT_Test ( false, "unittest_get_pin() - pin not allowed" );
		return SNES2DB9_PIN_LOW;
	}
}

static SNES2DB9_Pinstate unittest_get_pin_by_pattern ( SNES2DB9_Pin pin )
{
	if ( pin == SNES_DATA )
	{
		unittest_pin_state[SNES_DATA] = SNES2DB9_PIN_LOW;

		if ( ( unittest_pinpattern & 0x8000 ) != 0 )
		{
			unittest_pin_state[SNES_DATA] = SNES2DB9_PIN_HIGH;
		}

		unittest_pinpattern <<= 1;
		unittest_nr_read_pins++;
		return unittest_pin_state[SNES_DATA];
	}
	else
	{
		UT_Test ( false, "unittest_get_pin_by_pattern() - pin not allowed" );
		return SNES2DB9_PIN_LOW;
	}
}

/**
 * @brief main function for Unittest example
 * @param argc
 * @param argv
 * @return
 */
int main ( int argc, char **argv )
{
	uint16_t idx, result;
	char tmpstr[80];
	SNESReader reader;
	UT_ENABLE_HTML();
	UT_BEGIN ( "Unittest SNESReader()" );
	UT_TESTCASE ( "Object init" );
	UT_DESCRIPTION ( "Pin levels at default: LATCH = LOW, CLK = HIGH" );
	UT_DESCRIPTION ( "Pin levels read are all low" );
	SNESReader_Init ( &reader, unittest_set_pin, unittest_get_pin );
	UT_TEST ( unittest_pin_state[SNES_LATCH] == SNES2DB9_PIN_LOW );
	UT_TEST ( unittest_pin_state[SNES_CLK] == SNES2DB9_PIN_HIGH );
	UT_TESTCASE ( "State idle (100 cycles tested)" );

	for ( idx = 1; idx <= 100; idx++ )
	{
		sprintf ( tmpstr, "Cycle %d", idx );
		( void ) SNESReader_Update ( &reader );
		UT_PRECONDITION_STR ( tmpstr );
		UT_TEST ( unittest_pin_state[SNES_LATCH] == SNES2DB9_PIN_LOW );
		UT_TEST ( unittest_pin_state[SNES_CLK] == SNES2DB9_PIN_HIGH );
		UT_TEST ( unittest_nr_read_pins == 0 );
	}

	UT_TESTCASE ( "State latching (begin of read cycle)" );
	SNESReader_BeginRead ( &reader );
	( void ) SNESReader_Update ( &reader );
	UT_DESCRIPTION ( "Pin levels for latching: LATCH = HIGH, CLK = HIGH" );
	UT_TEST ( unittest_pin_state[SNES_LATCH] == SNES2DB9_PIN_HIGH );
	UT_TEST ( unittest_pin_state[SNES_CLK] == SNES2DB9_PIN_HIGH );
	UT_TEST ( unittest_nr_read_pins == 0 );
	UT_TESTCASE ( "State reading (first button)" );
	( void ) SNESReader_Update ( &reader );
	UT_DESCRIPTION ( "Pin levels for read: LATCH = LOW, CLK = HIGH" );
	UT_TEST ( unittest_pin_state[SNES_LATCH] == SNES2DB9_PIN_LOW );
	UT_TEST ( unittest_pin_state[SNES_CLK] == SNES2DB9_PIN_HIGH );
	UT_TEST ( unittest_nr_read_pins == 1 );

	for ( idx = 1; idx <=15; idx++ )
	{
		sprintf ( tmpstr, "Clock button %d", idx );
		UT_TESTCASE ( tmpstr );
		( void ) SNESReader_Update ( &reader );
		UT_DESCRIPTION ( "Pin levels for clock: LATCH = LOW, CLK = LOW" );
		UT_TEST ( unittest_pin_state[SNES_CLK] == SNES2DB9_PIN_LOW );
		UT_TEST ( unittest_pin_state[SNES_LATCH] == SNES2DB9_PIN_LOW );
		sprintf ( tmpstr, "unittest_nr_read_pins == %d", idx );
		UT_Test ( unittest_nr_read_pins == idx, tmpstr );
		sprintf ( tmpstr, "Read button %d", idx );
		( void ) SNESReader_Update ( &reader );
		UT_DESCRIPTION ( "Pin levels for clock: LATCH = LOW, CLK = HIGH" );
		UT_DESCRIPTION ( "One value was read" );
		UT_TEST ( unittest_pin_state[SNES_CLK] == SNES2DB9_PIN_HIGH );
		UT_TEST ( unittest_pin_state[SNES_LATCH] == SNES2DB9_PIN_LOW );
		sprintf ( tmpstr, "unittest_nr_read_pins == %d", ( idx+1 ) );
		UT_Test ( unittest_nr_read_pins == ( idx+1 ), tmpstr );
	}

	UT_TESTCASE ( "State update values" );
	UT_DESCRIPTION ( "All keys low, no SNES buttons pressed, signals on default level" );
	UT_TEST ( 0 == SNESReader_Update ( &reader ) );
	UT_TEST ( unittest_pin_state[SNES_LATCH] == SNES2DB9_PIN_LOW );
	UT_TEST ( unittest_pin_state[SNES_CLK] == SNES2DB9_PIN_HIGH );
	UT_TEST ( unittest_nr_read_pins == 16 );
	SNESReader_Init ( &reader, unittest_set_pin, unittest_get_pin_by_pattern );
	UT_TESTCASE ( "Reading defined pattern A" );
	UT_PRECONDITION ( unittest_pinpattern = ( SNES_BTNMASK_B|SNES_BTNMASK_L ) );
	UT_PRECONDITION ( unittest_nr_read_pins = 0 );
	SNESReader_BeginRead ( &reader );

	for ( idx = 0; idx < 40; idx++ )
	{
		result = SNESReader_Update ( &reader );
	}

	UT_TEST ( result == ( SNES_BTNMASK_B|SNES_BTNMASK_L ) );
	UT_TEST ( unittest_nr_read_pins == 16 );
	UT_TESTCASE ( "Reading defined pattern B" );
	UT_PRECONDITION ( unittest_pinpattern = ( SNES_BTNMASK_Up ) );
	UT_PRECONDITION ( unittest_nr_read_pins = 0 );
	SNESReader_BeginRead ( &reader );

	for ( idx = 0; idx < 40; idx++ )
	{
		result = SNESReader_Update ( &reader );
	}

	UT_TEST ( result == ( SNES_BTNMASK_Up ) );
	UT_TEST ( unittest_nr_read_pins == 16 );
	UT_END();
#ifdef GCOV_ENABLED
	return 0;
#else
	return UT_Result;
#endif
}

/** @} */
