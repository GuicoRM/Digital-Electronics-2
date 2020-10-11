/*
 * Proyecto3_Puls_2LEDS.c
 *
 * Created: 10/10/2020 14:42:26
 * Author : Guillermo Cort�s Orellana
 */ 

/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed. Use 
 * functions from GPIO library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) Guillermo Cort�s 
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define LED_BLUE    PC0     // AVR pin where blue LED is connected
#define BTN         PD0    // AVR pin where blue PUSH BUTTON is connected
#define BLINK_DELAY 500    
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include "gpio.h"           // GPIO library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle two LEDs 
 * when a push button is pressed. Functions from user-defined GPIO
 * library is used instead of low-level logic operations.
 */
int main(void)
{
    /* GREEN LED */
    GPIO_config_output(&DDRB, LED_GREEN);
    GPIO_write_low(&PORTB, LED_GREEN); // LED off because active HIGH

    /* BLUE LED */
    GPIO_config_output(&DDRC, LED_BLUE);
    GPIO_write_high(&PORTC, LED_BLUE); // LED off because active LOW

    /* push button */
    GPIO_config_input_pullup(&DDRD, BTN); // Configure DDRx and PORTx in 'pullup mode'

    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(BLINK_DELAY);

		if (GPIO_read(&PIND, BTN) == 1){ // If we push the button (0), I enter de 'if'
			
			GPIO_toggle(&PORTB, LED_GREEN); // Switch (blink)
			GPIO_toggle(&PORTC, LED_BLUE); // Switch (blink)
		}
    }

    // Will never reach this
    return 0;
}

