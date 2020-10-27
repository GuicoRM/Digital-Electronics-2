/*
 * Proyecto4.c
 *
 * Created: 16/10/2020 19:58:51
 * Author : Guillermo Cortés
 */ 

/***********************************************************************
 * 
 * Control LEDs using functions from GPIO and Timer libraries. Do not 
 * use delay library any more.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) Guillermo Cortés 
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB5
#define LED_D2  PB4

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle TWO LEDS
 * on the Multi-function shield using the internal 8- or 16-bit 
 * Timer/Counter.
 */
int main(void)
{
    /* Configuration of LED(s) */
	
	/*************************************LED_D1******************************************/
    GPIO_config_output(&DDRB, LED_D1);
    GPIO_write_low(&PORTB, LED_D1);	
	
	
	/*************************************LED_D2******************************************/
	GPIO_config_output(&DDRB, LED_D2);
	GPIO_write_low(&PORTB, LED_D2);	
	

    /* Configuration of TIMER/COUNTER */

    /*********************************TIMER/COUNTER1**************************************/
	/* Configuration of 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
    TIM1_overflow_1s();
    TIM1_overflow_interrupt_enable();
	
	/*********************************TIMER/COUNTER2**************************************/
	/* Configuration of 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
    TIM2_overflow_4ms();
    TIM2_overflow_interrupt_enable();


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
 * ISR starts when Timer/Counter1 overflows. Toggle D1 LED on 
 * Multi-function shield. */
ISR(TIMER1_OVF_vect)
{
    GPIO_toggle(&PORTB, LED_D1);
}

/**
 * ISR starts when Timer/Counter1 overflows. Toggle D1 LED on 
 * Multi-function shield. */
ISR(TIMER2_OVF_vect)
{
	GPIO_toggle(&PORTB, LED_D2);
}