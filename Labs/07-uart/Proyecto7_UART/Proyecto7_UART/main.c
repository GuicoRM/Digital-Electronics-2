/*
 * Proyecto7_UART.c
 *
 * Created: 17/11/2020 20:40:13
 * Author : guico
 */ 

/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
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
#include "uart.h"           // Peter Fleury's UART library
#ifndef F_CPU
#define F_CPU 16000000
#endif

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");    // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");    // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("None");    // Put button name here

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
	ADMUX |= (1 << REFS0);

    // Set input channel to ADC0
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));

    // Enable ADC module
	ADCSRA |= (1 << ADEN);

    // Enable conversion complete interrupt
	ADCSRA |= (1 << ADIE);

    // Set clock prescaler to 128
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();

    // Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600, F_CPU));

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
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
	ADCSRA |= (1 << ADSC);

}

/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. Display value on LCD
 * and send it to UART.
 */
ISR(ADC_vect)
{
    uint16_t value = 0;
	char lcd_string[4] = "0000";	// Maximum value is 1023 -> We need 4 character in DEC

	value = ADC;					// Copy ADC register result to 16-bit variable

	itoa (value, lcd_string, 10);	// Convert DEC value to string
	lcd_gotoxy(8,0);				// Display in position 'a'
	lcd_puts("    ");				// Clear position 'a'
	lcd_gotoxy(8,0);				// Display in position 'a'
	lcd_puts(lcd_string);

	/*We use this condition in order to avoid display continuously value 
	  in UART communication (when we don't push any button)*/
	if(value < 700){				
		uart_puts("ADC value in DEC: ");
		uart_puts(lcd_string);
		uart_puts("\r\n");
	}

	itoa (value, lcd_string, 16);	// Convert HEX value to string
	lcd_gotoxy(13,0);				// Display in position 'b'
	lcd_puts("    ");				// Clear position 'b'
	lcd_gotoxy(13,0);				// Display in position 'b'
	lcd_puts(lcd_string);

	/*We use this condition in order to avoid display continuously value 
	  in UART communication (when we don't push any button)*/
	if(value < 700){
		uart_puts("ADC value in HEX: ");
		uart_puts(lcd_string);
		uart_puts("\r\n");
	}

	/**
	 * 1. We will display in position 'c' of LCD which button is pushed.
	 *    AVR ADC module has 10-bit resolution with +/-2LSB accuracy
	 *
	 * 2. We will use UART communication
	 */

	lcd_gotoxy(8, 1);				// Display in position 'c'
	lcd_puts("      ");				// Clear position 'c'
	lcd_gotoxy(8, 1);				// Display in position 'c'

	/*We use this condition in order to avoid display continuously value 
	  in UART communication (when we don't push any button)*/	
	if(value < 700){
		if(value <= 2){

			// Right
			lcd_puts("Right");
			uart_puts("Button: Right");
			uart_puts("\r\n");

		} else if ((value >= 99)&(value <= 103)){

			// Up
			lcd_puts("Up");
			uart_puts("Button: Up");
			uart_puts("\r\n");


		}else if ((value >= 243)&(value <= 247)){

			// Down
			lcd_puts("Down");
			uart_puts("Button: Down");
			uart_puts("\r\n");

		}else if ((value >= 400)&(value <= 404)){

			// Left
			lcd_puts("Left");
			uart_puts("Button: Left");
			uart_puts("\r\n");

		}else if ((value >= 648)&(value <= 652)){

			// Select
			lcd_puts("Select");
			uart_puts("Button: Select");
			uart_puts("\r\n");

		}else if (value >= 1021){

			// None
			lcd_puts("None");
			uart_puts("Button: None");
			uart_puts("\r\n");

		}else{

			// ERROR
			lcd_puts("ERROR");
			uart_puts("Button: ERROR");
			uart_puts("\r\n");
		}
	}
}
