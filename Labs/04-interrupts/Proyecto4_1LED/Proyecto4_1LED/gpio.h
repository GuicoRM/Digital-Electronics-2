/*
 * gpio.h
 *
 * Created: 10/10/2020 19:06:35
 *  Author: Guillermo Cortés 
 */ 


#ifndef GPIO_H
#define GPIO_H

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

/**
 * @file  gpio.h
 * @brief GPIO library for AVR-GCC.
 *
 * @details
 * The library contains functions for controlling AVRs' gpio pin(s).
 *
 * @note
 * Based on AVR Libc Reference Manual. Tested on ATmega328P (Arduino Uno),
 * 16 MHz, AVR 8-bit Toolchain 3.6.2.
 * 
 * @copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Configure one output pin in Data Direction Register.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_config_output(volatile uint8_t *reg_name, uint8_t pin_num);

/**
 * @brief Configure one input pin in DDR without pull-up resistor.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_config_input_nopullup(volatile uint8_t *reg_name, uint8_t pin_num);

/**
 * @brief Configure one input pin in DDR and enable pull-up resistor.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_config_input_pullup(volatile uint8_t *reg_name, uint8_t pin_num);

/**
 * @brief Set one output pin in PORT register to low.
 * @param reg_name - Address of Data Direction Register, such as &PORTA,
 *                   &PORTB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_write_low(volatile uint8_t *reg_name, uint8_t pin_num);

/**
 * @brief Set one output pin in PORT register to high.
 * @param reg_name - Address of Data Direction Register, such as &PORTA,
 *                   &PORTB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_write_high(volatile uint8_t *reg_name, uint8_t pin_num);

/**
 * @brief Toggle one output pin value in PORT register.
 * @param reg_name - Address of Data Direction Register, such as &PORTA,
 *                   &PORTB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_toggle(volatile uint8_t *reg_name, uint8_t pin_num);

/**
 * @brief Get input pin value from PIN register.
 * @param reg_name - Address of Data Direction Register, such as &PINA,
 *                   &PINB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
   @return uint8_t - Control value (0 or 1)
 */
uint8_t GPIO_read(volatile uint8_t *reg_name, uint8_t pin_num);

#endif