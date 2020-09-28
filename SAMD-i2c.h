#ifndef SAMD_I2C
#define SAMD_I2C

//Bit position in the c1 status byte of various buttons
#define BTN_ENTER  0
#define BTN_LEFT   1
#define BTN_RIGHT  2
#define BTN_EXIT   3
#define BTN_DOWN   4
#define BTN_UP     5

class SAMD {
 private:
  uint8_t   c1, c2, prevc1, prevc2;
  int       addr;
 public:
  SAMD(uint8_t ADDR) :addr(ADDR), c1(0), c2(0), prevc1(0), prevc2(0) {}

  bool isDown(uint8_t btn);
  bool isUp(uint8_t btn);
  bool isPressed(uint8_t btn);
  bool isReleased(uint8_t btn);
  void updateStatus(void);

  bool buzz(void);
  bool ledsOff(void);
  bool setBl(uint8_t blstatus);
  bool setLed(uint8_t id, uint8_t r, uint8_t g, uint8_t b);
  
};

#endif
