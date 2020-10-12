/*
$Id:$

ST7565 LCD library!

Copyright (C) 2010 Limor Fried, Adafruit Industries

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

// some of this code was written by <cstone@pobox.com> originally; it is in the public domain.
*/

//#include <Wire.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
#include <util/delay.h>
#endif

#ifndef _delay_ms
  #define _delay_ms(t) delay(t)
#endif

#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif


#include <stdlib.h>
#include <Wire.h>

#include "ST7565-i2c.h"

// #define ST7565_STARTBYTES 1
#define ST7565_STARTBYTES 4

uint8_t is_reversed = 0;

// a handy reference to where the pages are on the screen
//const uint8_t pagemap[] = { 3, 2, 1, 0, 7, 6, 5, 4 };
const uint8_t pagemap[] = { 7, 6, 5, 4, 3, 2, 1, 0 };

// a 5x7 font table
const extern uint8_t PROGMEM font5x7[];
const extern uint8_t PROGMEM font7x9[];
const extern uint8_t PROGMEM font10x15[];
const uint8_t *font = font5x7;


uint8_t st7565_buffer[1024] = { 
0X00,0X00,0X00,0X00,0X00,0X0C,0X00,0X06,0X00,0X02,0X01,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X40,0X00,0XA0,0X10,0X40,0X04,
0X22,0X11,0X00,0X09,0X04,0X00,0X02,0X00,0X03,0X01,0X03,0X01,0X03,0X01,0X01,0X01,
0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X03,0X2F,0X4F,0X9E,0X2C,0X95,0X05,
0X83,0X21,0X09,0X24,0X00,0X02,0XA8,0X10,0X38,0X18,0XBC,0X9E,0XCE,0XDE,0XCF,0XEF,
0XE7,0XEF,0XE3,0XFB,0X69,0X78,0X3E,0X1E,0X1F,0X07,0X07,0X01,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X3F,0X3F,0X02,0X02,0X02,0X02,0X3F,0X3F,0X00,0X00,0X00,0X00,0X3F,0X3F,0X22,
0X22,0X22,0X22,0X00,0X00,0X00,0X00,0X3F,0X3F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X3F,0X3F,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0X1F,0X18,0X20,0X20,0X20,0X20,0X18,
0X1F,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0X2F,0X3F,0XFE,0XF8,0XC3,0XCB,0X3F,0X3F,0XFF,0XEC,0XFC,
0XF0,0XC0,0X40,0X80,0X40,0X01,0X20,0X00,0X30,0X10,0X88,0X07,0X01,0X02,0X08,0X80,
0XA0,0XE0,0XF8,0XE9,0XFA,0XFC,0XFE,0X7F,0X1F,0X8F,0XE3,0XF8,0XFA,0X7F,0X1F,0X05,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0XF0,0XF0,0X00,0X00,0X00,0X00,0XF0,0XF0,0X00,0X00,0X00,0X00,0XF0,0XF0,0X10,
0X10,0X10,0X10,0X00,0X00,0X00,0X00,0XF0,0XF0,0X10,0X10,0X10,0X10,0X00,0X00,0X00,
0XF0,0XF0,0X10,0X10,0X10,0X10,0X00,0X00,0X80,0XE0,0X60,0X10,0X10,0X10,0X10,0X60,
0XE0,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X2E,0XFF,0XFF,0XF5,0X00,0XBF,0XFE,0XFF,0X7F,0XDF,0X07,0X03,0X01,
0X00,0X00,0X00,0X40,0XC0,0XE0,0X40,0XF0,0X1C,0X93,0XB1,0XC8,0X08,0X04,0X00,0X08,
0X31,0X00,0X82,0X01,0X04,0X08,0X81,0XD0,0X79,0XF8,0XFC,0X3E,0X80,0XEB,0XFF,0XFF,
0X5D,0X00,0X00,0X00,0X00,0X03,0X0F,0X0E,0X0F,0X03,0X00,0X00,0X00,0X00,0X00,0X0F,
0X0F,0X08,0X08,0X08,0X0F,0X07,0X00,0X00,0X00,0X00,0X0F,0X0F,0X08,0X08,0X08,0X04,
0X07,0X03,0X00,0X00,0X00,0X0F,0X0F,0X0F,0X00,0X00,0X00,0X00,0X0F,0X0F,0X00,0X00,
0X08,0X08,0X0F,0X0F,0X08,0X08,0X00,0X00,0X00,0X0F,0X0F,0X07,0X03,0X00,0X00,0X0F,
0X0F,0X00,0X00,0X00,0X01,0X07,0X06,0X08,0X08,0X08,0X08,0X06,0X07,0X01,0X00,0X00,
0X00,0X00,0X00,0X80,0XF0,0XFE,0XFF,0X1F,0X83,0XE9,0XFC,0XFE,0XB7,0XFF,0XFB,0XFE,
0X7F,0X3F,0X0F,0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X80,0XE4,0X1F,0X06,0X20,0X00,
0X30,0X80,0X13,0X87,0X4B,0X03,0X21,0X08,0X02,0X28,0X8A,0X12,0X0F,0XBF,0X9E,0XF4,
0X00,0X00,0X00,0X1C,0X7C,0XE0,0X20,0X20,0XA0,0XE0,0X7C,0X0C,0X00,0X00,0X00,0XFC,
0XFC,0X40,0X40,0X70,0XF8,0X9C,0X0C,0X00,0X00,0X00,0XFC,0XFC,0X04,0X04,0X04,0X08,
0XF8,0XF0,0XC0,0X00,0X00,0XE0,0XF8,0XF8,0X04,0X04,0X04,0X08,0XF8,0XF0,0X00,0X00,
0X04,0X04,0XFC,0XFC,0X04,0X04,0X00,0X00,0X00,0XFC,0X00,0X00,0XC0,0XE0,0X78,0XFC,
0XFC,0X00,0X00,0X00,0XE0,0XF8,0X18,0X04,0X04,0X04,0X04,0X18,0XF8,0XE0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XF0,0XF8,0X78,0X3E,0X9E,0XCF,0XE6,0XF7,
0XD3,0XFB,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XD0,0X00,0X00,0X00,
0X00,0XEB,0XBF,0XF3,0XF3,0XCF,0XCF,0X9D,0X7E,0X7C,0X70,0X10,0X20,0X90,0X48,0X00,
0X10,0X0A,0X04,0X02,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X80,
0X80,0XE0,0XC0,0X20,0X20,0X30,0X20,0X30,0X20,0X30,0X30,0X30,0X20,0X30,0X20,0X20,
0X20,0XA0,0XC0,0XC0,0X00,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

// the memory buffer for the LCD
// uint8_t st7565_buffer[1024] = { 
// 0X00,0X00,0X00,0X00,0X00,0X30,0X00,0X60,0X00,0X40,0X80,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X02,0X00,0X05,0X08,0X02,0X20,
// 0X44,0X88,0X00,0X90,0X20,0X00,0X40,0X00,0XC0,0X80,0XC0,0X80,0XC0,0X80,0X80,0X80,
// 0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0XC0,0XF4,0XF2,0X79,0X34,0XA9,0XA0,
// 0XC1,0X84,0X90,0X24,0X00,0X40,0X15,0X08,0X1C,0X18,0X3D,0X79,0X73,0X7B,0XF3,0XF7,
// 0XE7,0XF7,0XC7,0XDF,0X96,0X1E,0X7C,0X78,0XF8,0XE0,0XE0,0X80,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0XFC,0XFC,0X40,0X40,0X40,0X40,0XFC,0XFC,0X00,0X00,0X00,0X00,0XFC,0XFC,0X44,
// 0X44,0X44,0X44,0X00,0X00,0X00,0X00,0XFC,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0XFC,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XF8,0X18,0X04,0X04,0X04,0X04,0X18,
// 0XF8,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0XC0,0XF4,0XFC,0X7F,0X1F,0XC3,0XD3,0XFC,0XFC,0XFF,0X37,0X3F,
// 0X0F,0X03,0X02,0X01,0X02,0X80,0X04,0X00,0X0C,0X08,0X11,0XE0,0X80,0X40,0X10,0X01,
// 0X05,0X07,0X1F,0X97,0X5F,0X3F,0X7F,0XFE,0XF8,0XF1,0XC7,0X1F,0X5F,0XFE,0XF8,0XA0,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X0F,0X0F,0X00,0X00,0X00,0X00,0X0F,0X0F,0X00,0X00,0X00,0X00,0X0F,0X0F,0X08,
// 0X08,0X08,0X08,0X00,0X00,0X00,0X00,0X0F,0X0F,0X08,0X08,0X08,0X08,0X00,0X00,0X00,
// 0X0F,0X0F,0X08,0X08,0X08,0X08,0X00,0X00,0X01,0X07,0X06,0X08,0X08,0X08,0X08,0X06,
// 0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X74,0XFF,0XFF,0XAF,0X00,0XFD,0X7F,0XFF,0XFE,0XFB,0XE0,0XC0,0X80,
// 0X00,0X00,0X00,0X02,0X03,0X07,0X02,0X0F,0X38,0XC9,0X8D,0X13,0X10,0X20,0X00,0X10,
// 0X8C,0X00,0X41,0X80,0X20,0X10,0X81,0X0B,0X9E,0X1F,0X3F,0X7C,0X01,0XD7,0XFF,0XFF,
// 0XBA,0X00,0X00,0X00,0X00,0XC0,0XF0,0X70,0XF0,0XC0,0X00,0X00,0X00,0X00,0X00,0XF0,
// 0XF0,0X10,0X10,0X10,0XF0,0XE0,0X00,0X00,0X00,0X00,0XF0,0XF0,0X10,0X10,0X10,0X20,
// 0XE0,0XC0,0X00,0X00,0X00,0XF0,0XF0,0XF0,0X00,0X00,0X00,0X00,0XF0,0XF0,0X00,0X00,
// 0X10,0X10,0XF0,0XF0,0X10,0X10,0X00,0X00,0X00,0XF0,0XF0,0XE0,0XC0,0X00,0X00,0XF0,
// 0XF0,0X00,0X00,0X00,0X80,0XE0,0X60,0X10,0X10,0X10,0X10,0X60,0XE0,0X80,0X00,0X00,
// 0X00,0X00,0X00,0X01,0X0F,0X7F,0XFF,0XF8,0XC1,0X97,0X3F,0X7F,0XED,0XFF,0XDF,0X7F,
// 0XFE,0XFC,0XF0,0XE0,0X80,0X00,0X00,0X00,0X00,0X00,0X01,0X27,0XF8,0X60,0X04,0X00,
// 0X0C,0X01,0XC8,0XE1,0XD2,0XC0,0X84,0X10,0X40,0X14,0X51,0X48,0XF0,0XFD,0X79,0X2F,
// 0X00,0X00,0X00,0X38,0X3E,0X07,0X04,0X04,0X05,0X07,0X3E,0X30,0X00,0X00,0X00,0X3F,
// 0X3F,0X02,0X02,0X0E,0X1F,0X39,0X30,0X00,0X00,0X00,0X3F,0X3F,0X20,0X20,0X20,0X10,
// 0X1F,0X0F,0X03,0X00,0X00,0X07,0X1F,0X1F,0X20,0X20,0X20,0X10,0X1F,0X0F,0X00,0X00,
// 0X20,0X20,0X3F,0X3F,0X20,0X20,0X00,0X00,0X00,0X3F,0X00,0X00,0X03,0X07,0X1E,0X3F,
// 0X3F,0X00,0X00,0X00,0X07,0X1F,0X18,0X20,0X20,0X20,0X20,0X18,0X1F,0X07,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X03,0X0F,0X1F,0X1E,0X7C,0X79,0XF3,0X67,0XEF,
// 0XCB,0XDF,0XFF,0XBF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0X00,0X00,0X00,
// 0X00,0XD7,0XFD,0XCF,0XCF,0XF3,0XF3,0XB9,0X7E,0X3E,0X0E,0X08,0X04,0X09,0X12,0X00,
// 0X08,0X50,0X20,0X40,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,
// 0X01,0X07,0X03,0X04,0X04,0X0C,0X04,0X0C,0X04,0X0C,0X0C,0X0C,0X04,0X0C,0X04,0X04,
// 0X04,0X05,0X03,0X03,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
// };


// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
#define enablePartialUpdate
//#undef enablePartialUpdate
#ifdef enablePartialUpdate
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
#endif



static void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) {
#ifdef enablePartialUpdate
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
#endif
}

