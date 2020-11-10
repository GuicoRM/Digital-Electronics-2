/*
 * Proyecto6_COUNTER_BA.c
 *
 * Created: 06/11/2020 18:36:13
 * Author : Guillermo Cortés
 */ 

/***********************************************************************
 * 
 * Stopwatch with LCD display output, progress bar and custom character.
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

/* Defines -----------------------------------------------------------*/
#define COL1 1
#define COL2 11

/* Variables ---------------------------------------------------------*/
/*Codifiction of |, ||, |||, ||||, |||||. It will be used in our progress bar*/
uint8_t customChar[6*8] = {
	 // addr 0: .....
	0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
	 // addr 1: |....
	0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
	 // addr 2: ||...
	0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
	 // addr 3: |||..
	0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
	 // addr 4: ||||.
	0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110,
	 // addr 5: |||||
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111
};

/* Codification of one 'chessboard' custom character*/
uint8_t customChar2[8] = {
	0b10101, 0b01010, 0b10101, 0b01010, 0b10101, 0b01010, 0b10101, 0b01010
};

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows. It also
 * shows bar state using Timer/Counter2 overflows.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

    // Put string(s) at LCD display
    lcd_gotoxy(COL1, 0);
    lcd_puts("00:00.0");
	lcd_gotoxy(COL2, 0);
	lcd_putc('0');
	lcd_gotoxy(COL1, 1);
	lcd_putc(' ');
	
	/******************* Custome characters ******************************/
	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	
	/*The codification of |, ||, |||, ||||, ||||| will be store in position 0, 1, 2, 3, 4 and 5 of CGRAM memory respectively*/
	for (uint8_t i = 0; i < 6*8; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	
	/*The codification of 'chessboard' character will be store in position 6 of CGRAM memory*/
	for (uint8_t j = 0; j < 8; j++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar2[j]);
	}
	
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
	// Position where we want to display
	lcd_gotoxy(COL2,1);
	
	// Display 'chessboard' custom character
	lcd_putc(6);
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
	static uint8_t tens = 1;		// Tenths of a second
	static uint8_t secs = 0;        // Seconds
	static uint8_t mins = 0;        // Minutes
	
	char lcd_string[2] = " ";
	char lcd_sqr[2] = " ";

	number_of_overflows++;
	
	if (number_of_overflows >= 6)
	{
		// Do this every 6 x 16 ms = 100 ms
		number_of_overflows = 0;
		
		if(tens >= 10){
			
			tens = 0;
			
			secs++;
			
			/*SQUARE OF SECONDS*/
			itoa(secs*secs, lcd_sqr, 10);     // Convert decimal value to string
			lcd_gotoxy(COL2, 0);
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
				lcd_gotoxy(COL2, 0);
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
					
					lcd_gotoxy(COL1, 0);
					lcd_putc('0');
					lcd_gotoxy(2, 0);
					lcd_putc('0');
					
				}else if (mins >= 10){
					itoa(mins, lcd_string, 10);     // Convert decimal value to string
					lcd_gotoxy(COL1, 0);
					lcd_puts(lcd_string);
					
				}else {
					itoa(mins, lcd_string, 10);     // Convert decimal value to string
					lcd_gotoxy(2, 0);
					lcd_puts(lcd_string);
				}
				
			}
			
		}
		
		/*TENTHS*/
		itoa(tens, lcd_string, 10);     // Convert decimal value to string
		lcd_gotoxy(7, 0);
		lcd_puts(lcd_string);
		
		tens++;
		
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
	static uint8_t position = 0;
	
	lcd_gotoxy(COL1 + position, 1);
	lcd_putc(symbol);
	
	symbol++;	
	
	if(symbol >= 6){
		symbol=0;
		position++;
		
		if (position>=10)
		{
			position=0;
			lcd_gotoxy(COL1, 1);
			lcd_puts("          ");
		}
	}	
}
