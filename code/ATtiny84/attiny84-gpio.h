/**
 * Generated port pin mapping
 * Portpin mapper for AVRs
 * @file attiny84-gpio.h
 * @brief Pin mapping for SNES2DB9 implementation on ATtiny84
 * @author Matthias Arndt <marndt@asmsoftware.de>
 * @see http://www.final-memory.org/?p=2687
 */
#ifndef ATTINY84GPIOH_H
#define ATTINY84GPIOH_H
/* User preprocessor directives: */


#include <avr/io.h>
#include <stdint.h>

/* bit mappings: */
#define PINMASK0 1   /**< ... */
#define PINMASK1 2   /**< ... */
#define PINMASK2 4   /**< ... */
#define PINMASK3 8   /**< ... */
#define PINMASK4 16   /**< ... */
#define PINMASK5 32   /**< ... */
#define PINMASK6 64   /**< ... */
#define PINMASK7 128   /**< ... */

/* helper macros: */
#define SET_BIT(byte,bitmask)    (byte)|=(bitmask)   /**< set a bit(mask) inside a byte */
#define CLEAR_BIT(byte,bitmask)    (byte)&=(0xFFU ^ bitmask)   /**< clear a bit(mask) inside a byte */
#define TOGGLE_BIT(byte,bitmask)    (byte)^=(bitmask)   /**< toggle a bit(mask) inside a byte */

