/**
	SNES to DB9 Joystick converter - fully functional Arduino prototype

	(c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
	http://www.asmsoftware.de/

	The MIT License applies to this software. See COPYING for details.

	@file    SNES2DB9.ino
	@brief   SNES2DB9 implementation on Arduino Nano
	@details This is used to validate hardware pad reading and DB9 output update with desired timing.

*/

#include "snes2db9.h"

static SNESReader reader;           /**< SNES reader instance */
static uint32_t previousMillis = 0; /**< keep track of milliseconds passed */
static uint16_t currentSNES = 0;    /**< SNES pad state as read */


static volatile uint8_t _200micros = 0;  /**< internal state of 200 µS ticks (updated by Timer 1) */

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
    if (pin <= DB9_FIRE)
    {
        return (SNES2DB9_Pinstate)digitalRead(pin_id[pin]);
    }
    else
    {
        return SNES2DB9_PIN_LOW;
    }
}

ISR(TIMER1_COMPA_vect) {
    _200micros++;
}

void setupTimer1() {
    noInterrupts();
    // Clear registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // 5000 Hz (16000000/((49+1)*64))
    OCR1A = 49;
    // CTC
    TCCR1B |= (1 << WGM12);
    // Prescaler 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
    // Output Compare Match A Interrupt Enable
    TIMSK1 |= (1 << OCIE1A);
    interrupts();
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

    /* prepare timer interrupt for SNES polling cycle: */
    setupTimer1();

}

void loop()
{
    // generate pulse pattern and update SNES reader state on timer tick

    if (_200micros != 0)
    {
        currentSNES = SNESReader_Update(&reader);
        _200micros = 0;
    }

    // schedule SNES reading in intervalls of 16ms (60Hz)
    if (millis() - previousMillis >= 16)
    {
        uint8_t db9_btnmask = 0;

        if (currentSNES & SNES_BTNMASK_Up)
        {
            db9_btnmask |= DB9_BTNMASK_Up;
        }

        if (currentSNES & SNES_BTNMASK_Down)
        {
            db9_btnmask |= DB9_BTNMASK_Down;
        }

        if (currentSNES & SNES_BTNMASK_Left)
        {
            db9_btnmask |= DB9_BTNMASK_Left;
        }

        if (currentSNES & SNES_BTNMASK_Right)
        {
            db9_btnmask |= DB9_BTNMASK_Right;
        }

        if (currentSNES & (SNES_BTNMASK_B | SNES_BTNMASK_A | SNES_BTNMASK_X | SNES_BTNMASK_Y | SNES_BTNMASK_L | SNES_BTNMASK_R))
        {
            db9_btnmask |= DB9_BTNMASK_Fire;
        }

        DB9_SetPins(db9_btnmask, pin_write);

        SNESReader_BeginRead(&reader);

        previousMillis += 16;
    }
}

