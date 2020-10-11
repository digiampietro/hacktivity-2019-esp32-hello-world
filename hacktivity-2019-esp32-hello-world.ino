#include <Wire.h>
#include "ST7565-i2c.h"
#include "SAMD-i2c.h"

#define MYLED         21
#define DISP_CMD_ADDR 0x38
#define DISP_DAT_ADDR 0x39
#define SAMD_ADDR     0x30
#define PIN_I2C_INT   25
#define PIN_SDA       23
#define PIN_SCL       22

#define MY_HOSTNAME "hacktivity2019"

// display object
ST7565 glcd(DISP_CMD_ADDR, DISP_DAT_ADDR);

// samd (co-processor) objet
SAMD   samd(SAMD_ADDR);


char *mymenu[7];

// Scan the i2c bus, not used
void i2cScan(void) {
  byte error, address;
  int  nDevices;
  int currline=0;
  char s[22];

  glcd.clear();
  glcd.drawstring(0, currline, "I2C Scanning ...", FONT_SMALL);
  currline++;
  glcd.display();
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      sprintf(s," found at 0x%02X", address);
      glcd.drawstring(0, currline, s, FONT_SMALL);
      glcd.display();
      currline++;
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      sprintf(s," error at 0x%02X", address);
      glcd.drawstring(0, currline, s, FONT_SMALL);
      glcd.display();
      currline++;
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
      glcd.drawstring(0, currline, "No devices found", FONT_SMALL);
      glcd.display();
      currline++;
      Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
    glcd.drawstring(0, currline, "Done", FONT_SMALL);
    currline++;
  }
  samd.updateStatus();
}

// wait for the "Enter" button to be pressed
void waitEnter(void) {
  glcd.drawstring( 0, 7, "Press ENTER to cont.", FONT_SMALL); 
  glcd.display();
  while (not samd.isPressed(BTN_ENTER)) {
    samd.updateStatus();
  }
  samd.buzz(1000,100);
}


// nitems: maximum index of items (number of items minus 1)

int fullmenu(uint8_t nitems, char *items[], uint8_t fontsize) {
  uint8_t i;
  uint8_t dstart;    // first line of the string array on display
  uint8_t dend;      // last line of the string array on display
  uint8_t dselect;   // selected line of the string array on display
  uint8_t dlines;    // number of lines of the array in the display
  uint8_t linesize;  // line size (1 for small font 2 for big font)
  bool    notpressed = true;
  char    s[80];

  dlines =   LCDHEIGHT / (fontsizey[fontsize]+fontspacey[fontsize]) -1; // 0 based
  linesize = (fontsizey[fontsize]+fontspacey[fontsize]) / 8;

  dstart  = 0;
  dend    = (nitems > dlines) ? dlines : nitems;
  dselect = 0;

  Serial.println("---> Entered fullmenu");
  sprintf(s,"---> dstart: %d, dend: %d, dselect: %d, linesize: %d",dstart,dend,dselect,linesize);
  Serial.println(s);
  
  samd.updateStatus();
  while (samd.isUp(BTN_ENTER)) {  // exit from the loop when ENTER is pressed
    delay(20);
    samd.updateStatus();
    notpressed=true;
    Serial.println("---> inside while loop");
    sprintf(s,"---> dstart: %d, dend: %d, dselect: %d, notpressed: %d, millis: %d",dstart,dend,dselect,notpressed,millis());
    Serial.println(s);
    glcd.clear();
    for (i=dstart; i <= dend; i++) {
      sprintf(s,"---> i: %d, param2: %d", i, (i-dstart)*linesize);
      Serial.println(s);
      glcd.drawstring(0, (i-dstart)*linesize, items[i], fontsize);
      Serial.println(items[i]);
    }
    
    for (i=0; i<linesize; i++) {   // invert selected line  
      glcd.negativeline((dselect-dstart)*linesize+i);
    }
    glcd.display();

    while (notpressed) {
      if (samd.isPressed(BTN_DOWN)) {
	notpressed=false;
	dselect++;
	if (dselect > nitems) {
	  dselect=nitems;
	  samd.buzz(1000,100);
	}
	if (dselect > dend) {
	  dstart++;
	  dend++;
	}
      } else if (samd.isPressed(BTN_UP)) {
	notpressed=false;
	dselect--;
	if ((dselect < 0) or (dselect > nitems)) {
	  dselect=0;
	  samd.buzz(1000,100);
	}
	if (dselect < dstart) {
	  dstart--;
	  dend--;
	}
      } else if (samd.isPressed(BTN_ENTER)) {
	notpressed=false;
      } else if (samd.isPressed(BTN_EXIT)) {    // left is escape
	notpressed=false;
	Serial.println("---> Exit Button pressed fullmenu returning -1");
	return -1;
      } else {
	delay(20);
	samd.updateStatus();
      }
    }
  }
  sprintf(s,"--> Exiting fullmenu, returning: %d",dselect);
  Serial.println(s);
  return dselect;
}


void drawRectangles(void) {
  int i;
  glcd.clear();
  for (i=0; i<8; i++) {
    glcd.drawrect(i*8,i*4,128-(i*16),64-(i*8),BLACK);
    glcd.display();
  }
}

void drawLines(void) {
  int i;
  glcd.clear();
  for (i=0; i<8; i++) {
    glcd.drawline(0, 0, i*16, 64, BLACK);
    glcd.display();
  }
}

void drawCircles(void) {
  int i;
  glcd.clear();
  for (i=0; i<8; i++) {
    glcd.drawcircle(63, 31, i*4, BLACK);
    glcd.display();
  }
}

