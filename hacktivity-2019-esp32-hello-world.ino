#include <Wire.h>
#include "ST7565-i2c.h"
#include "SAMD-i2c.h"

// Needed for OTA setup
// #include "ota.h"

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

// print dots during the Wifi Connection phase

void myProgress () {
  //  Serial.print(".");
}


// Scan the i2c bus, normally not used
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

//#define NUMFLAKES 10
//#define XPOS 0
//#define YPOS 1
//#define DELTAY 2

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

void waitEnter(void) {
  glcd.drawstring( 0, 7, "Press ENTER to cont.", FONT_SMALL); 
  glcd.display();
  while (not samd.isReleased(BTN_ENTER)) {
    samd.updateStatus();
  }
  samd.buzz();
}

void setup() {
  int i, j;
  char s[100];

  pinMode(MYLED, OUTPUT);
  Serial.begin(115200);
  delay(100);
  Serial.println("ESP32 Starting");

  pinMode(PIN_I2C_INT, INPUT);
  Wire.begin(PIN_SDA, PIN_SCL);

  samd.ledsOff();
  samd.setBl(250);

  // initialize and set the contrast to 0x18
  glcd.i2cbegin(0x18);
  delay(1000);
  glcd.display(); // show splashscreen
  delay(3000);

  glcd.clear();
  glcd.drawstring(30, 1, "Hello",  FONT_BIG);
  glcd.drawstring(27, 3, "World!", FONT_BIG);
  glcd.drawstring(43, 5, "Hello",  FONT_SMALL);
  glcd.drawstring(41, 6, "World!", FONT_SMALL);
  glcd.drawstring( 0, 7, "Press ENTER to cont.",FONT_SMALL); 
  glcd.display();
  waitEnter();

  // draw rectangles
  glcd.clear();
  for (i=0; i<8; i++) {
    glcd.drawrect(i*8,i*4,128-(i*16),64-(i*8),BLACK);
    glcd.display();
  }
  delay(2000);

  // draw lines
  glcd.clear();
  for (i=0; i<16; i++) {
    glcd.drawline(0, 0, i*8, 64, BLACK);
    glcd.display();
  }
  delay(2000);

  // draw circles
  glcd.clear();
  for (i=0; i<8; i++) {
    glcd.drawcircle(63, 31, i*4, BLACK);
    glcd.display();
  }
  delay(2000);
  
  samd.updateStatus();
  glcd.clear();
  // draw a lenghty string
  glcd.drawstring(0, 0, "The quick",   FONT_BIG);
  glcd.drawstring(0, 2, "brown fox",   FONT_BIG);
  glcd.drawstring(0, 4, "jumps over",  FONT_SMALL);
  glcd.drawstring(0, 5, "the lazy dog",FONT_SMALL);
  glcd.display();
  waitEnter();
  
  glcd.clear();
  glcd.display();

  // ------ setup WiFi
  // setupWifi(MY_HOSTNAME,&myProgress);
  // Serial.println(" ");
  // Serial.print("Connected! IP: ");
  // Serial.println(WiFi.localIP());
  // ------------------------------


}

int     cycle      = 0;
uint8_t currled    = 0;
int     lastmillis = 0;

void loop() {
  int     li;
  uint8_t col;
  char    msg[15];

  // ArduinoOTA.handle();
  if ((millis() - lastmillis) > 200) {
    lastmillis = millis();

    // get info on touch button pressed
    samd.updateStatus();

    if (not samd.isDown(BTN_LEFT)) samd.setLed(currled,0,0,0);

    Serial.println(" ");
    currled = cycle % 6;
    col     = cycle % 3;

    if (currled == 0) {
      sprintf(msg,"STEP: %5d", cycle / 6);
      glcd.drawstring(0,0,msg,FONT_BIG);
      glcd.display();
    }
    
    if (samd.isDown(BTN_DOWN))  col=0;
    if (samd.isDown(BTN_EXIT))  col=1;
    if (samd.isDown(BTN_ENTER)) col=2;

    if (digitalRead(PIN_I2C_INT) == HIGH ) {
      Serial.print("Led: ");
      Serial.print(currled);
      Serial.print(" Color: ");
      Serial.println(col); 
      if ( col == 0) samd.setLed(currled,1,0,0);
      if ( col == 1) samd.setLed(currled,0,1,0);
      if ( col == 2) samd.setLed(currled,0,0,1);
      if (samd.isDown(BTN_RIGHT)) samd.buzz();
      if (samd.isDown(BTN_UP)) samd.setBl(0); else samd.setBl(250);
    } else {
      Serial.println("PIN_I2C_INT is disabled");   
    }
    cycle++;
  }
}


