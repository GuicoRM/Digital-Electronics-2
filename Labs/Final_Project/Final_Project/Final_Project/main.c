/*
 * Final_Project.c
 *
 * Created: 26/11/2020 22:23:47
 * Author : Guillermo Cortés Orellana
 */ 

/* Defines -----------------------------------------------------------*/
#define OUTPUT_MICRO   PB5  // AVR pin where green LED is connected


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions (IN/OUT)
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

int main(void)
{
	/* CONFIGURATION OF PIN PB5 -> OUTPUT_MICRO */	
	GPIO_config_output(&DDRB, OUTPUT_MICRO);		// Set pin as OUTPUT in Data Direction Register... (PIN -> OUTPUT)
	GPIO_write_low(&PORTB, OUTPUT_MICRO);			// OUTPUT = '0'
	
	/* Configuration of TIMER */
	
	/*********************************TIMER/COUNTER1**************************************/
	/* Configuration of 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
    TIM1_overflow_1s();
    TIM1_overflow_interrupt_enable();
	
	// Enables interrupts by setting the global interrupt mask
	sei();
	
    while (1) 
    {
		 /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter1 overflows. Switch OUTPUT_MICRO 
 * Multi-function shield. */
ISR(TIMER1_OVF_vect)
{
    GPIO_toggle(&PORTB, OUTPUT_MICRO);				// Switch (blink)
}

