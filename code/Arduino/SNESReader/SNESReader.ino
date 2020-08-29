/**
    SNES to DB9 Joystick converter

    (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
    http://www.asmsoftware.de/

    The MIT License applies to this software. See COPYING for details.

    @file    SNESReader.ino
    @brief   implements SNESReader handling on Arduino Nano
    @details This is used for wave pattern verification.

*/

#include "snes2db9.h"

static SNESReader reader;           /**< SNES reader instance */
static uint32_t previousMillis = 0; /**< keep track of milliseconds passed */

/**
    @brief Arduino pin mapping to SNES2DB9 mapping
*/
static uint8_t pin_id[] =
{
    [SNES_LATCH] = 3,
    [SNES_CLK] = 4,
    [SNES_DATA] = 13,
    [DB9_UP] = 8,
    [DB9_DOWN] = 9,
    [DB9_LEFT] = 10,
    [DB9_RIGHT] = 11,
    [DB9_FIRE] = 12
};

/**
    @brief   hardware interaction fucntion to set pin to a given state
    @details The Arduino pin mapping is used to update the associated Arduino signal.
*/
void pin_write ( SNES2DB9_Pin pin, SNES2DB9_Pinstate state )
{
    if (pin <= DB9_FIRE)
    {
        if (state == SNES2DB9_PIN_HIGH)
        {
            digitalWrite(pin_id[pin], 1);
        }
        else
        {
            digitalWrite(pin_id[pin], 0);
        }
    }
}

/**
    @brief   hardware interaction fucntion to read pin state
    @details The Arduino pin mapping is used to read the associated Arduino signal and convert to SNES2DB9 realm.
*/
SNES2DB9_Pinstate pin_read ( SNES2DB9_Pin pin )
{
    /* toggle helper pin for read indication */
    digitalWrite(7, 1);
    digitalWrite(7, 0);

    return SNES2DB9_PIN_LOW;
}


void setup()
{
    /* setup pin states */
    for (uint8_t pin_idx = 0; pin_idx < sizeof(pin_id); pin_idx ++)
    {
        pinMode(pin_id[pin_idx], OUTPUT);
    }
    pinMode(pin_id[SNES_DATA], INPUT);

    /* helper pin to indicate read access */
    pinMode(7, OUTPUT);
    digitalWrite(7, 0);

    /* initialize reader instance */
    SNESReader_Init(&reader, pin_write, pin_read);

    SNESReader_BeginRead(&reader);
}

void loop()
{
    // generate pulse pattern and update SNES reader state
    delay(1);
    SNESReader_Update(&reader);

    // schedule SNES reading in intervalls
    if (millis() - previousMillis >= 1000)
    {
        previousMillis += 1000;

        SNESReader_BeginRead(&reader);
    }
}

