/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    snes2db9_setdb9.c
 * @brief   implements DB9_SetPins
 * @details The DB9 related output ports are set according to the state. DB9 pins are active low.
 *
 */

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "snes2db9.h"

/**
 * @brief   internal helper function for readability of pin update
 * @details Pin is set to low if the bitmask is non-zero.
 * @param   setfunc points to hardware abstraction to update hardware pins
 * @param   pin to set
 * @param   bitmask of DB9_BTNMASK_xxx to decode positive logic
 */
static void UpdateDB9Pin(SNES2DB9_SetPinFunc setfunc, SNES2DB9_Pin pin, uint8_t bitmask)
{
    if(bitmask != 0)
    {
        setfunc(pin, SNES2DB9_PIN_LOW);
    }
    else
    {
        setfunc(pin, SNES2DB9_PIN_HIGH);
    }
}


void DB9_SetPins ( uint8_t state, SNES2DB9_SetPinFunc setfunc )
{
    assert(setfunc != NULL);

    UpdateDB9Pin(setfunc, DB9_UP,    (state & DB9_BTNMASK_Up));
    UpdateDB9Pin(setfunc, DB9_DOWN,  (state & DB9_BTNMASK_Down));
    UpdateDB9Pin(setfunc, DB9_LEFT,  (state & DB9_BTNMASK_Left));
    UpdateDB9Pin(setfunc, DB9_RIGHT, (state & DB9_BTNMASK_Right));
    UpdateDB9Pin(setfunc, DB9_FIRE,  (state & DB9_BTNMASK_Fire));
}
