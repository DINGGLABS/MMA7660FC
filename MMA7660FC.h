#ifndef MMA7660FC_H
#define MMA7660FC_H

/* Includes --------------------------------------------------- */
#include <Arduino.h>
#include <Wire.h>

/* Class ------------------------------------------------------ */
class MMA7660FC
{
public:
	/* constructor(s) & deconstructor */
	MMA7660FC(TwoWire& AccelWire) : AccelWire_(AccelWire) {};
	~MMA7660FC() {};

	/* public constants (static) */

	/* public enumerations */
	enum mode {STAND_BY = 0, ACTIVE = 1};
  enum orientation {UNKNOWN = 0, UP, DOWN, HORIZONTALLY, VERTICALLY};
  enum sample_rate
  {
    AUTO_SLEEP_120 = 0X00,  // 120 sample per second
    AUTO_SLEEP_64  = 0X01,
    AUTO_SLEEP_32  = 0X02,
    AUTO_SLEEP_16  = 0X03,
    AUTO_SLEEP_8   = 0X04,
    AUTO_SLEEP_4   = 0X05,
    AUTO_SLEEP_2   = 0X06,
    AUTO_SLEEP_1   = 0X07
  };

	/* public methods */
  void begin();
  void end();
  void setMode(mode modeNr);
  void setSampleRate(sample_rate rateNr);
  void getAccelerationVector(int8_t vector[3]);
  void getAccelerationVector(float vector[3], bool convert = true);
  orientation getOrientation();

private:
	/* attributes */
	// ...

  /* private constants (static) */
	static const uint8_t I2C_ADDR        = 0x4C;

  static const uint8_t REG_X           = 0x00;
  static const uint8_t REG_Y           = 0x01;
  static const uint8_t REG_Z           = 0x02;
  static const uint8_t REG_TILT        = 0x03;
  static const uint8_t REG_SRST        = 0x04;
  static const uint8_t REG_SPCNT       = 0x05;
  static const uint8_t REG_INTSU       = 0x06;
  static const uint8_t REG_MODE        = 0x07;
  static const uint8_t REG_SR          = 0x08;  // sample rate register
  static const uint8_t REG_PDET        = 0x09;
  static const uint8_t REG_PD          = 0x0A;

  static const uint8_t G_DIVISOR       = 21;    // ~1g
  static const int8_t ACCEL_THRESHOLD  = 20;

	/* private variables */
  TwoWire& AccelWire_;
  
	/* private methods */
  uint8_t readRegister(uint8_t address);
  void writeRegister(uint8_t address, uint8_t value);
};

#endif
