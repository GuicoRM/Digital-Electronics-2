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
uint8_t cnt0 = 0;			// Decimal counter value for position '0'
uint8_t cnt1 = 0;			// Decimal counter value for position '1'

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
	
    /* Configure 16-bit Timer/Counter1 and Timer/Counter0
     * Set prescaler and enable overflow interrupt */
	TIM0_overflow_4ms();	// We will use this Timer to switch between display '0' and display '1'
	TIM0_overflow_interrupt_enable();
	
	TIM1_overflow_1s();		// We will use this Timer to increment the value of our Decimal Counter
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
ISR(TIMER0_OVF_vect)
{
	static uint8_t pos = 0;
											
	if (pos == 0){							  
		SEG_update_shift_regs(cnt0,pos);	// We display the value of 'cont0' in position '0'
		pos++;								// We increment the position in order to use this value to display in position '1'
	}else{
		SEG_update_shift_regs(cnt1,pos);	// We display the value of 'cont1' in position '1'
		pos--;								// We decrement the position in order to use this value to display in position '0'
	}
	
}

ISR(TIMER1_OVF_vect){    
	
	cnt0++;									// We increment the counter each 1 sec 
	
	if(cnt0 >= 10){							// When we reach the value '9' in first digit, we restart it and increment the second digit		
		cnt0 = 0;
		cnt1++;						
	}
	
	if(cnt1>=6){							// When we reach the value '5' in second digit, we restart it
		cnt1 = 0;
	}
	
}
