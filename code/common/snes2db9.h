/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    snes2db9.h
 * @brief   public API for the SNEs to DB9 Jostick converter software
 * @details A implementation uses thus API to integrate the logic.
 *
 */

#ifndef SNES2DB9_H
#define SNES2DB9_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SNES_BTNMASK_B       0x8000
#define SNES_BTNMASK_Y       0x4000
#define SNES_BTNMASK_Select  0x2000
#define SNES_BTNMASK_Start   0x1000
#define SNES_BTNMASK_Up      0x0800
#define SNES_BTNMASK_Down    0x0400
#define SNES_BTNMASK_Left    0x0200
#define SNES_BTNMASK_Right   0x0100
#define SNES_BTNMASK_A       0x0080
#define SNES_BTNMASK_X       0x0040
#define SNES_BTNMASK_L       0x0020
#define SNES_BTNMASK_R       0x0010

#define DB9_BTNMASK_Up       1
#define DB9_BTNMASK_Down     2
#define DB9_BTNMASK_Left     4
#define DB9_BTNMASK_Right    8
#define DB9_BTNMASK_Fire     128

	enum SNES2DB9_Pinstate
	{
		SNES2DB9_PIN_LOW,
		SNES2DB9_PIN_HIGH
	};

	typedef enum SNES2DB9_Pinstate SNES2DB9_Pinstate;

	enum SNES2DB9_Pin
	{
		SNES_LATCH,
		SNES_CLK,
		SNES_DATA,
		DB9_UP,
		DB9_DOWN,
		DB9_LEFT,
		DB9_RIGHT,
		DB9_FIRE
	};

	typedef enum SNES2DB9_Pin SNES2DB9_Pin;

	typedef void ( *SNES2DB9_SetPinFunc ) ( SNES2DB9_Pin pin, SNES2DB9_Pinstate state );

	typedef SNES2DB9_Pinstate ( *SNES2DB9_ReadPinFunc ) ( SNES2DB9_Pin pin );

	struct SNESReader
	{
		SNES2DB9_SetPinFunc  setpin;
		SNES2DB9_ReadPinFunc getpin;
		uint16_t             shiftreg;
		uint16_t             result;
		uint8_t              state;
	};

	typedef struct SNESReader SNESReader;

	void     SNESReader_Init ( SNESReader * self, SNES2DB9_SetPinFunc setfunc, SNES2DB9_ReadPinFunc readfunc );
	uint16_t SNESReader_Update ( SNESReader * self );
	void     SNESReader_BeginRead ( SNESReader * self );

	void DB9_SetPins ( uint8_t state, SNES2DB9_SetPinFunc setfunc );

#ifdef __cplusplus
}
#endif

#endif