void ST7565::drawbitmap(uint8_t x, uint8_t y, 
			const uint8_t *bitmap, uint8_t w, uint8_t h,
			uint8_t color) {
  for (uint8_t j=0; j<h; j++) {
    for (uint8_t i=0; i<w; i++ ) {
      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
	my_setpixel(x+i, y+j, color);
      }
    }
  }

  updateBoundingBox(x, y, x+w, y+h);
}

void ST7565::drawstring(uint8_t x, uint8_t line, char *c) {
  drawstring(x, line, c, FONT_SMALL);
}

void ST7565::drawstring(uint8_t x, uint8_t line, char *c, uint8_t fontsize) {
  while (c[0] != 0) {
    drawchar(x, line, c[0], fontsize);
    c++;
    x += fontsizex[fontsize] + fontspacex[fontsize]; // x pixels wide
    if (x + fontsizex[fontsize] + fontspacex[fontsize] >= LCDWIDTH) {
      x = 0;    // ran out of this line
      line++;
      if (fontsize == FONT_BIG) {
	line++;
      }
    }
    if (line >= (LCDHEIGHT/8))
      return;        // ran out of space :(
  }
}


void ST7565::drawstring_P(uint8_t x, uint8_t line, const char *str) {
  while (1) {
    char c = pgm_read_byte(str++);
    if (! c)
      return;
    drawchar(x, line, c);
    x += 6; // 6 pixels wide
    if (x + 6 >= LCDWIDTH) {
      x = 0;    // ran out of this line
      line++;
    }
    if (line >= (LCDHEIGHT/8))
      return;        // ran out of space :(
  }
}

