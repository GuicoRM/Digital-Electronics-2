/*
 * segment.c
 *
 * Created: 25/10/2020 22:03:30
 *  Author: Guillermo Cortés
 */ 

/***********************************************************************
 * 
 * Seven-segment display library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) Guillermo Cortés
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#define F_CPU 16000000

#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Function definitions ----------------------------------------------*/
void SEG_init(void)
{
	/* Configuration of SSD signals */
	GPIO_config_output(&DDRD, SEGMENT_LATCH);
	GPIO_config_output(&DDRD, SEGMENT_CLK);
	GPIO_config_output(&DDRB, SEGMENT_DATA);
}

/*--------------------------------------------------------------------*/
void SEG_update_shift_regs(uint8_t segments, uint8_t position)
{
	uint8_t bit_number;
	segments = segment_value[segments];     // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	position = segment_position[position];  // 0, 1, 2, 3

	// Pull LATCH, CLK, and DATA low
	GPIO_write_low(&PORTD, SEGMENT_LATCH); // LATCH
	GPIO_write_low(&PORTD, SEGMENT_CLK);   // CLK
	GPIO_write_low(&PORTB, SEGMENT_DATA);  // DATA

	// Wait 1 us
	_delay_us(1);

	// Loop through the 1st byte (segments)
	// a b c d e f g DP (active low values)
	for (bit_number = 0; bit_number < 8; bit_number++)
	{
		// Output DATA value (bit 0 of "segments")
		if ((segments %2) == 0){			// LSB is 0 (EVEN) -> If it ends in '0' it implies that this position
			// must be ON in the SSD (receive a '0' so that it lights up -> GPIO_write_low)
			GPIO_write_low(&PORTB, SEGMENT_DATA);
			} else {							// LSB is 1 (ODD) -> If it ends in '1' it implies that this position
			// must be OFF in the SSD (receive a '1' so to turn it OFF -> GPIO_write_high)
			GPIO_write_high(&PORTB, SEGMENT_DATA);
		}

		// Wait 1 us
		_delay_us(1);
		
		// Pull CLK high
		GPIO_write_high(&PORTD, SEGMENT_CLK);

		// Wait 1 us
		_delay_us(1);
		
		// Pull CLK low
		GPIO_write_low(&PORTD, SEGMENT_CLK);

		// Shift "segments"
		segments = segments >> 1;
	}

	// Loop through the 2nd byte (position)
	// p3 p2 p1 p0 . . . . (active high values)
	for (bit_number = 0; bit_number < 8; bit_number++)
	{
		// Output DATA value (bit 0 of "position")
		if ((position %2) == 0){			// LSB is 0 (EVEN) -> If it ends in '0' it implies that this position
											// must be ON in the SSD (receive a '0' so that it lights up -> GPIO_write_low)
			GPIO_write_low(&PORTB, SEGMENT_DATA);
		} else {							// LSB is 1 (ODD) -> If it ends in '1' it implies that this position
											// must be OFF in the SSD (receive a '1' so to turn it OFF -> GPIO_write_high)
			GPIO_write_high(&PORTB, SEGMENT_DATA);
		}

		// Wait 1 us
		_delay_us(1);
		
		// Pull CLK high
		GPIO_write_high(&PORTD, SEGMENT_CLK);

		// Wait 1 us
		_delay_us(1);
		
		// Pull CLK low
		GPIO_write_low(&PORTD, SEGMENT_CLK);

		// Shift "position"
		position = position >> 1;
	}

	// Pull LATCH high
	GPIO_write_high(&PORTD, SEGMENT_LATCH);
	
	// Wait 1 us
	_delay_us(1);
}

/*--------------------------------------------------------------------*/
/* SEG_clear */
void SEG_clear(void){
	
	uint8_t bit_number2;

	// Pull LATCH, CLK, and DATA low
	GPIO_write_low(&PORTD, SEGMENT_LATCH); // LATCH
	GPIO_write_low(&PORTD, SEGMENT_CLK);   // CLK
	GPIO_write_low(&PORTB, SEGMENT_DATA);  // DATA

	// Wait 1 us
	_delay_us(1);

	// Loop through the 1st byte (segments)
	// a b c d e f g DP (active low values)
	for (bit_number2 = 0; bit_number2 < 8; bit_number2++)
	{
		// Output DATA value (bit 0 of "segments")
		GPIO_write_high(&PORTB, SEGMENT_DATA);

		// Wait 1 us
		_delay_us(1);
		
		// Pull CLK high
		GPIO_write_high(&PORTD, SEGMENT_CLK);

		// Wait 1 us
		_delay_us(1);
		
		// Pull CLK low
		GPIO_write_low(&PORTD, SEGMENT_CLK);

	}

	// Loop through the 2nd byte (position)
	// p3 p2 p1 p0 . . . . (active high values)
	for (bit_number2 = 0; bit_number2 < 8; bit_number2++)
	{
		// Output DATA value (bit 0 of "position")
		GPIO_write_high(&PORTB, SEGMENT_DATA);

		// Wait 1 us
		_delay_us(1);
		
		// Pull CLK high
		GPIO_write_high(&PORTD, SEGMENT_CLK);

		// Wait 1 us
		_delay_us(1);
		
		// Pull CLK low
		GPIO_write_low(&PORTD, SEGMENT_CLK);
		
	}

	// Pull LATCH high
	GPIO_write_high(&PORTD, SEGMENT_LATCH);
	
	// Wait 1 us
	_delay_us(1);
	
}

/*--------------------------------------------------------------------*/
/* SEG_clk_2us */
void SEG_clk_2us(void){
	
	while(1){
		GPIO_write_low(&PORTD, SEGMENT_CLK);	// Due to frequency of the signal = 500kHz,
												// period = 2 us -> T(ON) = 1 us
		_delay_us(1);							
												
		
		GPIO_write_high(&PORTD, SEGMENT_CLK);	// Due to frequency of the signal = 800kHz,
												// period = 2 us -> T(OFF) = 1 us
		_delay_us(1);
	}
	
	
}