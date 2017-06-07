/*******************************************************************************
* \file    MMA7660FC.cpp
********************************************************************************
* \author  Jascha Haldemann jh@oxon.ch
* \date    01.11.2016
* \version 1.0
*******************************************************************************/

/* ================================= Imports ================================ */
#include "MMA7660FC.h"

/* ======================= Module constant declaration ====================== */

/* ======================== Module macro declaration ======================== */

/* ====================== Module class instantiations ======================= */

/* ======================== Public member Functions ========================= */
/** -------------------------------------------------------------------------
  * \fn     begin
  * \brief  inits the accelerometer
  --------------------------------------------------------------------------- */
  void MMA7660FC::begin(const uint32_t clockSpeed)
  {
    AccelWire_.begin();
    AccelWire_.setClock(clockSpeed);

    setMode(STAND_BY);
    setSampleRate(AUTO_SLEEP_120);
    setMode(ACTIVE);
  }

/** -------------------------------------------------------------------------
  * \fn     end
  * \brief  deinits the accelerometer
  --------------------------------------------------------------------------- */
  void MMA7660FC::end()
  {
    setMode(STAND_BY);
    AccelWire_.end();
  }

/** -------------------------------------------------------------------------
  * \fn     setMode
  * \brief  sets the mode of the accelerometer
  *
  * \param  modeNr  mode number (see enumerator in the header file)
  * \return None
  --------------------------------------------------------------------------- */
  void MMA7660FC::setMode(mode_t modeNr)
  {
    writeRegister(REG_MODE, modeNr);
  }

/** -------------------------------------------------------------------------
  * \fn     setSampleRate
  * \brief  sets the sample rate of the accelerometer
  *
  * \param  rateNr  rate number (see enumerator in the header file)
  * \return None
  --------------------------------------------------------------------------- */
  void MMA7660FC::setSampleRate(sampleRate_t rateNr)
  {
    writeRegister(REG_SR, rateNr);
  }

/** -------------------------------------------------------------------------
  * \fn     getAccelerationVector
  * \brief  gets the acceleration vector (x, y, z)
  *
  * \param  vector  acceleration array pointer (int8)
  * \return None
  --------------------------------------------------------------------------- */
  void MMA7660FC::getAccelerationVector(int8_t vector[3])
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

/** -------------------------------------------------------------------------
  * \fn     getAccelerationVector
  * \brief  gets the acceleration vector (x, y, z)
  *
  * \param  vector  acceleration array pointer (float)
  * \param  convert converts to a normed value if true
  * \return None
  --------------------------------------------------------------------------- */
  void MMA7660FC::getAccelerationVector(float vector[3], bool convert)
  {
    int8_t tempVector[3];
    getAccelerationVector(tempVector);
    vector[0] = tempVector[0];
    vector[1] = tempVector[1];
    vector[2] = tempVector[2];

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

/** -------------------------------------------------------------------------
  * \fn     getOrientation
  * \brief  gets the orientation
  *
  * \return orientation (see enumerator in the header file)
  --------------------------------------------------------------------------- */
  MMA7660FC::orientation_t MMA7660FC::getOrientation()
  {
    orientation_t o = UNKNOWN;
    int8_t vector[3] = {0};
    getAccelerationVector(vector);
    if      (vector[2] <= -ACCEL_THRESHOLD) o = DOWN;
    else if (vector[2] >=  ACCEL_THRESHOLD) o = UP;
    else if (vector[0] <= -ACCEL_THRESHOLD) o = HORIZONTALLY;
    else if (vector[0] >=  ACCEL_THRESHOLD) o = HORIZONTALLY;
    else if (vector[1] <= -ACCEL_THRESHOLD) o = VERTICALLY;
    else if (vector[1] >=  ACCEL_THRESHOLD) o = VERTICALLY;
    return o;
  }

/* ======================= Private member Functions ========================= */
/** -------------------------------------------------------------------------
  * \fn     readRegister
  * \brief  reads the register of the given address
  *
  * \param  address  address of the accelerometer
  * \return content of the given address
  --------------------------------------------------------------------------- */
  uint8_t MMA7660FC::readRegister(uint8_t address)
  {
    AccelWire_.beginTransmission(I2C_ADDR);
    AccelWire_.write(address);
    AccelWire_.endTransmission();

    AccelWire_.requestFrom(I2C_ADDR, uint8_t(1));
    return AccelWire_.read();
  }

/** -------------------------------------------------------------------------
  * \fn     writeRegister
  * \brief  writes the register of the given address
  *
  * \param  address  address of the accelerometer
  * \param  value    value to write
  * \return None
  --------------------------------------------------------------------------- */
  void MMA7660FC::writeRegister(uint8_t address, uint8_t value)
  {
    AccelWire_.beginTransmission(I2C_ADDR);
    AccelWire_.write(address);
    AccelWire_.write(value);
    AccelWire_.endTransmission();
  }
