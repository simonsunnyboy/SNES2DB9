/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    test_mapper.c
 * @brief   unittest implementation for SNESMapper
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "snes2db9.h"   /* object to test */

#include "unittest.h"      /* unittest framework access */

/**
 * @brief main function for Unittest example
 * @param argc
 * @param argv
 * @return
 */
int main ( int argc, char **argv )
{
	uint16_t ut_snes_input_state;
	uint16_t cnt;
	SNESMapper ut_mapper;  /**< mapper instance under test */
	char tmpstr[80];
	UT_ENABLE_HTML();
	UT_BEGIN ( "Unittest SNESMapper()" );
	UT_TESTCASE ( "Mapper object initialization, Autofire disabled" );
	SNESMapperButtonMasks masks_used;
	UT_PRECONDITION ( masks_used.fire_mask     = SNES_BTNMASK_B );
	UT_PRECONDITION ( masks_used.jump_mask     = SNES_BTNMASK_A );
	UT_PRECONDITION ( masks_used.autofire_mask = SNES_BTNMASK_L );
	UT_PRECONDITION_STR ( "SNESMapper_Init(&ut_mapper, &masks_used)" );
	SNESMapper_Init ( &ut_mapper, &masks_used );
	SNESMapper_SetAutofireDuration ( &ut_mapper, 0 );
	UT_TEST ( ut_mapper.millis == 0 );
	UT_TEST ( ut_mapper.autofire_cycletime_millis == 0 );
	UT_TEST ( ut_mapper.autofire_active == false );
	UT_TEST ( memcmp ( &ut_mapper.button_masks, &masks_used, sizeof ( SNESMapperButtonMasks ) ) == 0 );
	UT_TESTCASE ( "Joypad idle" );
	UT_PRECONDITION ( ut_snes_input_state = 0 );
	UT_TEST ( 0 == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Up" );
	UT_PRECONDITION ( ut_snes_input_state = SNES_BTNMASK_Up );
	UT_TEST ( DB9_BTNMASK_Up == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Up+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Up | SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Up|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Up+Right" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Up| SNES_BTNMASK_Right ) );
	UT_TEST ( ( DB9_BTNMASK_Up|DB9_BTNMASK_Right ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Up+Right+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Up| SNES_BTNMASK_Right| SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Up|DB9_BTNMASK_Right|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Right" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Right ) );
	UT_TEST ( ( DB9_BTNMASK_Right ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Right+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Right|SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Right|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Down+Right" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Down| SNES_BTNMASK_Right ) );
	UT_TEST ( ( DB9_BTNMASK_Down|DB9_BTNMASK_Right ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Down+Right+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Down| SNES_BTNMASK_Right| SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Down|DB9_BTNMASK_Right|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Down" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Down ) );
	UT_TEST ( ( DB9_BTNMASK_Down ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Down+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Down|SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Down|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Down+Left" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Down| SNES_BTNMASK_Left ) );
	UT_TEST ( ( DB9_BTNMASK_Down|DB9_BTNMASK_Left ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Down+Left+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Down| SNES_BTNMASK_Left | SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Down|DB9_BTNMASK_Left|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Left" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Left ) );
	UT_TEST ( ( DB9_BTNMASK_Left ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Left+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Left|SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Left|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Up+Left" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Up| SNES_BTNMASK_Left ) );
	UT_TEST ( ( DB9_BTNMASK_Up|DB9_BTNMASK_Left ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Up+Left+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_Up| SNES_BTNMASK_Left| SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Up|DB9_BTNMASK_Left|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Jump" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_A ) );
	UT_TEST ( ( DB9_BTNMASK_Up ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "Joypad Jump+Fire" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_A|SNES_BTNMASK_B ) );
	UT_TEST ( ( DB9_BTNMASK_Up|DB9_BTNMASK_Fire ) == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 16 ) );
	UT_TESTCASE ( "enable Autofire by setting duration on 20ms, begin in off state" );
	UT_PRECONDITION ( SNESMapper_SetAutofireDuration ( &ut_mapper, 20 ) );
	UT_PRECONDITION ( ut_mapper.millis = 0 );
	UT_PRECONDITION ( ut_mapper.autofire_active = false );
	UT_COMMENT ( "internal time is reset" );
	UT_TEST ( ut_mapper.autofire_cycletime_millis == 20 );
	UT_TESTCASE ( "test Autofire, every 20ms fire must toggle" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_L ) );
	UT_PRECONDITION_STR ( "waiting 19ms" );

	for ( cnt = 0; cnt < 19; cnt++ )
	{
		sprintf ( tmpstr, "ms elapsed: %d", cnt );
		UT_COMMENT ( tmpstr );
		UT_TEST ( 0 == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 1 ) );
	}

	UT_TESTCASE ( "test Autofire, every 20ms fire must toggle, now turning on" );
	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_L ) );
	UT_PRECONDITION_STR ( "on for 20ms" );

	for ( cnt = 0; cnt < 20; cnt++ )
	{
		sprintf ( tmpstr, "ms elapsed: %d", cnt );
		UT_COMMENT ( tmpstr );
		UT_TEST ( DB9_BTNMASK_Fire == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 1 ) );
	}

	UT_PRECONDITION ( ut_snes_input_state = ( SNES_BTNMASK_L ) );
	UT_PRECONDITION_STR ( "off for 20 ms" );

	for ( cnt = 0; cnt < 20; cnt++ )
	{
		sprintf ( tmpstr, "ms elapsed: %d", cnt );
		UT_COMMENT ( tmpstr );
		UT_TEST ( 0 == SNESMapper_Update ( &ut_mapper, ut_snes_input_state, 1 ) );
	}

	UT_END();
#ifdef GCOV_ENABLED
	return 0;
#else
	return UT_Result;
#endif
}

/** @} */
