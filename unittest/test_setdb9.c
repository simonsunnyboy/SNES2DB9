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

    UT_ENABLE_HTML();
    UT_BEGIN ( "Unittest SNESReader()" );

    UT_END();
#ifdef GCOV_ENABLED
    return 0;
#else
    return UT_Result;
#endif
}

/** @} */
