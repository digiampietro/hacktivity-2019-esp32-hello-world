
#include <Wire.h>
#include "ST7565-i2c.h"
#include "SAMD-i2c.h"

// Needed for OTA setup
#include "ota.h"

#define MYLED         21
#define DISP_CMD_ADDR 0x38
#define DISP_DAT_ADDR 0x39
#define SAMD_ADDR     0x30
#define PIN_I2C_INT   25
#define PIN_SDA       23
#define PIN_SCL       22

#define MY_HOSTNAME "hacktivity2019"

ST7565 glcd(DISP_CMD_ADDR, DISP_DAT_ADDR);
SAMD   samd(SAMD_ADDR);

bool btnUp     = LOW;
bool btnDown   = LOW;
bool btnExit   = LOW;
bool btnEnter  = LOW;
bool btnLeft   = LOW;
bool btnRight  = LOW;

// print dots during the Wifi Connection phase

void myProgress () {
  //  Serial.print(".");
}


void readSamd(void) {
  byte c1;
  byte c2;
  Wire.requestFrom(SAMD_ADDR,2);
  c1=Wire.read();
  c2=Wire.read();
  Serial.print("read from samd: ");
  Serial.print(c1);
  Serial.print(" ");
  Serial.println(c2);
  btnEnter = bitRead(c1,0);
  btnLeft  = bitRead(c1,1);
  btnRight = bitRead(c1,2);
  btnExit  = bitRead(c1,3);
  btnDown  = bitRead(c1,4);
  btnUp    = bitRead(c1,5);
}

void buzz(void) {
  byte c = 0x03;
  byte z = 0x00;
  byte fh = 4;
  byte fl = 0;
  byte dh = 1;
  byte dl = 0;
  byte error;
  Wire.beginTransmission(SAMD_ADDR);
  Wire.write(c);
  Wire.write(fh);
  Wire.write(fl);
  Wire.write(dh);
  Wire.write(dl);
  error = Wire.endTransmission();  
  if (error == 0) {
    Serial.print("Successfully send buzz command ");
    Serial.println(c);
  } else {
    Serial.print("Error sending buzz command ");
    Serial.println(c);
  }  
}


void ledOff(void) {
  byte c = 0x04;
  byte z = 0x00;
  byte error;
  Wire.beginTransmission(SAMD_ADDR);
  Wire.write(c);
  error = Wire.endTransmission();  
  if (error == 0) {
    Serial.print("Successfully set ledoff to ");
    Serial.println(c);
  } else {
    Serial.print("Error set ledoff to ");
    Serial.println(c);
  }  
}

void setBl(uint8_t blstatus) {
  byte c = 0x02;
  byte error;
  byte z = 0x00;
  Wire.beginTransmission(SAMD_ADDR);
  Wire.write(c);
  Wire.write(blstatus);
  error = Wire.endTransmission();
  if (error == 0) {
    Serial.print("Successfully set blstatus to ");
    Serial.println(blstatus);
  } else {
    Serial.print("Error setting blstatus to ");
    Serial.println(blstatus);
  }  
}

void i2cScan(void) {
  byte error, address;
  int  nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
  randomSeed(666);     // whatever seed
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(128);
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
  }
}

void setLed(uint8_t id, uint8_t r, uint8_t g, uint8_t b) {
  byte c = 0x01;
  byte z = 0x00;
  byte error;
  Wire.beginTransmission(SAMD_ADDR);
  Wire.write(c);
  Wire.write(id);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  error = Wire.endTransmission();  
  if (error == 0) {
    Serial.print("Successfully set led ");
    Serial.println(id);
  } else {
    Serial.print("Error set led ");
    Serial.println(id);
  }    
}

void waitKey(void) {
  char  c;
  while (Serial.available() <= 0) {
    //Serial.print(".");
  }
  Serial.println(".");
    // read the incoming byte:
  c = Serial.read();
}

void waitMsg(char msg[]) {
  Serial.print(msg);
  Serial.print(" - press a Key ");
  waitKey();
}

