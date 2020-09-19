/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    snes2db9_mapper.c
 * @brief   implements SNESMapper object
 * @details The mapper maps SNES button masks to DB9 masks and processes jump/autofire functionalities.
 *
 */

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "snes2db9.h"

/**
 * @brief          internal helper function to computer autofire state based on callcycle
 * @details        Pin states are not affected. Internal state autofire_active is calculated.
 * @param[in, out] self points to instance of SNESMapper
 * @param[in]      millis_passed is the number of ms passed since last call to SNESMapper_Update
 */
static void ComputeAutofireState ( SNESMapper * self, uint16_t millis_passed )
{
	assert ( self != NULL );
	self->millis += millis_passed;

    if ( self->autofire_cycletime_millis == 0 )
	{
		self->autofire_active = false;
	}
	else
	{
		if ( self->millis >= self->autofire_cycletime_millis )
		{
			self->millis -= self->autofire_cycletime_millis;
            self->autofire_active = !self->autofire_active;
		}
	}
}

void     SNESMapper_Init ( SNESMapper * self, SNESMapperButtonMasks * button_mask_config )
{
	assert ( self != NULL );
	assert ( button_mask_config != NULL );
	self->button_masks = *button_mask_config;
	self->millis = 0;
	self->autofire_active = false;
	self->autofire_cycletime_millis = AUTOFIRE_CYCLETIME_IN_MS;
}

void     SNESMapper_SetAutofireDuration ( SNESMapper * self, uint16_t autofire_cycletime_millis )
{
	assert ( self != NULL );
	self->autofire_cycletime_millis = autofire_cycletime_millis;
}

uint8_t  SNESMapper_Update ( SNESMapper * self, uint16_t snes_pin_mask, uint16_t millis_passed )
{
	assert ( self != NULL );
	uint8_t db9_btnmask = 0;
	ComputeAutofireState ( self, millis_passed );

	if ( ( snes_pin_mask & SNES_BTNMASK_Up ) != 0 )
	{
		db9_btnmask |= DB9_BTNMASK_Up;
	}

	if ( ( snes_pin_mask & SNES_BTNMASK_Down ) != 0 )
	{
		db9_btnmask |= DB9_BTNMASK_Down;
	}

	if ( ( snes_pin_mask & SNES_BTNMASK_Left ) != 0 )
	{
		db9_btnmask |= DB9_BTNMASK_Left;
	}

	if ( ( snes_pin_mask & SNES_BTNMASK_Right ) != 0 )
	{
		db9_btnmask |= DB9_BTNMASK_Right;
	}

	if ( ( snes_pin_mask & self->button_masks.fire_mask ) != 0 )
	{
		db9_btnmask |= DB9_BTNMASK_Fire;
	}

	if ( ( snes_pin_mask & self->button_masks.jump_mask ) != 0 )
	{
		db9_btnmask |= DB9_BTNMASK_Up;
	}

	/* carryover autofire state if active: */
	if ( ( ( snes_pin_mask & self->button_masks.autofire_mask ) != 0 ) &&
	        ( self->autofire_active == true )
	   )
	{
		db9_btnmask |= DB9_BTNMASK_Fire;
	}

	return db9_btnmask;
}
