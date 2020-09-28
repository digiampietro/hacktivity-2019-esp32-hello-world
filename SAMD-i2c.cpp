#include <Arduino.h>
#include <Wire.h>
#include "SAMD-i2c.h"


// True if touch button is currently pressed
bool SAMD::isDown(uint8_t btn) {
  return (bitRead(c1,btn) > 0);
}

// True if touch button is currently not pressed 
bool SAMD::isUp(uint8_t btn) {
  return (bitRead(c1,btn) == 0);
}

// True if touch button was not pressed and now is pressed
bool SAMD::isPressed(uint8_t btn) {
  return (bitRead(c1,btn) and (bitRead(prevc1,btn) == 0));
}

// True if touch button was pressed and now is not pressed
bool SAMD::isReleased(uint8_t btn) {
  return ((bitRead(c1,btn) == 0) and bitRead(prevc1,btn));
}

void SAMD::updateStatus(void) {
  Wire.requestFrom(addr,2);
  prevc1=c1;
  prevc2=c2;
  c1=Wire.read();
  c2=Wire.read();
}

bool SAMD::buzz(void) {
  uint8_t c = 0x03;
  uint8_t z = 0x00;
  uint8_t fh = 4;
  uint8_t fl = 0;
  uint8_t dh = 1;
  uint8_t dl = 0;
  uint8_t error;
  Wire.beginTransmission(addr);
  Wire.write(c);
  Wire.write(fh);
  Wire.write(fl);
  Wire.write(dh);
  Wire.write(dl);
  error = Wire.endTransmission();
  return (error == 0);
}

bool SAMD::ledsOff(void) {
  uint8_t c = 0x04;
  uint8_t z = 0x00;
  uint8_t error;
  Wire.beginTransmission(addr);
  Wire.write(c);
  error = Wire.endTransmission();
  return (error == 0);
}

bool SAMD::setBl(uint8_t blstatus) {
  uint8_t c = 0x02;
  uint8_t error;
  uint8_t z = 0x00;
  Wire.beginTransmission(addr);
  Wire.write(c);
  Wire.write(blstatus);
  error = Wire.endTransmission();
  return (error == 0);
}

bool SAMD::setLed(uint8_t id, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t c = 0x01;
  uint8_t z = 0x00;
  uint8_t error;
  Wire.beginTransmission(addr);
  Wire.write(c);
  Wire.write(id);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  error = Wire.endTransmission();
  return (error == 0);
}

