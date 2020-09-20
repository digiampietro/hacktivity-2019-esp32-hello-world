#ifndef ERC12864_10_display

#include <Wire.h>

#define DISP_CMD_ADDR 0x38
#define DISP_DAT_ADDR 0x39

const uint8_t pic0[]=
{/*ERC12864-10 Series.bmp  --*/
/*-- 128x64  --*/
0xFF,0x01,0x01,0xF9,0x49,0x49,0x49,0x49,0x01,0xC1,0xB1,0x89,0xB1,0xC1,0x01,0x31,
0x49,0x49,0x91,0x01,0x09,0x09,0xF9,0x09,0x09,0x01,0xF9,0x49,0x49,0xC9,0x31,0x01,
0x01,0xF9,0x01,0x01,0x31,0x49,0x49,0x91,0x01,0x01,0xF9,0x01,0x01,0xF9,0x31,0x41,
0x81,0xF9,0x01,0x01,0xF1,0x09,0x09,0x49,0xD1,0x01,0x01,0x01,0x01,0x09,0x09,0xF9,
0x09,0x09,0x01,0xF9,0x49,0x49,0x49,0x49,0x01,0xF1,0x09,0x09,0x09,0x11,0x01,0x01,
0xF9,0x41,0x41,0x41,0xF9,0x01,0x01,0x01,0x01,0x01,0xF1,0x09,0x09,0x09,0x11,0x01,
0x01,0xF1,0x09,0x09,0x09,0xF1,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xF9,0x01,
0x01,0x01,0x09,0x09,0xF9,0x09,0x09,0x01,0xF9,0x09,0x09,0x09,0xF1,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x03,0x02,0x02,0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x01,0x02,0x01,
0x02,0x02,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x83,0xF0,0x80,0x80,0x03,0xE0,
0x20,0x23,0x20,0xE0,0x01,0x02,0x02,0x01,0xF0,0x10,0x13,0x10,0x10,0x13,0x10,0xF0,
0x01,0x03,0x20,0x20,0x21,0xA2,0x62,0xB2,0x21,0x20,0x20,0x20,0x20,0x00,0x40,0x43,
0x40,0x40,0xC0,0x53,0x62,0x42,0x42,0x62,0x40,0x01,0xA2,0xA2,0xE2,0x91,0x90,0x00,
0x23,0x40,0x00,0xF0,0x03,0x00,0x40,0x42,0xF0,0x40,0x01,0x42,0x42,0xF2,0x41,0x40,
0x40,0x01,0x02,0x02,0x02,0x01,0x00,0x00,0x02,0x00,0x00,0x06,0x00,0x00,0x03,0x02,
0x02,0x02,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x01,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x1F,0x00,0x7F,0x40,0x5F,
0x49,0x49,0x49,0x5F,0x60,0x00,0x00,0x00,0x7F,0x21,0x21,0x21,0x21,0x21,0x21,0x7F,
0x00,0x00,0x20,0x22,0x13,0x0A,0x42,0x7F,0x02,0x06,0x0A,0x12,0x20,0x00,0x40,0x40,
0x20,0x18,0x07,0x41,0x41,0x41,0x3F,0x00,0x00,0x00,0x18,0x06,0x7F,0x02,0x04,0x08,
0x09,0x0A,0x08,0x7F,0x04,0x00,0x44,0x44,0x7F,0x02,0x40,0x46,0x2A,0x13,0x2A,0x46,
0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xE0,
0x20,0x20,0x20,0x00,0xE0,0xE0,0x20,0x20,0xE0,0xC0,0x00,0x80,0xC0,0x60,0x20,0x20,
0x60,0x40,0x00,0x80,0xC0,0xE0,0xE0,0x00,0x00,0x40,0x60,0x20,0xE0,0xC0,0x00,0xC0,
0xE0,0x20,0xE0,0xC0,0x00,0xC0,0xE0,0x20,0x60,0x40,0x00,0x00,0x80,0x40,0xE0,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xE0,0x00,0x00,0xC0,0xE0,0x20,0xE0,
0xC0,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,
0x11,0x11,0x11,0x00,0x1F,0x1F,0x02,0x06,0x0F,0x19,0x10,0x07,0x0F,0x18,0x10,0x10,
0x18,0x08,0x00,0x01,0x00,0x1F,0x1F,0x00,0x00,0x18,0x1C,0x16,0x13,0x11,0x00,0x0E,
0x1F,0x11,0x1F,0x0E,0x00,0x0F,0x1F,0x11,0x1F,0x0E,0x00,0x06,0x05,0x04,0x1F,0x1F,
0x00,0x02,0x02,0x02,0x02,0x00,0x01,0x00,0x1F,0x1F,0x00,0x00,0x0F,0x1F,0x10,0x1F,
0x0F,0x00,0x00,0x00,0x00,0x00,0x09,0x12,0x12,0x0C,0x00,0x0E,0x15,0x15,0x16,0x00,
0x1F,0x01,0x01,0x1F,0x00,0x0E,0x15,0x15,0x16,0x00,0x12,0x15,0x15,0x09,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x10,0xF8,0x00,0x00,0x10,0x88,0x48,0x30,0x00,
0xB0,0x48,0x48,0xB0,0x00,0x00,0x18,0xA0,0xE0,0x18,0x00,0xF0,0x48,0x48,0x90,0x00,
0x80,0x60,0x10,0xF8,0x00,0x00,0x00,0xC0,0x20,0x20,0x20,0xF8,0x00,0xC0,0x20,0x20,
0x20,0xC0,0x20,0xF8,0x20,0x00,0x40,0xA0,0xA0,0x20,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xF0,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x70,0x88,0x88,0xF0,0x00,0xF0,0x28,
0x28,0xD0,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0xC0,0xC0,0x00,0x80,0xC0,0x80,0x00,0xC0,0xC0,0x00,0xC0,
0xC0,0x00,0x80,0xC0,0x80,0x00,0xC0,0xC0,0x03,0xC0,0xC0,0x03,0x82,0xC2,0x82,0x00,
0xC1,0xC2,0x02,0x01,0x00,0x00,0xF3,0xF0,0x80,0x43,0xC0,0x81,0x02,0xC2,0xC1,0x00,
0x01,0xC1,0xC1,0xC3,0xC1,0x00,0x00,0x01,0xC2,0xC2,0x82,0xC3,0x40,0x81,0xF2,0xF2,
0x02,0xD1,0xD0,0x03,0x82,0xC0,0x42,0x42,0xC2,0x81,0x00,0xC0,0xC0,0x80,0x40,0xC0,
0x80,0x01,0xF2,0xF2,0x01,0x80,0x40,0x42,0x40,0xC1,0x82,0xC2,0xC1,0x00,0x01,0x02,
0xC2,0xC1,0x00,0x00,0x00,0x80,0xC0,0x40,0x40,0xC0,0x80,0x00,0x80,0xC0,0x40,0x40,
0xC0,0x80,0x00,0xC0,0xC0,0x80,0x40,0xC0,0xC0,0x80,0x40,0xC0,0x80,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x83,0x9F,0x9C,0x8F,0x81,0x8F,0x9C,0x9F,0x83,0x80,0x83,
0x9F,0x9C,0x8F,0x81,0x8F,0x9C,0x9F,0x83,0x80,0x83,0x9F,0x9C,0x8F,0x81,0x8F,0x9C,
0x9F,0x83,0x80,0x98,0x98,0x80,0x9F,0x9F,0x88,0x90,0x9F,0x8F,0x80,0x8F,0x9F,0x90,
0x88,0x9F,0x9F,0xC0,0xC7,0xFF,0xB8,0x9F,0x87,0x80,0x8F,0x9F,0x90,0x88,0x9F,0x9F,
0x80,0x9F,0x9F,0x80,0x89,0x9B,0x93,0x96,0x9E,0x8C,0x80,0xFF,0xFF,0x88,0x90,0x9F,
0x8F,0x80,0x9F,0x9F,0x80,0x8C,0x9E,0x93,0x91,0x9F,0x9F,0xC0,0xC7,0xFF,0xB8,0x9F,
0x87,0x80,0x98,0x98,0x80,0x8F,0x9F,0x90,0x90,0x98,0x88,0x80,0x8F,0x9F,0x90,0x90,
0x9F,0x8F,0x80,0x9F,0x9F,0x80,0x80,0x9F,0x9F,0x80,0x80,0x9F,0x9F,0x80,0x80,0xFF,

};