void drawText(void) {
  glcd.clear();
  glcd.drawstring(30, 0, "Hello",  FONT_BIG);
  glcd.drawstring(27, 2, "World!", FONT_BIG);
  glcd.drawstring(43, 5, "Hello",  FONT_SMALL);
  glcd.drawstring(41, 6, "World!", FONT_SMALL);
  glcd.display();
}


void setup() {
  int i, j;
  char s[100];
  
  mymenu[0]="Rectangles";
  mymenu[1]="Circles";
  mymenu[2]="Lines";
  mymenu[3]="Text";
  mymenu[4]="I2C scan";
  mymenu[5]="LCD Light";
  mymenu[6]="Contrast";
  
  pinMode(MYLED, OUTPUT);
  Serial.begin(115200);
  delay(100);
  Serial.println("ESP32 Starting");

  pinMode(PIN_I2C_INT, INPUT);
  Wire.begin(PIN_SDA, PIN_SCL);

  samd.ledsOff();  // switch off all the leds
  samd.setBl(250); // switch on the display back light

  // initialize and set the contrast to 0x20
  glcd.i2cbegin(0x20);
  delay(1000);
  glcd.display(); // show splashscreen
  delay(3000);

  
  // read button status
  samd.updateStatus();
  glcd.clear();
  // draw a lenghty string
  glcd.drawstring(0, 0, "The quick",   FONT_BIG);
  glcd.drawstring(0, 2, "brown fox",   FONT_BIG);
  glcd.drawstring(0, 4, "jumps over",  FONT_SMALL);
  glcd.drawstring(0, 5, "the lazy dog",FONT_SMALL);
  glcd.negativeline(5);
  // glcd.negativeline(3);
  glcd.display();
  waitEnter();
  
  glcd.clear();
  glcd.display();
}

int menuGetValue(char msg[], int vini, int vmin, int vmax, int vstep, void (*cbUpdate)(int bl)) {
  int  vout=vini;
  bool toupdate = true;
  char vstr[12];
  samd.updateStatus();
  while (not (samd.isPressed(BTN_ENTER) or samd.isPressed(BTN_EXIT))) {
    if (toupdate) {
      glcd.clear();
      glcd.drawstring(0,0,msg,FONT_BIG);
      sprintf(vstr,"%10d",vout);
      glcd.drawstring(0,3,vstr,FONT_BIG);
      glcd.display();
      cbUpdate(vout);
      toupdate=false;
    }
    if (samd.isPressed(BTN_UP)) {
      toupdate=true;
      vout+=vstep;
      if (vout > vmax) {
	vout = vmax;
      }
    } else if (samd.isPressed(BTN_DOWN)) {
      toupdate=true;
      vout-=vstep;
      if (vout < vmin) {
	vout = vmin;
      }
    }
    samd.updateStatus();
  }
  if (samd.isPressed(BTN_EXIT)) {
    vout = vini;
  }
  return vout;
}

int     cycle      = 0;  //cycle number
uint8_t currled    = 0;  //current led to switch on
int     lastmillis = 0;  //previous millisecond counter

void mysetBl(int bl) {
  samd.setBl(bl);
}

void mysetContrast(int contrast) {
  glcd.st7565_set_brightness(contrast);
}

void loop() {
  int     li,m;
  uint8_t col;
  char    msg[15];
  static int status   = 0;    // 0: display menu
  static int bl       = 250;  // LCD Backlight
  static int contrast = 0x20;

  if ((millis() - lastmillis) > 200) {
    lastmillis = millis();

    // get info on touch button pressed
    samd.updateStatus();
    if (status == 0) {
      m=fullmenu(6, mymenu, FONT_BIG);
      samd.updateStatus();
      switch (m) {
      case 0: drawRectangles();
	waitEnter();
	break;
      case 1: drawCircles();
	waitEnter();
	break;
      case 2: drawLines();
	waitEnter();
	break;
      case 3: drawText();
	waitEnter();
	break;
      case 4: i2cScan();
	waitEnter();
	break;
      case 5: bl=menuGetValue("LCD Light",bl,0,255,5,&mysetBl);
	Serial.print("---> New back light value: ");
	Serial.println(bl);
	samd.setBl(bl);
	break;
      case 6: contrast=menuGetValue("Contrast",contrast,0,63,8,&mysetContrast);
	Serial.print("---> Contrast value: ");
	Serial.println(bl);
	mysetContrast(contrast);
	break;
      }
      status = 1;
      glcd.clear();
      glcd.drawstring(0,7,"Press RIGHT for menu",FONT_SMALL);
    }

    // switch off the previous cycle currled
    if (not samd.isDown(BTN_LEFT)) samd.setLed(currled,0,0,0);

    // Serial.println(" ");
    currled = cycle % 6;
    col     = cycle % 3;

    if (samd.isDown(BTN_DOWN))  col=0;
    if (samd.isDown(BTN_EXIT))  col=1;
    if (samd.isDown(BTN_ENTER)) col=2;

    if (digitalRead(PIN_I2C_INT) == HIGH ) {
      if ( col == 0) samd.setLed(currled,1,0,0);
      if ( col == 1) samd.setLed(currled,0,1,0);
      if ( col == 2) samd.setLed(currled,0,0,1);
      if (samd.isDown(BTN_RIGHT)) {
	samd.buzz(1000,100);
	status = 0;
      }
      if (samd.isDown(BTN_UP))    samd.setBl(0); else samd.setBl(bl);
      if (currled == 0) {
	sprintf(msg,"STEP: %5d", cycle / 6);
	glcd.drawstring(0,2,msg,FONT_BIG);
	glcd.display();
      }
    } else {
      Serial.println("PIN_I2C_INT is disabled");   
    }
    cycle++;
  }
}


