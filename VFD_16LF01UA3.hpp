#pragma once

#include <stdio.h>  /* for printf */
#include <stdlib.h> /* for exit */
#include <unistd.h>
//#include <chrono>
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


char flags = 0;
unsigned int delay=100000;

/*!
 *  \brief	Function to reset VFD dispaly by reset line\n
 *  		and set to default
 */
void reset_vfd();

/*!
 *  \brief	Function to write string to display at current position
 *  		If swirl effect is enabled, assumed position is beginning.
 *  \param 	txt - text to be written, max 16 chars
 *  \return	actual used characters to display string. On vfd dot, coma 
 * 			sign are displayed not as separate char, but as additional 
 * 			segment.
 *  \todo 	add checking for length. 
 */
int write_string(const char *txt);

/*!
 *  \brief	set position from left to right. 0 - is most left, 16 is 
 * 			most right
 */
void set_position(unsigned char pos);

/*!
 *  \brief	set brightness in scale 0-31.
 */
void set_brightnes(unsigned int br);
void printRotateCW(const char* txt);
int16_t printRotateLine(const char* txt, const int16_t x, int16_t y);
