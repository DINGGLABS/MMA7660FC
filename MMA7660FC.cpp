/* Includes --------------------------------------------------- */
#include "MMA7660FC.h"

/* Public ----------------------------------------------------- */
void MMA7660FC::begin()
{
  setMode(STAND_BY);
  setSampleRate(AUTO_SLEEP_120);
  setMode(ACTIVE);
}

void MMA7660FC::end()
{
  setMode(STAND_BY);
}

void MMA7660FC::setMode(mode modeNr)
{
  writeRegister(REG_MODE, modeNr);
}

void MMA7660FC::setSampleRate(sample_rate rateNr)
{
  writeRegister(REG_SR, rateNr);
}

void MMA7660FC::getAccelerationVector(int8_t *vector)
{
  bool alert;
  do
  {
    alert = false;
    AccelWire_.beginTransmission(I2C_ADDR);
    AccelWire_.write(REG_X);
    AccelWire_.endTransmission();
    AccelWire_.requestFrom(I2C_ADDR, uint8_t(3));
    for (uint8_t i = 0; i < 3; i++)  // x, y, z
    {
      /* get register */
      vector[i] = AccelWire_.read();
      /* check the alert-bit */
      if (vector[i] & (1 << 6)) alert = true;
      else vector[i] = int8_t(vector[i] << 2) >> 2;   // calculate the signed 6-bit acceleration
    }
  } while (alert);  // repeat the whole crap if an alert bit is set!
}

void MMA7660FC::getAccelerationVector(float *vector, bool convert)
{
  getAccelerationVector((int8_t*)(vector));
  if (convert)
  {
    vector[0] /= G_DIVISOR;
    vector[1] /= G_DIVISOR;
    vector[2] /= G_DIVISOR;
  }

  // bool alert;
  // do
  // {
  //   alert = false;
  //   AccelWire_.beginTransmission(I2C_ADDR);
  //   AccelWire_.write(REG_X);
  //   AccelWire_.endTransmission();
  //   AccelWire_.requestFrom(I2C_ADDR, uint8_t(3));
  //   for (uint8_t i = 0; i < 3; i++)  // x, y, z
  //   {
  //     /* get register */
  //     vector[i] = AccelWire_.read();
  //     /* check the alert-bit */
  //     if (vector[i] & (1 << 6)) alert = true;
  //     else vector[i] = int8_t(vector[i] << 2) >> 2;   // calculate the signed 6-bit acceleration

  //     if (convert) vector[i] /= G_DIVISOR;
  //   }
  // } while (alert);  // repeat the whole crap if an alert bit is set!
}

MMA7660FC::orientation MMA7660FC::getOrientation()
{
  orientation o = UNKNOWN;
  int8_t vector[3] = {0};
  getAccelerationVector(vector);
  if (vector[2] <= -ACCEL_THRESHOLD) o = UP;
  else if (vector[2] >= ACCEL_THRESHOLD) o = DOWN;
  else if (vector[0] <= -ACCEL_THRESHOLD) o = HORIZONTALLY;
  else if (vector[0] >= ACCEL_THRESHOLD) o = HORIZONTALLY;
  else if (vector[1] <= -ACCEL_THRESHOLD) o = VERTICALLY;
  else if (vector[1] >= ACCEL_THRESHOLD) o = VERTICALLY;
  return o;
}

/* Private ---------------------------------------------------- */
uint8_t MMA7660FC::readRegister(uint8_t address)
{
  AccelWire_.beginTransmission(I2C_ADDR);
  AccelWire_.write(address);
  AccelWire_.endTransmission();

  AccelWire_.requestFrom(I2C_ADDR, uint8_t(1));
  return AccelWire_.read();
}

void MMA7660FC::writeRegister(uint8_t address, uint8_t value)
{
  AccelWire_.beginTransmission(I2C_ADDR);
  AccelWire_.write(address);
  AccelWire_.write(value);
  AccelWire_.endTransmission();
}
