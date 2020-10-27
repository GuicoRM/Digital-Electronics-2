/*
 * Proyecto5_1DISP_LUT.c
 *
 * Created: 26/10/2020 23:18:15
 * Author : Guillermo Cortés
 */ 

/*
 * Proyecto5_DC_LUT.c
 *
 * Created: 26/10/2020 19:05:31
 * Author : Guillermo Cortés
 */ 

/***********************************************************************
 * 
 * Decimal counter with 7-segment output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) Guillermo Cortés
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC

/* Variables ---------------------------------------------------------*/
uint8_t cnt0 = 0;			// Decimal counter value for position '0''

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Display decimal 
 * counter values on SSD (Seven-segment display) when 16-bit 
 * Timer/Counter1 overflows.
 */
int main(void)
{
    // Configure SSD signals
    SEG_init();
	
	// Iniciamos el display con el valor '0'
	SEG_update_shift_regs(cnt0,0);
	
    /* Configure 16-bit Timer/Counter1 and Timer/Counter0
     * Set prescaler and enable overflow interrupt */
	TIM1_overflow_1s();		// We will use this Timer to increment the value of our segment
	TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
	sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter1 overflows. Increment decimal counter
 * value and display it on SSD.
 */
ISR(TIMER1_OVF_vect){    
	
	cnt0++;									// We increment the counter each 1 sec 
	
	if(cnt0 >= 10){							// When we reach the value '9' in first digit, we restart it and increment the second digit		
		cnt0 = 0;				
	}
	
	SEG_update_shift_regs(cnt0,0);
	
}

