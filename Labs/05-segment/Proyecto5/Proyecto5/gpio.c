/*
 * gpio.c
 *
 * Created: 10/10/2020 21:06:48
 *  Author: Guillermo Cortés 
 */ 

/***********************************************************************
 * 
 * GPIO library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) Guillermo Cortés 
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include "gpio.h"

/* Function definitions ----------------------------------------------*/

/*OUTPUT*/
void GPIO_config_output(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name | (1<<pin_num);
}

/*INPUT*/
/*NO PULL*/
void GPIO_config_input_nopullup(volatile uint8_t *reg_name, uint8_t pin_num)
{
	*reg_name = *reg_name & ~(1<<pin_num);  // Data Direction Register
	*reg_name++;                    // Change pointer to Data Register
	*reg_name = *reg_name & ~(1<<pin_num);   // Data Register
}

/*PULL*/
void GPIO_config_input_pullup(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name & ~(1<<pin_num);  // Data Direction Register
    *reg_name++;                    // Change pointer to Data Register
    *reg_name = *reg_name | (1<<pin_num);   // Data Register
}

/*LOW*/
void GPIO_write_low(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name & ~(1<<pin_num); // Clear the bit (and not)
}

/*HIGH*/
void GPIO_write_high(volatile uint8_t *reg_name, uint8_t pin_num)
{
	*reg_name = *reg_name | (1<<pin_num); // Set the bit (or)
}

/*TOGGLE*/
void GPIO_toggle(volatile uint8_t *reg_name, uint8_t pin_num)
{
	*reg_name = *reg_name ^ (1<<pin_num); // Toggle the bit
}

/*READ*/
uint8_t GPIO_read(volatile uint8_t *reg_name, uint8_t pin_num)
{
	uint8_t result = 0;
	
	if (bit_is_clear(*reg_name, pin_num)){ // if 'PUSH' (0) -> I enter de 'if' -> Si hay un '0' (ACTIVO BAJO), entro en el 'if'
		result = 1;
	}
	
	return result;
}