void ST7565::negativeline(uint8_t line) {
  uint8_t i;
  for (i=0; i < LCDWIDTH; i++) {
    st7565_buffer[line*128 + i] = ~st7565_buffer[line*128 + i];
  }
}

void  ST7565::drawchar(uint8_t x, uint8_t line, char c) {
  drawchar(x, line, c, FONT_SMALL);
}

void  ST7565::drawchar(uint8_t x, uint8_t line, char c, uint8_t fontsize) {
  uint8_t xsave=x;
  if ((c < 32) or (c > 127)) {
    c=127;
  }
  c = c-32;
  if (fontsize == FONT_SMALL) {
    for (uint8_t i =0; i<fontsizex[fontsize]; i++ ) {
      st7565_buffer[x + (line*128) ] = pgm_read_byte(font+(c*fontsizex[fontsize])+i);
      x++;
    }
  } else if (fontsize == FONT_BIG) {
      for (uint8_t i =0; i<fontsizex[fontsize]; i++ ) {
	st7565_buffer[x + (line*128) ] = pgm_read_byte(font10x15+(c*fontsizex[fontsize]*2)+i*2);
	st7565_buffer[x + (line*128 + 128) ] = pgm_read_byte(font10x15+(c*fontsizex[fontsize]*2)+i*2+1);
	x++;
      }
  }

  updateBoundingBox(xsave,
		    line*8,
		    xsave + fontsizex[fontsize] + fontspacex[fontsize],
	  	    line*8  + fontsizey[fontsize] + fontspacey[fontsize]);
}