/* signal mappings: */
#define DATA_PORT    PORTB   /**< GPIO port for signal DATA */
#define DATA_PIN    PINMASK2   /**< GPIO pin bitmask for signal DATA */
#define DATA_AS_OUTPUT    SET_BIT(DDRB, PIN2)   /**< set GPIO as push/pull output for signal DATA */
#define DATA_AS_INPUT    CLEAR_BIT(DDRB, PIN2); SET_BIT(PORTB, PIN2)   /**< set GPIO as input with pullup for signal DATA */
#define DATA_AS_HIGHZ    CLEAR_BIT(DDRB, PIN2); CLEAR_BIT(PORTB, PIN2)   /**< set GPIO as input High-Z without pullup for signal DATA */
#define SET_DATA    SET_BIT(PORTB, PIN2)   /**< set GPIO pin high for signal DATA */
#define CLEAR_DATA    CLEAR_BIT(PORTB, PIN2)   /**< set GPIO pin low for signal DATA */
#define TOGGLE_DATA    TOGGLE_BIT(PORTB, PIN2)   /**< toggle GPIO pin for signal DATA */
#define READ_DATA    (PINB & PINMASK2)   /**< read GPIO pin for signal DATA */
#define CLOCK_PORT    PORTA   /**< GPIO port for signal CLOCK */
#define CLOCK_PIN    PINMASK7   /**< GPIO pin bitmask for signal CLOCK */
#define CLOCK_AS_OUTPUT    SET_BIT(DDRA, PIN7)   /**< set GPIO as push/pull output for signal CLOCK */
#define CLOCK_AS_INPUT    CLEAR_BIT(DDRA, PIN7); SET_BIT(PORTA, PIN7)   /**< set GPIO as input with pullup for signal CLOCK */
#define CLOCK_AS_HIGHZ    CLEAR_BIT(DDRA, PIN7); CLEAR_BIT(PORTA, PIN7)   /**< set GPIO as input High-Z without pullup for signal CLOCK */
#define SET_CLOCK    SET_BIT(PORTA, PIN7)   /**< set GPIO pin high for signal CLOCK */
#define CLEAR_CLOCK    CLEAR_BIT(PORTA, PIN7)   /**< set GPIO pin low for signal CLOCK */
#define TOGGLE_CLOCK    TOGGLE_BIT(PORTA, PIN7)   /**< toggle GPIO pin for signal CLOCK */
#define READ_CLOCK    (PINA & PINMASK7)   /**< read GPIO pin for signal CLOCK */
#define LATCH_PORT    PORTA   /**< GPIO port for signal LATCH */
#define LATCH_PIN    PINMASK6   /**< GPIO pin bitmask for signal LATCH */
#define LATCH_AS_OUTPUT    SET_BIT(DDRA, PIN6)   /**< set GPIO as push/pull output for signal LATCH */
#define LATCH_AS_INPUT    CLEAR_BIT(DDRA, PIN6); SET_BIT(PORTA, PIN6)   /**< set GPIO as input with pullup for signal LATCH */
#define LATCH_AS_HIGHZ    CLEAR_BIT(DDRA, PIN6); CLEAR_BIT(PORTA, PIN6)   /**< set GPIO as input High-Z without pullup for signal LATCH */
#define SET_LATCH    SET_BIT(PORTA, PIN6)   /**< set GPIO pin high for signal LATCH */
#define CLEAR_LATCH    CLEAR_BIT(PORTA, PIN6)   /**< set GPIO pin low for signal LATCH */
#define TOGGLE_LATCH    TOGGLE_BIT(PORTA, PIN6)   /**< toggle GPIO pin for signal LATCH */
#define READ_LATCH    (PINA & PINMASK6)   /**< read GPIO pin for signal LATCH */
#define DB9UP_PORT    PORTA   /**< GPIO port for signal DB9UP */
#define DB9UP_PIN    PINMASK5   /**< GPIO pin bitmask for signal DB9UP */
#define DB9UP_AS_OUTPUT    SET_BIT(DDRA, PIN5)   /**< set GPIO as push/pull output for signal DB9UP */
#define DB9UP_AS_INPUT    CLEAR_BIT(DDRA, PIN5); SET_BIT(PORTA, PIN5)   /**< set GPIO as input with pullup for signal DB9UP */
#define DB9UP_AS_HIGHZ    CLEAR_BIT(DDRA, PIN5); CLEAR_BIT(PORTA, PIN5)   /**< set GPIO as input High-Z without pullup for signal DB9UP */
#define SET_DB9UP    SET_BIT(PORTA, PIN5)   /**< set GPIO pin high for signal DB9UP */
#define CLEAR_DB9UP    CLEAR_BIT(PORTA, PIN5)   /**< set GPIO pin low for signal DB9UP */
#define TOGGLE_DB9UP    TOGGLE_BIT(PORTA, PIN5)   /**< toggle GPIO pin for signal DB9UP */
#define READ_DB9UP    (PINA & PINMASK5)   /**< read GPIO pin for signal DB9UP */
#define DB9DOWN_PORT    PORTA   /**< GPIO port for signal DB9DOWN */
#define DB9DOWN_PIN    PINMASK4   /**< GPIO pin bitmask for signal DB9DOWN */
#define DB9DOWN_AS_OUTPUT    SET_BIT(DDRA, PIN4)   /**< set GPIO as push/pull output for signal DB9DOWN */
#define DB9DOWN_AS_INPUT    CLEAR_BIT(DDRA, PIN4); SET_BIT(PORTA, PIN4)   /**< set GPIO as input with pullup for signal DB9DOWN */
#define DB9DOWN_AS_HIGHZ    CLEAR_BIT(DDRA, PIN4); CLEAR_BIT(PORTA, PIN4)   /**< set GPIO as input High-Z without pullup for signal DB9DOWN */
#define SET_DB9DOWN    SET_BIT(PORTA, PIN4)   /**< set GPIO pin high for signal DB9DOWN */
#define CLEAR_DB9DOWN    CLEAR_BIT(PORTA, PIN4)   /**< set GPIO pin low for signal DB9DOWN */
#define TOGGLE_DB9DOWN    TOGGLE_BIT(PORTA, PIN4)   /**< toggle GPIO pin for signal DB9DOWN */
#define READ_DB9DOWN    (PINA & PINMASK4)   /**< read GPIO pin for signal DB9DOWN */
#define DB9LEFT_PORT    PORTA   /**< GPIO port for signal DB9LEFT */
#define DB9LEFT_PIN    PINMASK3   /**< GPIO pin bitmask for signal DB9LEFT */
#define DB9LEFT_AS_OUTPUT    SET_BIT(DDRA, PIN3)   /**< set GPIO as push/pull output for signal DB9LEFT */
#define DB9LEFT_AS_INPUT    CLEAR_BIT(DDRA, PIN3); SET_BIT(PORTA, PIN3)   /**< set GPIO as input with pullup for signal DB9LEFT */
#define DB9LEFT_AS_HIGHZ    CLEAR_BIT(DDRA, PIN3); CLEAR_BIT(PORTA, PIN3)   /**< set GPIO as input High-Z without pullup for signal DB9LEFT */
#define SET_DB9LEFT    SET_BIT(PORTA, PIN3)   /**< set GPIO pin high for signal DB9LEFT */
#define CLEAR_DB9LEFT    CLEAR_BIT(PORTA, PIN3)   /**< set GPIO pin low for signal DB9LEFT */
#define TOGGLE_DB9LEFT    TOGGLE_BIT(PORTA, PIN3)   /**< toggle GPIO pin for signal DB9LEFT */
#define READ_DB9LEFT    (PINA & PINMASK3)   /**< read GPIO pin for signal DB9LEFT */
#define DB9RIGHT_PORT    PORTA   /**< GPIO port for signal DB9RIGHT */
#define DB9RIGHT_PIN    PINMASK2   /**< GPIO pin bitmask for signal DB9RIGHT */
#define DB9RIGHT_AS_OUTPUT    SET_BIT(DDRA, PIN2)   /**< set GPIO as push/pull output for signal DB9RIGHT */
#define DB9RIGHT_AS_INPUT    CLEAR_BIT(DDRA, PIN2); SET_BIT(PORTA, PIN2)   /**< set GPIO as input with pullup for signal DB9RIGHT */
#define DB9RIGHT_AS_HIGHZ    CLEAR_BIT(DDRA, PIN2); CLEAR_BIT(PORTA, PIN2)   /**< set GPIO as input High-Z without pullup for signal DB9RIGHT */
#define SET_DB9RIGHT    SET_BIT(PORTA, PIN2)   /**< set GPIO pin high for signal DB9RIGHT */
#define CLEAR_DB9RIGHT    CLEAR_BIT(PORTA, PIN2)   /**< set GPIO pin low for signal DB9RIGHT */
#define TOGGLE_DB9RIGHT    TOGGLE_BIT(PORTA, PIN2)   /**< toggle GPIO pin for signal DB9RIGHT */
#define READ_DB9RIGHT    (PINA & PINMASK2)   /**< read GPIO pin for signal DB9RIGHT */
#define DB9RIGHT_PORT    PORTA   /**< GPIO port for signal DB9RIGHT */
#define DB9RIGHT_PIN    PINMASK1   /**< GPIO pin bitmask for signal DB9RIGHT */
#define DB9RIGHT_AS_OUTPUT    SET_BIT(DDRA, PIN1)   /**< set GPIO as push/pull output for signal DB9RIGHT */
#define DB9RIGHT_AS_INPUT    CLEAR_BIT(DDRA, PIN1); SET_BIT(PORTA, PIN1)   /**< set GPIO as input with pullup for signal DB9RIGHT */
#define DB9RIGHT_AS_HIGHZ    CLEAR_BIT(DDRA, PIN1); CLEAR_BIT(PORTA, PIN1)   /**< set GPIO as input High-Z without pullup for signal DB9RIGHT */
#define SET_DB9RIGHT    SET_BIT(PORTA, PIN1)   /**< set GPIO pin high for signal DB9RIGHT */
#define CLEAR_DB9RIGHT    CLEAR_BIT(PORTA, PIN1)   /**< set GPIO pin low for signal DB9RIGHT */
#define TOGGLE_DB9RIGHT    TOGGLE_BIT(PORTA, PIN1)   /**< toggle GPIO pin for signal DB9RIGHT */
#define READ_DB9RIGHT    (PINA & PINMASK1)   /**< read GPIO pin for signal DB9RIGHT */
#define UNUSED_A0_PORT    PORTA   /**< GPIO port for signal UNUSED_A0 */
#define UNUSED_A0_PIN    PINMASK0   /**< GPIO pin bitmask for signal UNUSED_A0 */
#define UNUSED_A0_AS_OUTPUT    SET_BIT(DDRA, PIN0)   /**< set GPIO as push/pull output for signal UNUSED_A0 */
#define UNUSED_A0_AS_INPUT    CLEAR_BIT(DDRA, PIN0); SET_BIT(PORTA, PIN0)   /**< set GPIO as input with pullup for signal UNUSED_A0 */
#define UNUSED_A0_AS_HIGHZ    CLEAR_BIT(DDRA, PIN0); CLEAR_BIT(PORTA, PIN0)   /**< set GPIO as input High-Z without pullup for signal UNUSED_A0 */
#define SET_UNUSED_A0    SET_BIT(PORTA, PIN0)   /**< set GPIO pin high for signal UNUSED_A0 */
#define CLEAR_UNUSED_A0    CLEAR_BIT(PORTA, PIN0)   /**< set GPIO pin low for signal UNUSED_A0 */
#define TOGGLE_UNUSED_A0    TOGGLE_BIT(PORTA, PIN0)   /**< toggle GPIO pin for signal UNUSED_A0 */
#define READ_UNUSED_A0    (PINA & PINMASK0)   /**< read GPIO pin for signal UNUSED_A0 */
#define UNUSED_B0_PORT    PORTB   /**< GPIO port for signal UNUSED_B0 */
#define UNUSED_B0_PIN    PINMASK0   /**< GPIO pin bitmask for signal UNUSED_B0 */
#define UNUSED_B0_AS_OUTPUT    SET_BIT(DDRB, PIN0)   /**< set GPIO as push/pull output for signal UNUSED_B0 */
#define UNUSED_B0_AS_INPUT    CLEAR_BIT(DDRB, PIN0); SET_BIT(PORTB, PIN0)   /**< set GPIO as input with pullup for signal UNUSED_B0 */
#define UNUSED_B0_AS_HIGHZ    CLEAR_BIT(DDRB, PIN0); CLEAR_BIT(PORTB, PIN0)   /**< set GPIO as input High-Z without pullup for signal UNUSED_B0 */
#define SET_UNUSED_B0    SET_BIT(PORTB, PIN0)   /**< set GPIO pin high for signal UNUSED_B0 */
#define CLEAR_UNUSED_B0    CLEAR_BIT(PORTB, PIN0)   /**< set GPIO pin low for signal UNUSED_B0 */
#define TOGGLE_UNUSED_B0    TOGGLE_BIT(PORTB, PIN0)   /**< toggle GPIO pin for signal UNUSED_B0 */
#define READ_UNUSED_B0    (PINB & PINMASK0)   /**< read GPIO pin for signal UNUSED_B0 */
#define UNUSED_B1_PORT    PORTB   /**< GPIO port for signal UNUSED_B1 */
#define UNUSED_B1_PIN    PINMASK0   /**< GPIO pin bitmask for signal UNUSED_B1 */
#define UNUSED_B1_AS_OUTPUT    SET_BIT(DDRB, PIN0)   /**< set GPIO as push/pull output for signal UNUSED_B1 */
#define UNUSED_B1_AS_INPUT    CLEAR_BIT(DDRB, PIN0); SET_BIT(PORTB, PIN0)   /**< set GPIO as input with pullup for signal UNUSED_B1 */
#define UNUSED_B1_AS_HIGHZ    CLEAR_BIT(DDRB, PIN0); CLEAR_BIT(PORTB, PIN0)   /**< set GPIO as input High-Z without pullup for signal UNUSED_B1 */
#define SET_UNUSED_B1    SET_BIT(PORTB, PIN0)   /**< set GPIO pin high for signal UNUSED_B1 */
#define CLEAR_UNUSED_B1    CLEAR_BIT(PORTB, PIN0)   /**< set GPIO pin low for signal UNUSED_B1 */
#define TOGGLE_UNUSED_B1    TOGGLE_BIT(PORTB, PIN0)   /**< toggle GPIO pin for signal UNUSED_B1 */
#define READ_UNUSED_B1    (PINB & PINMASK0)   /**< read GPIO pin for signal UNUSED_B1 */
#define UNUSED_B3_PORT    PORTB   /**< GPIO port for signal UNUSED_B3 */
#define UNUSED_B3_PIN    PINMASK3   /**< GPIO pin bitmask for signal UNUSED_B3 */
#define UNUSED_B3_AS_OUTPUT    SET_BIT(DDRB, PIN3)   /**< set GPIO as push/pull output for signal UNUSED_B3 */
#define UNUSED_B3_AS_INPUT    CLEAR_BIT(DDRB, PIN3); SET_BIT(PORTB, PIN3)   /**< set GPIO as input with pullup for signal UNUSED_B3 */
#define UNUSED_B3_AS_HIGHZ    CLEAR_BIT(DDRB, PIN3); CLEAR_BIT(PORTB, PIN3)   /**< set GPIO as input High-Z without pullup for signal UNUSED_B3 */
#define SET_UNUSED_B3    SET_BIT(PORTB, PIN3)   /**< set GPIO pin high for signal UNUSED_B3 */
#define CLEAR_UNUSED_B3    CLEAR_BIT(PORTB, PIN3)   /**< set GPIO pin low for signal UNUSED_B3 */
#define TOGGLE_UNUSED_B3    TOGGLE_BIT(PORTB, PIN3)   /**< toggle GPIO pin for signal UNUSED_B3 */
#define READ_UNUSED_B3    (PINB & PINMASK3)   /**< read GPIO pin for signal UNUSED_B3 */

/* port inits: */
#define INIT_PORTA  190  /**< ... */
#define INIT_PORTB  4  /**< ... */

/* data direction inits: */
#define INIT_DDRA  254  /**< ... */

/* function prototypes: */
void InitPorts(void);
#endif

