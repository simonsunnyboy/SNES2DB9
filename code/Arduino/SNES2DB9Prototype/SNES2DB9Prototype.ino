/**
    SNES to DB9 Joystick converter - Arduino prototype with serial output

    (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
    http://www.asmsoftware.de/

    The MIT License applies to this software. See COPYING for details.

    @file    SNES2DB9Prototype.ino
    @brief   reads SNES pad via SNESReader with serial debug output on Arduino Nano
    @details This is used to validate hardware pad reading.

*/

#include "snes2db9.h"

static SNESReader reader;           /**< SNES reader instance */
static uint32_t previousMillis = 0; /**< keep track of milliseconds passed */

static uint16_t previousSNES = 0;

/**
    @brief Arduino pin mapping to SNES2DB9 mapping
*/
static uint8_t pin_id[] =
{
    [SNES_LATCH] = 3,
    [SNES_CLK] = 4,
    [SNES_DATA] = 5,
    [DB9_UP] = 7,
    [DB9_DOWN] = 8,
    [DB9_LEFT] = 9,
    [DB9_RIGHT] = 10,
    [DB9_FIRE] = 11
};

/**
    @brief   hardware interaction fucntion to set pin to a given state
    @details The Arduino pin mapping is used to update the associated Arduino signal.
*/
void pin_write ( SNES2DB9_Pin pin, SNES2DB9_Pinstate state )
{
    if (pin <= DB9_FIRE)
    {
		if (state == SNES2DB9_PIN_HIGHZ)
        {
			pinMode(pin_id[pin], INPUT);
        }
        else if (state == SNES2DB9_PIN_HIGH)
        {
			pinMode(pin_id[pin], OUTPUT);
            digitalWrite(pin_id[pin], 1);
        }
        else
        {
			pinMode(pin_id[pin], OUTPUT);
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
    if (pin <= DB9_FIRE)
    {
        return (SNES2DB9_Pinstate)digitalRead(pin_id[pin]);   
    }
    else
    {
        return SNES2DB9_PIN_LOW;
    }
}


void setup()
{
    /* setup pin states */
    for (uint8_t pin_idx = 0; pin_idx < sizeof(pin_id); pin_idx ++)
    {
        pinMode(pin_id[pin_idx], OUTPUT);
    }
    pinMode(pin_id[SNES_DATA], INPUT);


    /* initialize reader instance */
    SNESReader_Init(&reader, pin_write, pin_read);

    SNESReader_BeginRead(&reader);

    Serial.begin(9600);
}

void loop()
{
    // generate pulse pattern and update SNES reader state
    delay(1);
    uint16_t currentSNES = SNESReader_Update(&reader);

    if(previousSNES != currentSNES)
    {
        if(currentSNES & SNES_BTNMASK_Up)
        {
            Serial.print("UP ");
        }

        if(currentSNES & SNES_BTNMASK_Down)
        {
            Serial.print("DOWN ");
        }

        if(currentSNES & SNES_BTNMASK_Left)
        {
            Serial.print("LEFT ");
        }

        if(currentSNES & SNES_BTNMASK_Right)
        {
            Serial.print("RIGHT ");
        }

        if(currentSNES & SNES_BTNMASK_A)
        {
            Serial.print("A ");
        }

        if(currentSNES & SNES_BTNMASK_B)
        {
            Serial.print("B ");
        }

        if(currentSNES & SNES_BTNMASK_X)
        {
            Serial.print("X ");
        }

        if(currentSNES & SNES_BTNMASK_Y)
        {
            Serial.print("Y ");
        }

        if(currentSNES & SNES_BTNMASK_L)
        {
            Serial.print("L ");
        }

        if(currentSNES & SNES_BTNMASK_R)
        {
            Serial.print("R ");
        }

        if(currentSNES & SNES_BTNMASK_Select)
        {
            Serial.print("SELECT ");
        }

        if(currentSNES & SNES_BTNMASK_Start)
        {
            Serial.print("START ");
        }
        
        Serial.println("");
        
        previousSNES = currentSNES;
    }

    // schedule SNES reading in intervalls
    if (millis() - previousMillis >= 1000)
    {
        previousMillis += 1000;

        
        

        SNESReader_BeginRead(&reader);
    }
}

