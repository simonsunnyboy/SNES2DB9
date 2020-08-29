/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    snes2db9_reader.c
 * @brief   implements SNESReader object
 * @details The cycle time of the controller polling is derived from the callrate.
 *
 * @note    Cycles of 16µs or slower should be sufficient
 *
 */
 
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "snes2db9.h"

/* states 0...31 are either latch/clock or read  */
#define READER_ST_LATCH 0

#define READER_ST_UPDATE 32
#define READER_ST_IDLE 33

typedef enum CycleType
{
	IDLE,
	LATCH,
	CLOCK,
	READ
} CycleType;


static CycleType GetCycleType(uint8_t state)
{
	CycleType type;
	
	if(state >= READER_ST_IDLE)
	{
		type = IDLE;
	}
	else if(state == READER_ST_LATCH)
	{
		type = LATCH;
	}
	else if((state % 2) == 0)
	{
		type = CLOCK;
	} 
	else
	{
		type = READ;
	}
	
	return type;
}
 
 
void SNESReader_Init(SNESReader * self, SNES2DB9_SetPinFunc setfunc, SNES2DB9_ReadPinFunc readfunc)
{
	assert(self != NULL);
	assert(setfunc != NULL);
	assert(readfunc != NULL);	
		
	self->setpin = setfunc;
	self->getpin = readfunc;
	self->shiftreg = 0;
	self->result = 0;
	self->state = READER_ST_IDLE;
	
	/* set pins to default levels: */
	self->setpin(SNES_CLK, SNES2DB9_PIN_HIGH);
	self->setpin(SNES_LATCH, SNES2DB9_PIN_LOW);
	
}

void SNESReader_BeginRead(SNESReader * self)
{
	assert(self != NULL);
	self->state = READER_ST_LATCH;
}

uint16_t SNESReader_Update(SNESReader * self)
{
	assert(self != NULL);
	assert(self->setpin != NULL);
	assert(self->getpin != NULL);
	
	/* handle latch and clock command, shift register
	 * pins are physically updated first in the same order for each 
	 * case to ensure equal runtime.
	 * Additional workload such as shift register handling will not 
	 * introduce jitter.
	 */
	if(self->state == READER_ST_LATCH)
	{
		/* initiate latch pulse to high, clk idle high: */
		self->setpin(SNES_CLK, SNES2DB9_PIN_HIGH);
		self->setpin(SNES_LATCH, SNES2DB9_PIN_HIGH);
		
		/* prepare shift register for subsequent reads */
		self->shiftreg = 0;   
	}
	else if(self->state == READER_ST_UPDATE)
	{
		/* clock and latch at defaults, update overall result: */
		self->setpin(SNES_CLK, SNES2DB9_PIN_HIGH);
		self->setpin(SNES_LATCH, SNES2DB9_PIN_LOW);		
		self->result = self->shiftreg;
	}
	else if(GetCycleType(self->state) == CLOCK)
	{
		/* initiate clock pulse to low: */
		self->setpin(SNES_CLK, SNES2DB9_PIN_LOW);
		self->setpin(SNES_LATCH, SNES2DB9_PIN_LOW);
		
		/* update shift register for upcoming read cycle */
		self->shiftreg <<= 1; 
	}
	else
	{
		/* assume default pin states for CLK and LATCH:  */
		self->setpin(SNES_CLK, SNES2DB9_PIN_HIGH);
		self->setpin(SNES_LATCH, SNES2DB9_PIN_LOW);
	}
	
	/* perform read after update of CLK/LATCH pins: */
	if(GetCycleType(self->state) == READ)
	{
		SNES2DB9_Pinstate DATA = self->getpin(SNES_DATA);
		
		/* update date shift regisetr depending on data read: */
		if(DATA == SNES2DB9_PIN_HIGH)
		{
			self->shiftreg |= 1;
		}
	}
	
	/* go to next state: */
	if(self->state < READER_ST_IDLE)
	{
		self->state++;
	}
	
	return self->result;	
}