// bresenham's algorithm - thx wikpedia
void ST7565::drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
		      uint8_t color) {
  updateBoundingBox(x0, y0, x1, y1);
  uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    st_swap(x0, y0);
    st_swap(x1, y1);
  }

  if (x0 > x1) {
    st_swap(x0, x1);
    st_swap(y0, y1);
  }

  // much faster to put the test here, since we've already sorted the points

  uint8_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<=x1; x0++) {
    if (steep) {
      my_setpixel(y0, x0, color);
    } else {
      my_setpixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// filled rectangle
void ST7565::fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint8_t color) {

  // stupidest version - just pixels - but fast with internal buffer!
  for (uint8_t i=x; i<x+w; i++) {
    for (uint8_t j=y; j<y+h; j++) {
      my_setpixel(i, j, color);
    }
  }

  updateBoundingBox(x, y, x+w, y+h);
}

// draw a rectangle
void ST7565::drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint8_t color) {
  // stupidest version - just pixels - but fast with internal buffer!
  for (uint8_t i=x; i<x+w; i++) {
    my_setpixel(i, y, color);
    my_setpixel(i, y+h-1, color);
  }
  for (uint8_t i=y; i<y+h; i++) {
    my_setpixel(x, i, color);
    my_setpixel(x+w-1, i, color);
  } 

  updateBoundingBox(x, y, x+w, y+h);
}

