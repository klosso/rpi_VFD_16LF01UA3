/**
 * @file VFD_16LF01UA3.hpp
 * @brief 
 * @author Sebastian Mikulski
 * @version 1.0
 * @date 2023-11-13
 */
#pragma once

#include <stdio.h>  /* for printf */
#include <stdlib.h> /* for exit */
#include <unistd.h>
#include <pigpio.h>
#include <string.h>
#include <ctype.h>


#define VFD_RST 20
#define VFD_CLK 21
#define VFD_DTA 16
/*
 * connection VFD display to Raspbery pi GPIO
 *  rpi                          VFD_16LF01UA3
 *  ---|                       |------
 *     |             +5V(1,2)--|
 *     |-- VFD_RST -- RST(10)--|
 *     |-- VFD_CLK -- SCLK(8)--|
 *     |-- VFD_DTA -- DATA(9)--|
 *     |           GND(11,12)--|
 *  ---|                       |-------
 */

#define SWIRL_EFFECT 0x1

extern char flags;
extern unsigned int delay;

/*!
 *  @brief	Function to reset VFD dispaly by reset line\n
 *  		and set to default
 */
void reset_vfd();

/*!
 *  @brief	Function to write string to display at current position
 *  		If swirl effect is enabled, assumed position is beginning.
 *  @param 	txt - text to be written, max 16 chars
 *  @return	actual used characters to display string. On vfd dot, coma 
 * 			sign are displayed not as separate char, but as additional 
 * 			segment.
 *  @todo 	add checking for length. 
 */
int write_string(const char *txt);

/*!
 *  @brief	set position from left to right. 0 - is most left, 16 is 
 * 			most right
 *  @param	pos - position on display.
 */
void set_position(unsigned char pos);

/*!
 *  @brief	set brightness
 *  @param 	br - brightness in scale 0-31. 
 */
void set_brightnes(unsigned int br);

/*!
 *  @brief	rotate string in CCW direction
 *  @param 	txt - string that will be displayed. Unlimited length.
 */
void printRotateCW(const char* txt);
/*!
 *  @brief	rotate string in CC direction
 *  @param 	txt - string that will be displayed. Unlimited length.
 * 	@todo	improve displaying in case of dots and comas, glitches
 * 			comes from fact that dot and coma on vfd is not separate
 * 			character
 */
int16_t printRotateLine(const char* txt, const int16_t x, int16_t y);
