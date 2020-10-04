
/***********************************************************************
 * 
 * Proyecto2_Puls_5LEDS.c
 
 * Program that will --after you press the button-- ensure that only one of LED is switched on at a time in Knight Rider style
 *
 * Copyright (c) Guillermo Cortés
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_RED_1   PC0     // AVR pin where green LED1 is connected
#define LED_RED_2   PB5     // AVR pin where green LED2 is connected
#define LED_RED_3   PB4     // AVR pin where green LED3 is connected
#define LED_RED_4   PB3     // AVR pin where green LED4 is connected
#define LED_RED_5   PB2     // AVR pin where green LED5 is connected

#define BTN         PD0    // AVR pin where blue PUSH BUTTON is connected

#define BLINK_DELAY 300
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops (PAUSAS)
#include <avr/io.h>         // AVR device-specific IO definitions (ENTRADA/SALIDA)

/* Functions ---------------------------------------------------------*/
/**
 * Main function where the program execution begins. One LED is switched on at a time in Knight Rider style
 */

int main(void)
{
    /* RED LED1 */
    // Set pin as OUTPUT in Data Direction Register...(PIN COMO SALIDA)
    DDRC = DDRC | (1<<LED_RED_1);
    // ...and turn LED off in Data Register (Inicialmente APAGADO)
    PORTC = PORTC | (1<<LED_RED_1);	
	
    /* RED LED2 */
	// Set pin as OUTPUT in Data Direction Register... (PIN COMO SALIDA)
    DDRB = DDRB | (1<<LED_RED_2);
    // ...and turn LED off in Data Register (Inicialmente APAGADO)
    PORTB = PORTB & ~(1<<LED_RED_2);
	
	/* RED LED2 */
	// Set pin as OUTPUT in Data Direction Register... (PIN COMO SALIDA)
	DDRB = DDRB | (1<<LED_RED_3);
	// ...and turn LED off in Data Register (Inicialmente APAGADO)
	PORTB = PORTB & ~(1<<LED_RED_3);
	
	/* RED LED4 */
	// Set pin as OUTPUT in Data Direction Register... (PIN COMO SALIDA)
	DDRB = DDRB | (1<<LED_RED_4);
	// ...and turn LED off in Data Register (Inicialmente APAGADO)
	PORTB = PORTB & ~(1<<LED_RED_4);
	
	/* RED LED5 */
	// Set pin as OUTPUT in Data Direction Register... (PIN COMO SALIDA)
	DDRB = DDRB | (1<<LED_RED_5);
	// ...and turn LED off in Data Register (Inicialmente APAGADO)
	PORTB = PORTB & ~(1<<LED_RED_5);
	
    /* PUSH BUTTON */ // ACTIVO A NIVEL BAJO
    DDRD = DDRD &~ (1<<BTN); // Define as an Input (PIN COMO ENTRADA)
	PORTD = PORTD | (1<<BTN);

    // Infinite loop
    while (1)
    {
        if(bit_is_clear(PIND,BTN)){ // if 'PUSH' -> Start Knight sequency (Evaluamos el registro del bit del pulsador -> Si hay un '0' (ACTIVO BAJO), entro en el 'if')
			
			// Start Knight Rider sequency			
			PORTC = PORTC ^ (1<<LED_RED_1);
			_delay_ms(BLINK_DELAY);
			PORTC = PORTC ^ (1<<LED_RED_1);
			PORTB = PORTB ^ (1<<LED_RED_2);
			_delay_ms(BLINK_DELAY);
			
			PORTB = PORTB ^ (1<<LED_RED_2);
			PORTB = PORTB ^ (1<<LED_RED_3);
			_delay_ms(BLINK_DELAY);
			
			PORTB = PORTB ^ (1<<LED_RED_3);
			PORTB = PORTB ^ (1<<LED_RED_4);
			_delay_ms(BLINK_DELAY);
			
			PORTB = PORTB ^ (1<<LED_RED_4);
			PORTB = PORTB ^ (1<<LED_RED_5);
			_delay_ms(BLINK_DELAY);
			
			PORTB = PORTB ^ (1<<LED_RED_5);
			PORTB = PORTB ^ (1<<LED_RED_4);
			_delay_ms(BLINK_DELAY);
			
			PORTB = PORTB ^ (1<<LED_RED_4);
			PORTB = PORTB ^ (1<<LED_RED_3);
			_delay_ms(BLINK_DELAY);
			
			PORTB = PORTB ^ (1<<LED_RED_3);
			PORTB = PORTB ^ (1<<LED_RED_2);
			_delay_ms(BLINK_DELAY);
			
			PORTB = PORTB ^ (1<<LED_RED_2);			
			
		}
		
    }

    // Will never reach this
    return 0;
}

/************************************************************************/
/* NOTAS

LED_RED_1 ACTIVO A NIVEL BAJO
LED_RED_2 A LED_RED_5 ACTIVOS A NIVEL ALTO

LOS LEDS SON ROJOS PARA SIMULAR EL EFECTO DEL COCHE FANTÁSTICO                                                                     */
/************************************************************************/