// draw a circle outline
void ST7565::drawcircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  updateBoundingBox(x0-r, y0-r, x0+r, y0+r);

  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  my_setpixel(x0, y0+r, color);
  my_setpixel(x0, y0-r, color);
  my_setpixel(x0+r, y0, color);
  my_setpixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    my_setpixel(x0 + x, y0 + y, color);
    my_setpixel(x0 - x, y0 + y, color);
    my_setpixel(x0 + x, y0 - y, color);
    my_setpixel(x0 - x, y0 - y, color);
    
    my_setpixel(x0 + y, y0 + x, color);
    my_setpixel(x0 - y, y0 + x, color);
    my_setpixel(x0 + y, y0 - x, color);
    my_setpixel(x0 - y, y0 - x, color);
    
  }



}

void ST7565::fillcircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  updateBoundingBox(x0-r, y0-r, x0+r, y0+r);

  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  for (uint8_t i=y0-r; i<=y0+r; i++) {
    my_setpixel(x0, i, color);
  }

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    for (uint8_t i=y0-y; i<=y0+y; i++) {
      my_setpixel(x0+x, i, color);
      my_setpixel(x0-x, i, color);
    } 
    for (uint8_t i=y0-x; i<=y0+x; i++) {
      my_setpixel(x0+y, i, color);
      my_setpixel(x0-y, i, color);
    }    
  }
}

void ST7565::my_setpixel(uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    st7565_buffer[x+ (y/8)*128] |= _BV(7-(y%8));  
  else
    st7565_buffer[x+ (y/8)*128] &= ~_BV(7-(y%8)); 
}

// the most basic function, set a single pixel
void ST7565::setpixel(uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    st7565_buffer[x+ (y/8)*128] |= _BV(7-(y%8));  
  else
    st7565_buffer[x+ (y/8)*128] &= ~_BV(7-(y%8)); 

  updateBoundingBox(x,y,x,y);
}


// the most basic function, get a single pixel
uint8_t ST7565::getpixel(uint8_t x, uint8_t y) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return 0;

  return (st7565_buffer[x+ (y/8)*128] >> (7-(y%8))) & 0x1;  
}

void ST7565::begin(uint8_t contrast) {
  st7565_init();
  st7565_command(CMD_DISPLAY_ON);
  st7565_command(CMD_SET_ALLPTS_NORMAL);
  st7565_set_brightness(contrast);
}

void ST7565::i2cbegin(uint8_t contrast) {
  st7565_i2cinit();
  st7565_command(CMD_DISPLAY_ON);
  st7565_command(CMD_SET_ALLPTS_NORMAL);
  #ifdef enablePartialUpdate
  xUpdateMin = 0   ;
  xUpdateMax = 127 ;
  yUpdateMin = 0   ;
  yUpdateMax = 64  ;
  #endif
  st7565_set_brightness(contrast);
}


