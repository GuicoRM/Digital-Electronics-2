/*
 * Final_Project.c
 *
 * Created: 26/11/2020 22:23:47
 * Author : Guillermo Cortés Orellana
 */ 

/* Defines -----------------------------------------------------------*/
#define OUTPUT_MICRO   PB5  // AVR pin where green LED is connected
#define BLINK_DELAY 500
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions (IN/OUT)
#include <util/delay.h>     // Functions for busy-wait delay loops (STOPS)
#include "gpio.h"           // GPIO library for AVR-GCC

int main(void)
{
	/* PIN PB5 */
	// Set pin as OUTPUT in Data Direction Register... (PIN -> OUTPUT)
	GPIO_config_output(&DDRB, OUTPUT_MICRO);
	GPIO_write_low(&PORTB, OUTPUT_MICRO); // OUTPUT = '0'
	
    while (1) 
    {
		 // Pause several milliseconds
		 _delay_ms(BLINK_DELAY);

		 // Invert values (BLINK)
		 GPIO_toggle(&PORTB, OUTPUT_MICRO); // Switch (blink)
    }
}

