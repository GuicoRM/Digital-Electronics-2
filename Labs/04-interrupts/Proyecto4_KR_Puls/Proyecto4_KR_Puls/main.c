/*
 * Proyecto4_KR_Puls.c
 *
 * Created: 19/10/2020 18:17:41
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
#define LED_D3  PB3
#define LED_D4  PB2
#define BTN     PD0

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Program that will -after you press the button- 
 * ensure that only one of LED (on the Multi-function shield) is switched on at a time in Knight Rider style (fast)
 *  using the internal 8- or 16-bit Timer/Counter. If you release the button, the Led will blink with other speed (slow)
 */
int main(void)
{
    /* Configuration of LED(s) and PUSH BUTTON*/
	
	/* 4 LED(s) are set to ACTIVE LOW mode as they are in Multi-function shield
	 * PUSH BUTTON is set to ACTIVE LOW mode
	 */
	
	/*************************************LED_D1******************************************/
    GPIO_config_output(&DDRB, LED_D1);
    GPIO_write_low(&PORTB, LED_D1);
	
	/*************************************LED_D2******************************************/
	GPIO_config_output(&DDRB, LED_D2);
	GPIO_write_high(&PORTB, LED_D2);
	
	/*************************************LED_D3******************************************/
	GPIO_config_output(&DDRB, LED_D3);
	GPIO_write_high(&PORTB, LED_D3);	
	
	/*************************************LED_D4******************************************/
	GPIO_config_output(&DDRB, LED_D4);
	GPIO_write_high(&PORTB, LED_D4);
	
	/***************************************BTN******************************************/
	GPIO_config_input_pullup(&DDRD, BTN);	
	

    /* Configuration of TIMER/COUNTER */	

    /*********************************TIMER/COUNTER1**************************************/
	/* Configuration of 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
	
	// Lo configuro dentro del while(1) para que así tenga efecto el pulsador
	
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
		
		// Debería ser un loop vacío, pero al utilizar el pulsador en mi diseño, lo he incluído dentro
		
		if(GPIO_read(&PIND, BTN) == 1){ 
			
			TIM1_overflow_262ms();
			TIM1_overflow_interrupt_enable();
			
		}else{
		
			TIM1_overflow_1s();
			TIM1_overflow_interrupt_enable();
		
		}
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
	 static uint8_t a = 0;
	
	if((a == 0) | (a == 5)){	
			
		if (a == 5){
			
			GPIO_toggle(&PORTB, LED_D1);
			GPIO_toggle(&PORTB, LED_D2);
			a = 0;
			
		}else{
			
			GPIO_toggle(&PORTB, LED_D1);
			GPIO_toggle(&PORTB, LED_D2);
			a++;
		}
		
	}else if((a == 1) | (a == 4)){
		
		GPIO_toggle(&PORTB, LED_D2);
		GPIO_toggle(&PORTB, LED_D3);
		a++;
		
	}else if((a == 2) | (a == 3)){
		
		GPIO_toggle(&PORTB, LED_D3);
		GPIO_toggle(&PORTB, LED_D4);
		a++;
		
	}		

}