void ST7565::st7565_i2cinit(void) {
  st7565_command(0x2f);            // Power_Control
  st7565_command(0xA2);            // BIAS Setting   A2(1/9);A3(1/7)
  st7565_command(0xA1);            // Set SEG Direction. It was initially 0xA1 
  st7565_command(0xC0);            // Set COM Direction. It was initially 0xC8
                          
  st7565_command(0x24);            // Set VLCD Resistor Ratio    0-7
  st7565_command(0x81);            // Set Electronic Volume
  st7565_command(37);              // Contrast level  0~63, It was initially 37
  st7565_command(0x40);            // Initial_Dispay_Line, It was initially 40, later 60
  st7565_command(0xAF);            // Display ON
}

void ST7565::st7565_init(void) {
  if (i2c == 0) {
    // set pin directions
    pinMode(sid, OUTPUT);
    pinMode(sclk, OUTPUT);
    pinMode(a0, OUTPUT);
    pinMode(rst, OUTPUT);
    pinMode(cs, OUTPUT);

    // toggle RST low to reset; CS low so it'll listen to us
    if (cs > 0)
      digitalWrite(cs, LOW);

    digitalWrite(rst, LOW);
    _delay_ms(500);
    digitalWrite(rst, HIGH);
  }
  
  // LCD bias select
  st7565_command(CMD_SET_BIAS_7);
  // ADC select
  st7565_command(CMD_SET_ADC_NORMAL);
  // SHL select
  st7565_command(CMD_SET_COM_NORMAL);
  // Initial display line
  st7565_command(CMD_SET_DISP_START_LINE);

  // turn on voltage converter (VC=1, VR=0, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x4);
  // wait for 50% rising
  _delay_ms(50);

  // turn on voltage regulator (VC=1, VR=1, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x6);
  // wait >=50ms
  _delay_ms(50);

  // turn on voltage follower (VC=1, VR=1, VF=1)
  st7565_command(CMD_SET_POWER_CONTROL | 0x7);
  // wait
  _delay_ms(10);

  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  st7565_command(CMD_SET_RESISTOR_RATIO | 0x6);

  // initial display line
  // set page address
  // set column address
  // write display data

  // set up a bounding box for screen updates

  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
}

inline void ST7565::spiwrite(uint8_t c) {
    
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
    shiftOut(sid, sclk, MSBFIRST, c);
#else
    int8_t i;
    for (i=7; i>=0; i--) {
        digitalWrite(sclk, LOW);
        delayMicroseconds(5);      //need to slow down the data rate for Due and Zero
        if (c & _BV(i))
            digitalWrite(sid, HIGH);
        else
            digitalWrite(sid, LOW);
  //      delayMicroseconds(5);      //need to slow down the data rate for Due and Zero
        digitalWrite(sclk, HIGH);
    }
#endif
  /*
  int8_t i;
  for (i=7; i>=0; i--) {
    SCLK_PORT &= ~_BV(SCLK);
    if (c & _BV(i))
      SID_PORT |= _BV(SID);
    else
      SID_PORT &= ~_BV(SID);
    SCLK_PORT |= _BV(SCLK);
  }
  */

  /*
  // loop unwrapped! too fast doesnt work :(
 
  SCLK_PORT &= ~_BV(SCLK);
  if (c & _BV(7))
    SID_PORT |= _BV(SID);
  else
    SID_PORT &= ~_BV(SID);
  SCLK_PORT |= _BV(SCLK);

  SCLK_PORT &= ~_BV(SCLK);
  if (c & _BV(6))
    SID_PORT |= _BV(SID);
  else
    SID_PORT &= ~_BV(SID);
  SCLK_PORT |= _BV(SCLK);
 
  SCLK_PORT &= ~_BV(SCLK);
  if (c & _BV(5))
    SID_PORT |= _BV(SID);
  else
    SID_PORT &= ~_BV(SID);
  SCLK_PORT |= _BV(SCLK);

  SCLK_PORT &= ~_BV(SCLK);
  if (c & _BV(4))
    SID_PORT |= _BV(SID);
  else
    SID_PORT &= ~_BV(SID);
  SCLK_PORT |= _BV(SCLK);

  SCLK_PORT &= ~_BV(SCLK);
  if (c & _BV(3))
    SID_PORT |= _BV(SID);
  else
    SID_PORT &= ~_BV(SID);
  SCLK_PORT |= _BV(SCLK);

  SCLK_PORT &= ~_BV(SCLK);
  if (c & _BV(2))
    SID_PORT |= _BV(SID);
  else
    SID_PORT &= ~_BV(SID);
  SCLK_PORT |= _BV(SCLK);


  SCLK_PORT &= ~_BV(SCLK);
  if (c & _BV(1))
    SID_PORT |= _BV(SID);
  else
    SID_PORT &= ~_BV(SID);
  SCLK_PORT |= _BV(SCLK);

  SCLK_PORT &= ~_BV(SCLK);
  if (c & _BV(0))
    SID_PORT |= _BV(SID);
  else
    SID_PORT &= ~_BV(SID);
  SCLK_PORT |= _BV(SCLK);
*/

}

