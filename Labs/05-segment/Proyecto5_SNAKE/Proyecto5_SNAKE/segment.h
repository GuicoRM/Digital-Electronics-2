/*
 * segment.h
 *
 * Created: 25/10/2020 22:02:21
 *  Author: Guillermo Cortés
 */ 


#ifndef SEGMENT_H
#define SEGMENT_H

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

/**
 * @file  segment.h
 * @brief Seven-segment display library for AVR-GCC.
 *
 * @details
 * The library contains functions for controlling the seven-segment 
 * display (SSD) using two shift registers 74HC595.
 *
 * @copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>

/* Defines -----------------------------------------------------------*/
/**
 * @brief Defines the interface of SSD.
 * @note  Connection is based on Multi-function shield.
 */
#define SEGMENT_LATCH   PD4
#define SEGMENT_CLK     PD7
#define SEGMENT_DATA    PB0

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Configure SSD signals LATCH, CLK, and DATA as output.
 */
void SEG_init(void);

/**
 * @brief Display segments at one position of the SSD.
 * @param segments - Segments to be displayed (abcdefgDP, active low)
 * @param position - Position of the display where the segments is to 
 *                   be displayed (p3 p2 p1 p0 ...., active high)
 * @note  Two shift registers are connected in series, ie 16 bits are 
 *        transmitted.
 */
void SEG_update_shift_regs(uint8_t segments, uint8_t position);

/** 
 * @brief Turn off all segments at all positions of the SSD 
 */
void SEG_clear (void);

/**
 * @brief Generate one CLK signal period with a duration of 2 us
 *
 * @note  Frequency of the signal = f = 500kHz = 500000Hz  
 * Period of the signal = T = 1/f = 1/500000 = 2 us
 * T(ON) = 1 us ; T(OFF) = 1 us
 * 
 */
void SEG_clk_2us(void);

#endif