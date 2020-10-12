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

/**
 * @addtogroup SNES2DB9
 * @{
 */

#ifndef SNES2DB9_H
#define SNES2DB9_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup SNES_BTNMASK_xxx
 * @{
 */
#define SNES_BTNMASK_B       0x8000  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_Y       0x4000  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_Select  0x2000  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_Start   0x1000  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_Up      0x0800  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_Down    0x0400  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_Left    0x0200  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_Right   0x0100  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_A       0x0080  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_X       0x0040  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_L       0x0020  /**< internal bitmask used for SNES button readings */
#define SNES_BTNMASK_R       0x0010  /**< internal bitmask used for SNES button readings */
/** @} */

/**
 * @addtogroup DB9_BTNMASK_xxx
 * @{
 */
#define DB9_BTNMASK_Up       1       /**< internal bitmask used for DB9 output settings  */
#define DB9_BTNMASK_Down     2       /**< internal bitmask used for DB9 output settings  */
#define DB9_BTNMASK_Left     4       /**< internal bitmask used for DB9 output settings  */
#define DB9_BTNMASK_Right    8       /**< internal bitmask used for DB9 output settings  */
#define DB9_BTNMASK_Fire     128     /**< internal bitmask used for DB9 output settings  */
/** @} */

#define AUTOFIRE_CYCLETIME_IN_MS 100 /**< default autofire cycletime in ms */

/**
 * @brief   possible pin states to control SNES gamepad reading and DB9 output signals
 * @details The pinstates are used by the hardware abstraction routines to be implemented by the calling application.
 */
enum SNES2DB9_Pinstate
{
    SNES2DB9_PIN_LOW,
    SNES2DB9_PIN_HIGH,
    SNES2DB9_PIN_HIGHZ
};

typedef enum SNES2DB9_Pinstate SNES2DB9_Pinstate; /**< see enum SNES2DB9_Pinstate */

/**
 * @brief port pin abstraction naming
 * @details The port pin names are used by the hardware abstraction routines to be implemented by the calling application to address pins.
 */
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

typedef enum SNES2DB9_Pin SNES2DB9_Pin;  /**< see enum SNES2DB9_Pin */

/**
 * @brief     prototype for hardware abstraction to set a given pin to a new state
 * @param[in] pin to set
 * @param[in] state to set
 */
typedef void ( *SNES2DB9_SetPinFunc ) ( SNES2DB9_Pin pin, SNES2DB9_Pinstate state );

/**
 * @brief     prototype for hardware abstraction to read a given pin state
 * @param[in] pin to read
 * @returns   pin state according to SNES2DB9_Pinstate
 */
typedef SNES2DB9_Pinstate ( *SNES2DB9_ReadPinFunc ) ( SNES2DB9_Pin pin );

/**
 * @brief   implements object to read the SNES gamepad
 * @details All members hall be considered private. Access should be routed through the SNESReader_... functions
 */
struct SNESReader
{
    SNES2DB9_SetPinFunc  setpin;    /**< function pointer to hardware access function to set pin states */
    SNES2DB9_ReadPinFunc getpin;    /**< function pointer to hardware access function to read pin states */
    uint16_t             shiftreg;  /**< internal shift register to accumulate SNES button states read */
    uint16_t             result;    /**< last complete SNES reading, bitcoded according to SNES_BTNMASK_xxx */
    uint8_t              state;     /**< internal state */
};

typedef struct SNESReader SNESReader;

/**
 * @brief   defines SNES button bitmasks to configure operation of an SNESMapper instance
 * @details Masks are bitmapped according to SNES_BTNMASK_xxx macros
 *          - Disable a functionality by setting the mask to 0
 *          - Map a functionality to one or more SNES buttons with bitmasks of combined SNES_BTNMASK_xxx values.
 * @see     SNESMapper
 */
struct SNESMapperButtonMasks
{
    uint16_t fire_mask;      /**< SNES button set for regular fire button action */
    uint16_t autofire_mask;  /**< SNES button set enabling autofire mode */
    uint16_t jump_mask;      /**< SNES button set enabling jump button mode as an alternative to "Pad Up" */
};

typedef struct SNESMapperButtonMasks SNESMapperButtonMasks;

/**
 * @brief   implements object to map SNES button masks to DB9 button masks
 * @details All members hall be considered private. Access should be routed through the SNESMapper_... functions
 */
struct SNESMapper
{
    struct SNESMapperButtonMasks button_masks;               /**< SNES button mapping configuration */
    uint16_t                     millis;                     /**< internal timestamp in ms */
    uint16_t                     autofire_cycletime_millis;  /**< autofire toggle cycle time in ms */
    bool                         autofire_active;            /**< internal autofire state */
};

typedef struct SNESMapper SNESMapper;

/**
 * @brief          initializes SNESReader instance
 * @details        The caller has to assign hardware abstraction functions for hardware access.
 * @attention      Function pointers for hardware abstraction functio must be valid and may not be NULL.
 * @param[in, out] self points to instance of SNESReader
 * @param[in]      setfunc points to hardware abstraction function to set port pins
 * @param[in]      readfunc points to hardware abstraction function to read port pins
 */
void     SNESReader_Init ( SNESReader * self, SNES2DB9_SetPinFunc setfunc, SNES2DB9_ReadPinFunc readfunc );

/**
 * @brief          updates SNESReader internal state until complete reading has been obtained
 * @details        - The callrate determines duration of SNES hardware control pulses.
 *                 - Once a complete reading has been obtained, the update a new reading must be requested through SNESReader_BeginRead()
 * @see            SNESReader_BeginRead
 * @param[in, out] self points to instance of SNESReader
 * @returns        last complete SNES reading, bitcoded according to SNES_BTNMASK_xxx (active high)
 */
uint16_t SNESReader_Update ( SNESReader * self );

/**
 * @brief          restarts the SNES gamepad reading process with a latch pulse
 * @param[in, out] self points to instance of SNESReader
 */
void     SNESReader_BeginRead ( SNESReader * self );

/**
 * @brief          initializes SNESMapper instance
 * @details        - The caller has to assign SNES button masks for subsequent operation.
 *                 - Autofire toggle cycle time is set to default value.
 * @param[in, out] self points to instance of SNESMapper
 * @param[in]      button_mask_config points toSNES button mask configuration to be used
 */
void     SNESMapper_Init ( SNESMapper * self, SNESMapperButtonMasks * button_mask_config);

/**
 * @brief          updates autofire toggle cycle time of given SNESMapper instance
 * @details        A cycle time of 0ms disables autofire functionality.
 * @param[in, out] self points to instance of SNESMapper
 * @param[in]      autofire_cycletime_millis to use
 */
void     SNESMapper_SetAutofireDuration ( SNESMapper * self, uint16_t autofire_cycletime_millis );

/**
 * @brief          updates DB9 pin state configuration from given SNES button inputs
 * @details        Autofire behaviour is computed during the update.
 * @param[in, out] self points to instance of SNESMapper
 * @param[in]      snes_pin_mask describes the current SNES button state as a  bitmask composed of SNES_BTNMASK_xxx (active high)
 * @param[in]      millis_passed is the number of ms passed since last call to SNESMapper_Update
 * @returns        resulting DB9 setting bitmask composed of DB9_BTNMASK_xxx
 */
uint8_t  SNESMapper_Update ( SNESMapper * self, uint16_t snes_pin_mask, uint16_t millis_passed );

/**
 * @brief     sets the port pins to set the DB9 state
 * @details   The state is decoded with positive logic.
 *            Hardware output is configure active low through the hardware abstraction function.
 * @param[in] state is the desired DB9 setting bitmask composed of DB9_BTNMASK_xxx
 * @param[in] setfunc points to hardware abstraction function to update a pin state
 */
void DB9_SetPins ( uint8_t state, SNES2DB9_SetPinFunc setfunc );


#ifdef __cplusplus
}
#endif

#endif

/** @} */
