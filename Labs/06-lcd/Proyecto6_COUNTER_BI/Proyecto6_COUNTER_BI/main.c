/*
 * Proyecto6_COUNTER_BI.c
 *
 * Created: 06/11/2020 21:09:49
 * Author : Guillermo Cortés
 */ 

/***********************************************************************
 * 
 * Stopwatch with LCD display output.
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
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Variables ----------------------------------------------------------*/
uint8_t customChar[8] = {
	0b10101,
	0b01010,
	0b10101,
	0b01010,
	0b10101,
	0b01010,
	0b10101,
	0b01010
};

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows. It also
 * shows bar state Timer/Counter2 overflows and one custom characters.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

    // Put string(s) at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("00:00.0");
	lcd_gotoxy(11, 0);
	lcd_putc('0');
	lcd_gotoxy(1, 1);
	lcd_putc(' ');
	
	/******************* Custome character ******************************/
	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 8; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
	// Position where we want to display
	lcd_gotoxy(11,1);
	
	// Display first custom character
	lcd_putc(0);
	/********************************************************************/

    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Enable interrupt and set the overflow prescaler to 16 ms
    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
	
	 // Configure 8-bit Timer/Counter2 for Stopwatch
	 // Enable interrupt and set the overflow prescaler to 16 ms
	 TIM0_overflow_16ms();
	 TIM0_overflow_interrupt_enable();

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
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
	static uint8_t tens = 0;		// Tenths of a second
    static uint8_t secs = 0;        // Seconds
	static uint8_t mins = 0;        // Minutes
	
	char lcd_string[2] = " ";
	char lcd_sqr[2] = " ";

    number_of_overflows++;
	
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;

        /*TENTHS*/
		itoa(tens, lcd_string, 10);     // Convert decimal value to string
		lcd_gotoxy(7, 0);
		lcd_puts(lcd_string);
		
		tens++;
		
		if(tens >= 10){
			tens = 0;
						
			secs++;
			
			/*SQUARE OF SECONDS*/
			itoa(secs*secs, lcd_sqr, 10);     // Convert decimal value to string
			lcd_gotoxy(11, 0);
			lcd_puts(lcd_sqr);	
			
			/*SECONDS*/
			if (secs >= 10){
				itoa(secs, lcd_string, 10);     // Convert decimal value to string 
				lcd_gotoxy(4, 0);
				lcd_puts(lcd_string); 
				
			}else{
				itoa(secs, lcd_string, 10);     // Convert decimal value to string 
				lcd_gotoxy(5, 0);
				lcd_puts(lcd_string); 
				
			}
			
			/*MINUTES*/
			if (secs >= 60){
				secs = 0;
				itoa(secs, lcd_string, 10);     // Convert decimal value to string 
				lcd_gotoxy(4, 0);
				lcd_puts(lcd_string);
				
				itoa(secs*secs, lcd_sqr, 10);     // Convert decimal value to string
				lcd_gotoxy(11, 0);
				lcd_puts(lcd_sqr);
				
				lcd_gotoxy(12, 0);
				lcd_data(0x20);
				lcd_gotoxy(13, 0);
				lcd_data(0x20);
				lcd_gotoxy(14, 0);
				lcd_data(0x20);				 
								
				mins++;	
				if (mins >= 60){ 
					mins = 0;
					
					lcd_gotoxy(1, 0);
					lcd_putc('0');
					lcd_gotoxy(2, 0);
					lcd_putc('0');					
					
				}else if (mins >= 10){
					itoa(mins, lcd_string, 10);     // Convert decimal value to string
					lcd_gotoxy(1, 0);
					lcd_puts(lcd_string);
				}else {
					itoa(mins, lcd_string, 10);     // Convert decimal value to string
					lcd_gotoxy(2, 0);
					lcd_puts(lcd_string);
				}
				
			}
			
		}		
		
    }
}

/**
 * ISR starts when Timer/Counter0 overflows. Shows
 * bar state, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER0_OVF_vect)
{
	static uint8_t symbol = 0;
	static uint8_t position = 1;
	
	symbol++;		

	if(symbol >= 6){
		
		symbol = 0;
		
		if(position == 11){
			
			lcd_gotoxy(0, 1);
			lcd_data(0x20);
			lcd_gotoxy(1, 1);
			lcd_data(0x20);
			lcd_gotoxy(2, 1);
			lcd_data(0x20);
			lcd_gotoxy(3, 1);
			lcd_data(0x20);
			lcd_gotoxy(4, 1);
			lcd_data(0x20);
			lcd_gotoxy(5, 1);
			lcd_data(0x20);
			lcd_gotoxy(6, 1);
			lcd_data(0x20);
			lcd_gotoxy(7, 1);
			lcd_data(0x20);
			lcd_gotoxy(8, 1);
			lcd_data(0x20);
			lcd_gotoxy(9, 1);
			lcd_data(0x20);
			lcd_gotoxy(10, 1);
			lcd_data(0x20);			

		}
		
		if(position < 11){
			lcd_gotoxy(position, 1);
			lcd_data(0xff);
		}else{
			position = 1;
					
			lcd_gotoxy(position, 1);
			lcd_data(0xff);
		}
		position++;
	}
	
}
