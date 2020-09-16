/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    test_setdb9.c
 * @brief   unittest implementation for DB9_SetPins
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "snes2db9.h"   /* object to test */

#include "unittest.h"      /* unittest framework access */

typedef struct
{
    char * testcase_desc;
    bool up;
    bool down;
    bool left;
    bool right;
    bool fire;
} DB9_SetPins_Testcase;

static uint16_t nr_wrong_pin_writes = 0;
static SNES2DB9_Pinstate ut_pinstate[DB9_FIRE + 1];

static void append_pin_state(char *deststr, char *prefix, SNES2DB9_Pinstate pin_state)
{

    strcat(deststr, prefix);

    if(pin_state == SNES2DB9_PIN_HIGH)
    {
        strcat(deststr, "HIGH ");
    }
    else if(pin_state == SNES2DB9_PIN_LOW)
    {
        strcat(deststr, "LOW ");
    }
    else
    {
        strcat(deststr, "<undefined> ");
        nr_wrong_pin_writes++;
    }
}

static void document_actual_pin_states ( char * deststr)
{
    strcpy(deststr, "Actual pinstates after update: ");

    append_pin_state( deststr, "UP = ", ut_pinstate[DB9_UP]);
    append_pin_state( deststr, "DOWN = ", ut_pinstate[DB9_DOWN]);
    append_pin_state( deststr, "LEFT = ", ut_pinstate[DB9_LEFT]);
    append_pin_state( deststr, "RIGHT = ", ut_pinstate[DB9_RIGHT]);
    append_pin_state( deststr, "FIRE = ", ut_pinstate[DB9_FIRE]);
}

static void ut_setpin ( SNES2DB9_Pin pin, SNES2DB9_Pinstate state )
{
    if (( pin == DB9_UP ) ||
        ( pin == DB9_DOWN ) ||
        ( pin == DB9_LEFT ) ||
        ( pin == DB9_RIGHT ) ||
        ( pin == DB9_FIRE )
       )
    {
        ut_pinstate[pin] = state;
    }
    else
    {
        nr_wrong_pin_writes++;
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
    char tmpstr[120];

    SNES2DB9_Pinstate ut_expected_pinstate[DB9_FIRE + 1];

    DB9_SetPins_Testcase tcs[] =
    {
      {"joystick idle",            false, false, false, false, false},
      {"joystick up",              true,  false, false, false, false},
      {"joystick up+fire",         true,  false, false, false, true},
      {"joystick up+left",         true,  false, true,  false, false},
      {"joystick up+left+fire",    true,  false, true,  false, true},
      {"joystick left",            false, false, true,  false, false},
      {"joystick left+fire",       false, false, true,  false, true},
      {"joystick down+left",       false, true,  true,  false, false},
      {"joystick down+left+fire",  false, true,  true,  false, true},
      {"joystick down",            false, true,  false, false, false},
      {"joystick down+fire",       false, true,  false, false, true},
      {"joystick down+right",      false, true,  false, true,  false},
      {"joystick down+right+fire", false, true,  false, true,  true},
      {"joystick right",           false, false, false, true,  false},
      {"joystick right+fire",      false, false, false, true,  true},
      {"joystick up+right",        true,  false, false, true,  false},
      {"joystick up+right+fire",   true,  false, false, true,  true},
    };

    const static uint16_t nr_tcs = (sizeof(tcs)/sizeof(DB9_SetPins_Testcase));

    UT_ENABLE_HTML();
    UT_BEGIN ( "Unittest SNESReader()" );

    for(uint16_t idx = 0; idx < nr_tcs; idx ++)
    {
        uint8_t db9_state = 0;

        strcpy(tmpstr, "Expected pins to be low: ");

        UT_TESTCASE(tcs[idx].testcase_desc);

        ut_expected_pinstate[DB9_UP] = SNES2DB9_PIN_HIGH;
        ut_expected_pinstate[DB9_DOWN] = SNES2DB9_PIN_HIGH;
        ut_expected_pinstate[DB9_LEFT] = SNES2DB9_PIN_HIGH;
        ut_expected_pinstate[DB9_RIGHT] = SNES2DB9_PIN_HIGH;
        ut_expected_pinstate[DB9_FIRE] = SNES2DB9_PIN_HIGH;

        /* configure test input and expected outcome: */
        if(tcs[idx].up)
        {
            db9_state |= DB9_BTNMASK_Up;
            ut_expected_pinstate[DB9_UP] = SNES2DB9_PIN_LOW;
            strcat(tmpstr, "UP ");

        }
        if(tcs[idx].down)
        {
            db9_state |= DB9_BTNMASK_Down;
            ut_expected_pinstate[DB9_DOWN] = SNES2DB9_PIN_LOW;
            strcat(tmpstr, "DOWN ");
        }
        if(tcs[idx].left)
        {
            db9_state |= DB9_BTNMASK_Left;
            ut_expected_pinstate[DB9_LEFT] = SNES2DB9_PIN_LOW;
            strcat(tmpstr, "LEFT ");
        }
        if(tcs[idx].right)
        {
            db9_state |= DB9_BTNMASK_Right;
            ut_expected_pinstate[DB9_RIGHT] = SNES2DB9_PIN_LOW;
            strcat(tmpstr, "RIGHT ");
        }
        if(tcs[idx].fire)
        {
            db9_state |= DB9_BTNMASK_Fire;
            ut_expected_pinstate[DB9_FIRE] = SNES2DB9_PIN_LOW;
            strcat(tmpstr, "FIRE ");
        }
        if(db9_state == 0)
        {
            strcat(tmpstr, "<none> ");
        }

        UT_Description(tmpstr);

        /* update pins */
        DB9_SetPins(db9_state, ut_setpin);

        document_actual_pin_states(tmpstr);
        UT_PRECONDITION_STR(tmpstr);

        /* perform tests: */
        UT_TEST(nr_wrong_pin_writes == 0);

        UT_TEST(ut_expected_pinstate[DB9_UP] == ut_pinstate[DB9_UP]);
        UT_TEST(ut_expected_pinstate[DB9_DOWN] == ut_pinstate[DB9_DOWN]);
        UT_TEST(ut_expected_pinstate[DB9_LEFT] == ut_pinstate[DB9_LEFT]);
        UT_TEST(ut_expected_pinstate[DB9_RIGHT] == ut_pinstate[DB9_RIGHT]);
        UT_TEST(ut_expected_pinstate[DB9_FIRE] == ut_pinstate[DB9_FIRE]);
    }

    UT_END();
#ifdef GCOV_ENABLED
    return 0;
#else
    return UT_Result;
#endif
}

/** @} */
