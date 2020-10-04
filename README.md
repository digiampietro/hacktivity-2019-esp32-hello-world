# About

In this repository you will find much more than a simple "Hello World"
program for the
[Hacktivity 2019 badge](https://hacktivity.com/index.php/badge/) based
on the Espressif ESP32 powerful microcontroller.

You will find:

* a library to manage the display;

* a library to talk to the SAMD21 co-processor to light on board RGB
  LEDs, to play sounds on the buzzer and to read the touch buttons;

* a sample program
  ([hacktivity-2019-esp32-hello-world.ino](./hacktivity-2019-esp32-hello-world.ino))
  with examples of:

  * writing text to the display using big and small fonts;

  * drawing lines, rectangles and circles on the display;

  * lighting LEDs on the board;

  * playing sounds on the buzzer;

  * reading the status of the touch buttons on the board

# Very Important

But first you have to replace the firmware on the ARM based Atmel
SAMD21 co-processor downloading it and following instructions on the
[Hacktivity 2019 SAMD unofficial firmware](https://github.com/digiampietro/hacktivity-2019-samd-firmware-unofficial)
GitHub repository.

And, in the Arduino IDE, you have to select *Tools -> Board -> ESP32
Arduino -> ESP32 Dev Module*; the Arduino automatic Board detection
can wrongly detect the board and can brick your device after uploading
the sketch.

# Table of Content

  - [About](#about)
  - [Very Important](#very-important)
  - [Table of Content](#table-of-content)
  - [The Hacktivity 2019 badge](#the-hacktivity-2019-badge)
  - [The ST7565-i2c LCD Arduino library to drive the LCD display](#the-st7565-i2c-lcd-arduino-library-to-drive-the-lcd-display)
    - [How To Use](#how-to-use)
      - [Writing text to the display](#writing-text-to-the-display)
      - [Drawing Lines](#drawing-lines)
      - [Drawing rectangles](#drawing-rectangles)
      - [Drawing circles](#drawing-circles)
      - [Filling rectangles](#filling-rectangles)
      - [Filling circles](#filling-circles)
      - [Setting a single pixel](#setting-a-single-pixel)
      - [Drawing a bitmap](#drawing-a-bitmap)
      - [Clearing the display buffer](#clearing-the-display-buffer)
      - [Displaying the buffer](#displaying-the-buffer)
  - [The SAMD-i2c LCD Arduino library to talk to the co-processor](#the-samd-i2c-lcd-arduino-library-to-talk-to-the-co-processor)
    - [How To Use](#how-to-use)
      - [Get the touch buttons state](#get-the-touch-buttons-state)
      - [Switch on a LED and set his color](#switch-on-a-led-and-set-his-color)
      - [Switch off all the LEDs](#switch-off-all-the-leds)
      - [Switch on or off the display back light](#switch-on-or-off-the-display-back-light)
      - [Play a tone on the buzzer](#play-a-tone-on-the-buzzer)
  - [The Makefile](#the-makefile)

# The Hacktivity 2019 badge

This badge was distributed at the Hacktivity 2019 Security Conference
attendees, it is based on the popular ESP32 processor by Espressif, has
a small, but nice display, and includes an ARM-based chip, the Atmel
ATSAMD21G16B co-processor, that manages the USB serial connection, the
touch buttons, and the LEDs on the board. The co-processor
communicates with the main ESP32 processor trough the I2C bus.

On the
[above](https://github.com/digiampietro/hacktivity-2019-samd-firmware-unofficial)
repository you will find detailed information on this board, including
connectors pinout and block diagram.

# The ST7565-i2c LCD Arduino library to drive the LCD display

The Hacktivity 2019 badge has an ERC12864-10 display that uses the
ST7565 chip to drive it. Arduino has some libraries able to drive this
LCD display, but, unfortunately, none of them includes support for the
I2C bus; they support the more popular SPI bus connection, but, in our
board, the display is connected through the I2C serial bus.

To solve this issue I downladed a very old
[Adafruit ST7565 library](https://github.com/adafruit/ST7565-LCD),
supporting the SPI bus and modified it to support the I2C serial bus
and did some other modifications:

* support for the I2C serial bus

* added support for big fonts (10x15) in addition to the existing
  small fonts (5x7). With small fonts you can have 8 lines of 21
  characters, with big fonts you can have 4 lines of 11 characters;

* fixed some minor issues.

This library is implemented in two files on this repository that
reside in the same directory as the main Arduino sketch:

* an header file [ST7565-i2c.h](./ST7565-i2c.h)

* and a c++ file [ST7565-i2c.cpp](./ST7565-i2c.cpp)

## How To Use

First you have to include the header file on your Arduino sketch and
the Wire library header file, needed to use the I2C bus:

```
#include <Wire.h>
#include "ST7565-i2c.h"
```

Then you have to declare an object of type **ST7565** with the
following syntax:

```
ST7565 glcd(DISP_CMD_ADDR, DISP_DAT_ADDR);
```

where:

**DISP\_CMD\_ADDR** is the I2C command address of the display (0x38 on
  this board)

**DISP\_DAT\_ADDR** is the I2C data address of the display (0x39 on
  this board)

finally, on the **setup** function you have to initialize the I2C bus:

```
Wire.begin(PIN_SDA, PIN_SCL);
```

where **PIN\_SDA** and **PIN\_SCL** are the pin number of the two I2C
signals **SDA** and **SCL**, in our board these values are 23 and 22
respectively.

Then, still in the **setup** function,  you have to call the object
initialization function:

```
glcd.i2cbegin(contrast);
```

where **contrast**  is the contrast value. 0x18 is a good starting
point, more information on the related
[datasheet](https://www.topwaydisplay.com/sites/default/files/2020-02/LM6059BCW.pdf).

Every drawing operations on this library will modify an internal
buffer whose initial value is a nice "Hello Arduino" image; the
content of this internal buffer will be transferred to the display
only calling the **display** method as in the following example:

```
glcd.display();
```

### Writing text to the display

The library functions to write text consider the display having one
horizonal coordinate (*x*) with a 1 pixel resolution, ranging from 0
to 127 and one vertical coordinate (*line*) with an 8 pixel resolution
ranging from 0 to 7 that gives 8 text lines using the small font.

Please note that the big font will use two adjacent vertical lines.

The function to write text to the display is:

```
void drawstring(uint8_t x, uint8_t line, char *c, uint8_t fontsize);
```

**x** is the horizontal position, ranging from 0 to 127;

**line** is the vertical position, ranging from 0 to 7;

***c** is a null terminated string;

**fontsize** can be FONT\_SMALL or FONT\_BIG. With FONT\_SMALL you can
  have 8 lines of 21 characters per line; with FONT\_BIG you can have
  4 lines of 11 characters per line, so FONT\_BIG will need two
  adjacent lines.

Only ASCII characters will be correctly printed; if the the string is
longer than a single line, it will wrap.

**fontsize** can be omitted and it defaults to FONT\_SMALL.

An example:

```
glcd.drawstring(43, 5, "Hello",  FONT_SMALL);
glcd.display();
```

### Drawing Lines

To draw lines there is the *drawline* function:

```
void drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
```

where

**x0** starting *x* coordinate (from 0 to 127);

**y0** starting *y* coordinate (from 0 to 63);

**x1** and **y1** ending coordinates;

**color** in a monochrome display is usually BLACK;

example usage:

```
glcd.drawline(3, 7, 40, 60, BLACK);
glcd.display();
```

### Drawing rectangles

To draw rectangles there is the *drawrect* function:

```
void drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
```

Where

**x**, **y** are the top left corner of the rectangle;

**w**, **h** are the width and heigh of the rectangle in pixels;

**color** in a monochrome display is usually BLACK;

example usage:

```
glcd.drawrect(8,4,110,50,BLACK);
glcd.display();
```

### Drawing circles

To draw circles there is the function *drawcircle*:

```
void drawcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);

```

where:

**x0**, **y0** are the center of the circle;

**r** is the radius;

**color** in a monochrome display is usually BLACK;

example usage:

```
glcd.drawcircle(63, 31, 12, BLACK);
glcd.display();
```

### Filling rectangles

To fill a rectangle region there is the function *fillrect*, similar to the *drawrect* function:

```
void fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
```

### Filling circles

To fill a circular region there is the function *fillcircle*, similar to the *drawcircle* function:

```
void fillcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
```

### Setting a single pixel

To set a single pixel there is the *setpixel* function:

```
void setpixel(uint8_t x, uint8_t y, uint8_t color);
```

### Drawing a bitmap

To draw a bitmap there is the *drawbitmap* function:

```
void drawbitmap(uint8_t x, uint8_t y, 
		  const uint8_t *bitmap, uint8_t w, uint8_t h,
		  uint8_t color);
```
		  
### Clearing the display buffer

To clear the display buffer there is the function *clear*:

```
void clear();
```

This clear the buffer only, to clear also the displa you have to use
the *display* function, as in the following example:

```
glcd.clear();
glcd.display();
```

### Displaying the buffer

Every operation, on this library, is done on the buffer, to have the
buffer displayed on the display you have to use the *display*
function:

```
void display();
```

example usage:

```
glcd.display();
```

# The SAMD-i2c LCD Arduino library to talk to the co-processor

In this board the main processor, the ESP32, talks to the SAMD21
co-processor using the I2C serial bus.

LEDs, touch buttons and the buzzer are connected to the SAMD21
co-processor, this means that the ESP32 processor has to talk to the
SAMD21 co-processor to interact with these peripherals. To do so there
is a simple I2C protocol (details on the
[SAMD21 firmware repostiory](https://github.com/digiampietro/hacktivity-2019-samd-firmware-unofficial))
that is encapsulated in the *SAMD-i2c* library.

This library is implemented in two files on this repository that
reside in the same directory as the main Arduino sketch:

* a heade file [SAMD-i2c.h](./SAMD-i2c.h)

* a c++ file [SAMD-i2c.cpp](./SAMD-i2c.cpp)

## How To Use

First you have to include the header file on you Arduino sketch and
the Wire library header file, needed to use the I2C bus:

```
#include <Wire.h>
#include "SAMD-i2c.h"

```

Then you have to declare an object of type **SAMD** with the following
syntax:

```
SAMD   samd(SAMD_ADDR);
```

where:

**SAMD_ADDR** is the I2C address of the SAMD21 co-processor (0x30 on
  this board)

finally, on the **setup** function you have to initialize the I2C bus,
if not already done:

```
Wire.begin(PIN_SDA, PIN_SCL);
```

where **PIN\_SDA** and **PIN\_SCL** are the pin number of the two I2C
signals **SDA** and **SCL**, in our board these values are 23 and 22
respectively.

### Get the touch buttons state

In the header files there are pre-processor definitions for each touch
button:

```
#define BTN_ENTER  0
#define BTN_LEFT   1
#define BTN_RIGHT  2
#define BTN_EXIT   3
#define BTN_DOWN   4
#define BTN_UP     5
```

To update the touch buttons state you have to call the
**updateStatus** function as in the following example:

```
samd.updateStatus();
```

Once the status has been updated you can inquire the status of a
single touch button with one of the following functions:

```
bool isDown(uint8_t btn);
```

*isDown* returns true if the button *btn* is down (currently pressed);

```
bool isUp(uint8_t btn);
```

*isUp* returns true if the button *btn* is up (currently not pressed);

```
bool isPressed(uint8_t btn);
```

*isPressed* returns true if the button *btn* is currently pressed, but
was not pressed on the previous state: it has just changed state;

```
bool isReleased(uint8_t btn);
```

*isReleased* return true if the button *btn* is curently not pressed,
but was pressed on the previous state: it has just changed state.

Example usage:

```
if (samd.isDown(BTN_UP)) {
  // do something
}
```

### Switch on a LED and set his color

To set a color LED you have to use the following function:

```
bool setLed(uint8_t id, uint8_t r, uint8_t g, uint8_t b);
```

where:

**id** is the LED id, from 0 to 5

**r** is the value for the RED color: 0 means switched off, 1 higher
intensity, 255 lower intensity;

**g** is the value for the GREEN color: 0 means switched off, 1 higher
intensity, 255 lower intensity;

**b** is the value for the BLUE color: 0 means switched off, 1 higher
intensity, 255 lower intensity;

**returned value** is *False* in case of I2C communication errors,
  otherwise is *True*

Example usage to set the LED number 4 to GREEN color, maximum
intensity:

```
samd.setLed(4,0,1,0);
```

### Switch off all the LEDs

To switch off all the LEDs with a single function call:

```
samd.ledsOff();
```

### Switch on or off the display back light

To switch on or off the display back ligth there is the funcion *setBl*:

```
bool setBl(uint8_t blstatus);
```

where:

**blstatus** is the back light intensity: *0* swtichd off, *255*
  maximum intensity;


**returned value** is *False* in case of I2C communication errors,
  otherwise is *True*;

example usage to set display back light near the maximum intensity:

```
samd.setBl(250);
```

### Play a tone on the buzzer

To play a tone on the buzzer there is the *buzz* function:

```
bool buzz(uint16_t freq, uint16_t time);
```

where:

**freq** is the frequency in Hertz;

**time** is the duration in milliseconds;

**returned value** is *False* in case of I2C communication errors,
  otherwise is *True*;

example usage to play a 1khz tone for 100 milliseconds:

```
samd.buzz(1000,100);
```

# The Makefile

The included Makefile, based on the *arduino-cli*, is not needed when
using the Arduino IDE, but can be useful if you, like me, prefer to
edit files with your preferred editor (*Emacs* in my case) and then
compile and upload the sketch using a traditional Makefile.

Typical usage is:

```
make compile  // compile the sketch without uploading
make upload   // upload the sketch using the USB serial interface
make          // compile and upload the sketch
make V=1      // verbose execution
```

It can be useful to read the content of this file that has a lot of
comments.