void ST7565::i2cCmdWrite(uint8_t c) {
  Wire.beginTransmission(cmd_addr);
  Wire.write(c);
  Wire.endTransmission();
  delay(10);
}

void ST7565::i2cDataWrite(uint8_t c) {
  Wire.beginTransmission(data_addr);
  Wire.write(c);
  Wire.endTransmission();
}

void ST7565::st7565_command(uint8_t c) {
  if (i2c == 0) {
    digitalWrite(a0, LOW);
    spiwrite(c);
  } else {
    i2cCmdWrite(c);
  }
}

void ST7565::st7565_data(uint8_t c) {
  if (i2c == 0) {
    digitalWrite(a0, HIGH);
    spiwrite(c);
  } else {
    i2cDataWrite(c);
  }
}

void ST7565::st7565_set_brightness(uint8_t val) {
    st7565_command(CMD_SET_VOLUME_FIRST);
    st7565_command(CMD_SET_VOLUME_SECOND | (val & 0x3f));
}


void ST7565::display(void) {
  uint8_t col, maxcol, p;

  /*
  Serial.print("Refresh ("); Serial.print(xUpdateMin, DEC); 
  Serial.print(", "); Serial.print(xUpdateMax, DEC);
  Serial.print(","); Serial.print(yUpdateMin, DEC); 
  Serial.print(", "); Serial.print(yUpdateMax, DEC); Serial.println(")");
  */

  for(p = 0; p < 8; p++) {
    /*
      putstring("new page! ");
      uart_putw_dec(p);
      putstring_nl("");
    */
#ifdef enablePartialUpdate
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
#endif

    st7565_command(CMD_SET_PAGE | pagemap[p]);


#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;
#endif

    st7565_command(CMD_SET_COLUMN_LOWER | ((col+ST7565_STARTBYTES) & 0xf));
    st7565_command(CMD_SET_COLUMN_UPPER | (((col+ST7565_STARTBYTES) >> 4) & 0x0F));
    st7565_command(CMD_RMW);
    
    for(; col <= maxcol; col++) {
      //uart_putw_dec(col);
      //uart_putchar(' ');
      st7565_data(st7565_buffer[(128*p)+col]);
    }
  }

#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif
}

// clear everything
void ST7565::clear(void) {
  memset(st7565_buffer, 0, 1024);
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
}


// this doesnt touch the buffer, just clears the display RAM - might be handy
void ST7565::clear_display(void) {
  uint8_t p, c;
  
  for(p = 0; p < 8; p++) {
    /*
      putstring("new page! ");
      uart_putw_dec(p);
      putstring_nl("");
    */

    st7565_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      //uart_putw_dec(c);
      //uart_putchar(' ');
      st7565_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      st7565_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      st7565_data(0x0);
    }     
  }
}

  
