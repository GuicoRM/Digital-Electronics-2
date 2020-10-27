/*
 * Proyecto5_SNAKE.c
 *
 * Created: 26/10/2020 23:23:41
 * Author : Guillermo Cort�s
 */ 

/***********************************************************************
 * 
 * Decimal counter with 7-segment output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) Guillermo Cort�s
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
uint8_t snk = 0;			// Position in the segment where the snake is located
uint8_t position = 3;		// Segment where is the snake located

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
	
	// Beginning of the SNAKE
	SEG_update_shift_regs(snk,position);
	
    /* Configure 16-bit Timer/Counter1 and Timer/Counter0
     * Set prescaler and enable overflow interrupt */
	
	TIM1_overflow_262ms();				// We will use this Timer to 'move' the snake
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
	
	// We move the SNAKE each 262 ms 
	
	if(((snk == 0) & (position == 3)) | ((snk == 0) & (position == 2)) | ((snk == 0) & (position == 1))){	
		SEG_update_shift_regs(snk,position);			
		position--;					
	}else if (((snk == 0) & (position == 0)) | ((snk == 1) & (position == 0)) | ((snk == 2) & (position == 0))){
		SEG_update_shift_regs(snk,position);
		snk++;
	}else if (((snk == 3) & (position == 0)) | ((snk == 3) & (position == 1)) | ((snk == 3) & (position == 2))){
		SEG_update_shift_regs(snk,position);
		position++;
	}else if (((snk == 3) & (position == 3)) | ((snk == 4) & (position == 3))){
		SEG_update_shift_regs(snk,position);
		snk++;
	}else{
		SEG_update_shift_regs(snk,position);
		snk = 0;
	}
	
}