const uint8_t pic[]= { /* 0X22,0X01,0X80,0X00,0X40,0X00, */
0X00,0X00,0X00,0X00,0X00,0X30,0X00,0X60,0X00,0X40,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X02,0X00,0X05,0X08,0X02,0X20,
0X44,0X88,0X00,0X90,0X20,0X00,0X40,0X00,0XC0,0X80,0XC0,0X80,0XC0,0X80,0X80,0X80,
0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0XC0,0XF4,0XF2,0X79,0X34,0XA9,0XA0,
0XC1,0X84,0X90,0X24,0X00,0X40,0X15,0X08,0X1C,0X18,0X3D,0X79,0X73,0X7B,0XF3,0XF7,
0XE7,0XF7,0XC7,0XDF,0X96,0X1E,0X7C,0X78,0XF8,0XE0,0XE0,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0XFC,0XFC,0X40,0X40,0X40,0X40,0XFC,0XFC,0X00,0X00,0X00,0X00,0XFC,0XFC,0X44,
0X44,0X44,0X44,0X00,0X00,0X00,0X00,0XFC,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFC,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XF8,0X18,0X04,0X04,0X04,0X04,0X18,
0XF8,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XC0,0XF4,0XFC,0X7F,0X1F,0XC3,0XD3,0XFC,0XFC,0XFF,0X37,0X3F,
0X0F,0X03,0X02,0X01,0X02,0X80,0X04,0X00,0X0C,0X08,0X11,0XE0,0X80,0X40,0X10,0X01,
0X05,0X07,0X1F,0X97,0X5F,0X3F,0X7F,0XFE,0XF8,0XF1,0XC7,0X1F,0X5F,0XFE,0XF8,0XA0,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0F,0X0F,0X00,0X00,0X00,0X00,0X0F,0X0F,0X00,0X00,0X00,0X00,0X0F,0X0F,0X08,
0X08,0X08,0X08,0X00,0X00,0X00,0X00,0X0F,0X0F,0X08,0X08,0X08,0X08,0X00,0X00,0X00,
0X0F,0X0F,0X08,0X08,0X08,0X08,0X00,0X00,0X01,0X07,0X06,0X08,0X08,0X08,0X08,0X06,
0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X74,0XFF,0XFF,0XAF,0X00,0XFD,0X7F,0XFF,0XFE,0XFB,0XE0,0XC0,0X80,
0X00,0X00,0X00,0X02,0X03,0X07,0X02,0X0F,0X38,0XC9,0X8D,0X13,0X10,0X20,0X00,0X10,
0X8C,0X00,0X41,0X80,0X20,0X10,0X81,0X0B,0X9E,0X1F,0X3F,0X7C,0X01,0XD7,0XFF,0XFF,
0XBA,0X00,0X00,0X00,0X00,0XC0,0XF0,0X70,0XF0,0XC0,0X00,0X00,0X00,0X00,0X00,0XF0,
0XF0,0X10,0X10,0X10,0XF0,0XE0,0X00,0X00,0X00,0X00,0XF0,0XF0,0X10,0X10,0X10,0X20,
0XE0,0XC0,0X00,0X00,0X00,0XF0,0XF0,0XF0,0X00,0X00,0X00,0X00,0XF0,0XF0,0X00,0X00,
0X10,0X10,0XF0,0XF0,0X10,0X10,0X00,0X00,0X00,0XF0,0XF0,0XE0,0XC0,0X00,0X00,0XF0,
0XF0,0X00,0X00,0X00,0X80,0XE0,0X60,0X10,0X10,0X10,0X10,0X60,0XE0,0X80,0X00,0X00,
0X00,0X00,0X00,0X01,0X0F,0X7F,0XFF,0XF8,0XC1,0X97,0X3F,0X7F,0XED,0XFF,0XDF,0X7F,
0XFE,0XFC,0XF0,0XE0,0X80,0X00,0X00,0X00,0X00,0X00,0X01,0X27,0XF8,0X60,0X04,0X00,
0X0C,0X01,0XC8,0XE1,0XD2,0XC0,0X84,0X10,0X40,0X14,0X51,0X48,0XF0,0XFD,0X79,0X2F,
0X00,0X00,0X00,0X38,0X3E,0X07,0X04,0X04,0X05,0X07,0X3E,0X30,0X00,0X00,0X00,0X3F,
0X3F,0X02,0X02,0X0E,0X1F,0X39,0X30,0X00,0X00,0X00,0X3F,0X3F,0X20,0X20,0X20,0X10,
0X1F,0X0F,0X03,0X00,0X00,0X07,0X1F,0X1F,0X20,0X20,0X20,0X10,0X1F,0X0F,0X00,0X00,
0X20,0X20,0X3F,0X3F,0X20,0X20,0X00,0X00,0X00,0X3F,0X00,0X00,0X03,0X07,0X1E,0X3F,
0X3F,0X00,0X00,0X00,0X07,0X1F,0X18,0X20,0X20,0X20,0X20,0X18,0X1F,0X07,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X03,0X0F,0X1F,0X1E,0X7C,0X79,0XF3,0X67,0XEF,
0XCB,0XDF,0XFF,0XBF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0X00,0X00,0X00,
0X00,0XD7,0XFD,0XCF,0XCF,0XF3,0XF3,0XB9,0X7E,0X3E,0X0E,0X08,0X04,0X09,0X12,0X00,
0X08,0X50,0X20,0X40,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,
0X01,0X07,0X03,0X04,0X04,0X0C,0X04,0X0C,0X04,0X0C,0X0C,0X0C,0X04,0X0C,0X04,0X04,
0X04,0X05,0X03,0X03,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};