void setup() {
  int i, j;
  char s[100];
  // put your setup code here, to run once:
  pinMode(MYLED, OUTPUT);
  Serial.begin(115200);
  delay(100);
  Serial.println("ESP32 Starting");

  pinMode(PIN_I2C_INT, INPUT);
  Wire.begin(PIN_SDA, PIN_SCL);
  
  // i2cScan();
  // Serial.println("dispInit");
  //dispInit();
  // waitMsg("ledOff;setBl");
  ////ledOff();
  ////setBl(250);

  samd.ledsOff();

  // initialize and set the contrast to 0x18
  // waitMsg("glcd.ic2begin");
  glcd.i2cbegin(0x18);
  
  // Serial.println("glcd.display, press a key");
  // waitKey();
  glcd.display(); // show splashscreen
  delay(1000);

  // waitMsg("glcd.clear");
  glcd.clear();
  glcd.drawstring(30, 1, "Hello",  FONT_BIG);
  glcd.drawstring(27, 3, "World!", FONT_BIG);
  glcd.drawstring(43, 5, "Hello",  FONT_SMALL);
  glcd.drawstring(41, 6, "World!", FONT_SMALL);
  glcd.display();
  delay(2000);

  // draw rectangles
  glcd.clear();
  for (i=0; i<8; i++) {
    glcd.drawrect(i*8,i*4,128-(i*16),64-(i*8),BLACK);
    glcd.display();
    delay(10);
  }
  delay(2000);

  // draw lines
  glcd.clear();
  for (i=0; i<16; i++) {
    glcd.drawline(0, 0, i*8, 64, BLACK);
    glcd.display();
    delay(10);
  }
  delay(2000);

  // draw circles
  glcd.clear();
  for (i=0; i<8; i++) {
    glcd.drawcircle(63, 31, i*4, BLACK);
    glcd.display();
    delay(10);
  }
  delay(2000);

  // draw mulitple circles
  testdrawcircle();
  glcd.display();
  delay(2000);
  glcd.clear();

  // draw a black circle, 10 pixel radius, at location (32,32)
  glcd.fillcircle(32, 32, 10, BLACK);
  glcd.display();
  delay(2000);
  glcd.clear();

  // draw the first ~120 characters in the font
  testdrawchar();
  glcd.display();
  delay(2000);
  glcd.clear();

  // draw a string at location (0,0)
  //glcd.drawstring(0, 0, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation");
  glcd.drawstring(0, 0, "0123456789 0123456789 ABCDEFJHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvxwyz");
  glcd.display();
  delay(2000);
  glcd.clear();

  // draw a bitmap icon and 'animate' movement
  // testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);

  // ------ setup WiFi
  setupWifi(MY_HOSTNAME,&myProgress);
  Serial.println(" ");
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
  // ------------------------------


}

int     cycle      = 0;
uint8_t currled    = 0;
int     lastmillis = 0;

void loop() {
  int     li;
  uint8_t col;

  ArduinoOTA.handle();
  if ((millis() - lastmillis) > 200) {
    lastmillis = millis();
    samd.updateStatus();
    ////if (not btnLeft) setLed(currled,0,0,0);
    if (not samd.isDown(BTN_LEFT)) samd.setLed(currled,0,0,0);
    Serial.println("");
    currled = cycle % 6;
    col     = cycle % 3;
    ////readSamd();
    ////if (btnDown)  col=0;   // Red color
    if (samd.isDown(BTN_DOWN))  col=0;
    ////if (btnExit)  col=1;   // Blue color
    if (samd.isDown(BTN_EXIT))  col=1;
    ////if (btnEnter) col=2;   // Green color
    if (samd.isDown(BTN_ENTER)) col=2;

    if (digitalRead(PIN_I2C_INT) == HIGH ) {
      Serial.print("Led: ");
      Serial.print(currled);
      Serial.print(" Color: ");
      Serial.println(col); 
      ////if ( col == 0) setLed(currled,1,0,0);
      ////if ( col == 1) setLed(currled,0,1,0);
      ////if ( col == 2) setLed(currled,0,0,1);
      if ( col == 0) samd.setLed(currled,1,0,0);
      if ( col == 1) samd.setLed(currled,0,1,0);
      if ( col == 2) samd.setLed(currled,0,0,1);
      ////if (btnRight) buzz();
      if (samd.isDown(BTN_RIGHT)) samd.buzz();
      ////if (btnUp)    setBl(0); else setBl(250);
      if (samd.isDown(BTN_UP)) samd.setBl(0); else samd.setBl(250);
    } else {
      Serial.println("PIN_I2C_INT is disabled");   
    }
    cycle++;
  }
}


void testdrawchar(void) {
  for (uint8_t i=0; i < 168; i++) {
    glcd.drawchar((i % 21) * 6, i/21, i);
  }    
}

void testdrawcircle(void) {
  for (uint8_t i=0; i<64; i+=2) {
    glcd.drawcircle(63, 31, i, BLACK);
  }
}


void testdrawrect(void) {
  for (uint8_t i=0; i<64; i+=2) {
    glcd.drawrect(i, i, 128-i, 64-i, BLACK);
  }
}

void testfillrect(void) {
  for (uint8_t i=0; i<64; i++) {
      // alternate colors for moire effect
    glcd.fillrect(i, i, 128-i, 64-i, i%2);
  }
}

void testdrawline() {
  for (uint8_t i=0; i<128; i+=4) {
    glcd.drawline(0, 0, i, 63, BLACK);
  }
  for (uint8_t i=0; i<64; i+=4) {
    glcd.drawline(0, 0, 127, i, BLACK);
  }

  glcd.display();
  delay(1000);

  for (uint8_t i=0; i<128; i+=4) {
    glcd.drawline(i, 63, 0, 0, WHITE);
  }
  for (uint8_t i=0; i<64; i+=4) {
    glcd.drawline(127, i, 0, 0, WHITE);
  }
}
