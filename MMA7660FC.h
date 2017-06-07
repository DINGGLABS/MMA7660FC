#ifndef _MMA7660FC_H_
#define _MMA7660FC_H_
/*******************************************************************************
* \file    MMA7660FC.h
********************************************************************************
* \author  Jascha Haldemann jh@oxon.ch
* \date    01.11.2016
* \version 1.0
*
* \brief   The MMA7660FC is a simple low power accelerometer
*
********************************************************************************
* Accelerometer Library
*******************************************************************************/

/* ============================== Global imports ============================ */
#include <Arduino.h>
#include <Wire.h>

/* ==================== Global module constant declaration ================== */

/* ========================= Global macro declaration ======================= */

/* ============================ Class declaration =========================== */
class MMA7660FC
{
public:
  /* Public member typedefs */
  typedef enum : uint8_t
  {
    STAND_BY = 0,   // Istand_by = 2... 10uA
    ACTIVE = 1
  } mode_t;

  typedef enum : uint8_t
  {
    UNKNOWN = 0,
    UP = 1,
    DOWN = 2,
    HORIZONTALLY = 3,
    VERTICALLY = 4
  } orientation_t;

  typedef enum : uint8_t
  {
    AUTO_SLEEP_120 = 0X00,  // 120 samples per second
    AUTO_SLEEP_64  = 0X01,
    AUTO_SLEEP_32  = 0X02,
    AUTO_SLEEP_16  = 0X03,
    AUTO_SLEEP_8   = 0X04,
    AUTO_SLEEP_4   = 0X05,
    AUTO_SLEEP_2   = 0X06,
    AUTO_SLEEP_1   = 0X07
  } sampleRate_t;

  /* Constructor(s) and  Destructor */
	MMA7660FC(TwoWire& AccelWire) : AccelWire_(AccelWire) {};
	~MMA7660FC() {};

	/* Public member functions */
  void begin(const uint32_t clockSpeed = 200000L);
  void end();
  void setMode(mode_t modeNr);
  void setSampleRate(sampleRate_t rateNr);
  void getAccelerationVector(int8_t vector[3]);
  void getAccelerationVector(float vector[3], bool convert = true);
  orientation_t getOrientation();
  //TODO: implement a sleep()- or turnOff()-methode

private:
  /* Private constant declerations (static) */
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

	/* Private member data */
  TwoWire& AccelWire_;

	/* Private member functions */
  uint8_t readRegister(uint8_t address);
  void writeRegister(uint8_t address, uint8_t value);
};

#endif