void dispSendCmd(uint8_t buf[], uint8_t len) {
  byte error;
  byte i;
  Wire.beginTransmission(DISP_CMD_ADDR);
  for (i=0; i < len; i++) {
    Wire.write(buf[i]);
  }
  error = Wire.endTransmission();
  if (error == 0) {
    Serial.print("Sent n bytes: ");
    Serial.println(len);
  } else {
    Serial.print("Error sending n bytes: ");
    Serial.println(len);
  }
}

void dispSendCmd(uint8_t c) {
  byte error;
  Wire.beginTransmission(DISP_CMD_ADDR);
  Wire.write(c);
  error = Wire.endTransmission();
  if (error == 0) {
    Serial.print("Successfully send command: ");
    Serial.println(c);
  } else {
    Serial.print("Error sending command: ");
    Serial.println(c);
  }  
  delay(10);
}

void dispSendData(uint8_t c) {
  byte error;
  Wire.beginTransmission(DISP_DAT_ADDR);
  Wire.write(c);
  error = Wire.endTransmission();
  if (error == 0) {
    Serial.print("Successfully send data: ");
    Serial.println(c);
  } else {
    Serial.print("Error sending data: ");
    Serial.println(c);
  }  
}

void dispSetAllPixelOn(uint8_t onOff) {
  byte c = 0xa4 | (onOff & 1);
  byte error;
  byte z = 0x00;
  Wire.beginTransmission(DISP_CMD_ADDR);
  Wire.write(c);
  error = Wire.endTransmission();
  if (error == 0) {
    Serial.print("Successfully set all pixel on ");
    Serial.println(c);
  } else {
    Serial.print("Error setting all pixel on ");
    Serial.println(c);
  }  
}



void dispSetPageAddr(uint8_t addr) {
  addr=0xb0 | addr;
  dispSendCmd(addr);
}

void dispSetColumnAddr(uint8_t addr) {
  uint8_t buf[2];
  buf[0] = 0x10 | (addr >> 4);
  buf[1] = (0x0f & addr) | 0x04;
  dispSendCmd(buf,2);
}

void dispPicture(const uint8_t pic[]) {
  uint8_t i, j, num=0, error;
  for (i=0; i<8; i++) {
    dispSetPageAddr(i);
    dispSetColumnAddr(0);
    Wire.beginTransmission(DISP_DAT_ADDR);
    for (j=0; j<0x80; j++) {
//    for (j=0; j<1; j++) {
      Wire.write(pic[i*0x80+j]);
    }
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.println("Sent Data bytes ");
    } else {
      Serial.println("Error sending data bytes ");
    }  
  }
}

void dispFixedByte(uint8_t b) {
  uint8_t i, j, num=0, error;
  for (i=0; i<8; i++) {
    dispSetPageAddr(i);
    dispSetColumnAddr(0);
    Wire.beginTransmission(DISP_DAT_ADDR);
    for (j=0; j<0x80; j++) {
//    for (j=0; j<1; j++) {
      Wire.write(b);
    }
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.println("Sent Data bytes ");
    } else {
      Serial.println("Error sending data bytes ");
    }  
  }
}


void dispSetPowerControl(void) {
  byte c = 0x2f;
  byte error;
  byte z = 0x00;
  Wire.beginTransmission(DISP_CMD_ADDR);
  Wire.write(c);
  error = Wire.endTransmission();
  if (error == 0) {
    Serial.print("Successfully set Power Control ");
    Serial.println(c);
  } else {
    Serial.print("Error setting Power Control ");
    Serial.println(c);
  }  
}

void dispInit(void) {
  dispSendCmd(0x2f);            //Power_Control
  dispSendCmd(0xA2);            // BIAS Setting   A2(1/9);A3(1/7)
  dispSendCmd(0xA0);            // Set SEG Direction. It was initially 0xA1 
  dispSendCmd(0xC8);            // Set COM Direction. It was initially 0xC8
                          
  dispSendCmd(0x24);            // Set VLCD Resistor Ratio    0-7
  dispSendCmd(0x81);            // Set Electronic Volume
  dispSendCmd(37);              // Contrast level  0~63
  dispSendCmd(0x40);            // Initial_Dispay_Line
  dispSendCmd(0xAF);            // Display ON

  // dispSetAllPixelOn(0);
  dispPicture(pic);
  // dispFixedByte(0x03);
  // delay(2000);
  // dispSetAllPixelOn(1);
}



#endif
