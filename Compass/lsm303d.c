
#include "../Communication.h"
#include "LSM303D.h"
#include <math.h>



LSM303_t * thisDevice;

uint8_t LSM303D_Init(LSM303_t * device, deviceType dev, sa0State sa0)
{
	device->m_min = (vect_t){-32767, -32767, -32767};
	device->m_max = (vect_t){+32767, +32767, +32767};
		
  // perform auto-detection unless device type and SA0 state were both specified
  if (dev == device_auto || sa0 == sa0_auto)
  {
    // check for LSM303D if device is unidentified or was specified to be this type
    if (dev == device_auto || device->_device == device_D)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && LSM303D_TestReg(D_SA0_HIGH_ADDRESS, WHO_AM_I) == D_WHO_ID)
      {
        // device responds to address 0011101 with D ID; it's a D with SA0 high
        dev = device_D;
        sa0 = sa0_high;
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && LSM303D_TestReg(D_SA0_LOW_ADDRESS, WHO_AM_I) == D_WHO_ID)
      {
        // device responds to address 0011110 with D ID; it's a D with SA0 low
        dev = device_D;
        sa0 = sa0_low;
      }
    }
    
    // check for LSM303DLHC, DLM, DLH if device is still unidentified or was specified to be one of these types
    if (dev == device_auto || dev == device_DLHC || dev == device_DLM || dev == device_DLH)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && LSM303D_TestReg(DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS, CTRL_REG1_A) != TEST_REG_ERROR)
      {
        // device responds to address 0011001; it's a DLHC, DLM with SA0 high, or DLH with SA0 high
        sa0 = sa0_high;
        if (dev == device_auto)
        { 
          // use magnetometer WHO_AM_I register to determine device type
          //
          // DLHC seems to respond to WHO_AM_I request the same way as DLM, even though this
          // register isn't documented in its datasheet. Since the DLHC accelerometer address is the
          // same as the DLM with SA0 high, but Pololu DLM boards pull SA0 low by default, we'll
          // guess that a device whose accelerometer responds to the SA0 high address and whose
          // magnetometer gives the DLM ID is actually a DLHC.
          device->_device = (LSM303D_TestReg(DLHC_DLM_DLH_MAG_ADDRESS, WHO_AM_I_M) == DLM_WHO_ID) ? device_DLHC : device_DLH;
        }
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && LSM303D_TestReg(DLM_DLH_ACC_SA0_LOW_ADDRESS, CTRL_REG1_A) != TEST_REG_ERROR)
      {
        // device responds to address 0011000; it's a DLM with SA0 low or DLH with SA0 low
        sa0 = sa0_low;
        if (dev == device_auto)
        {
          // use magnetometer WHO_AM_I register to determine device type
          dev = (LSM303D_TestReg(DLHC_DLM_DLH_MAG_ADDRESS, WHO_AM_I_M) == DLM_WHO_ID) ? device_DLM : device_DLH;
        }
      }
    }
    
    // make sure device and SA0 were successfully detected; otherwise, indicate failure
    if (dev == device_auto || sa0 == sa0_auto)
    {
      return 0;
    }
  }
  
  device->_device = dev;
  
  // set device addresses and translated register addresses
  switch (dev)
  {
    case device_D:
      device->acc_address = device->mag_address = (sa0 == sa0_high) ? D_SA0_HIGH_ADDRESS : D_SA0_LOW_ADDRESS;
      translated_regs[-OUT_X_L_M] = D_OUT_X_L_M;
      translated_regs[-OUT_X_H_M] = D_OUT_X_H_M;
      translated_regs[-OUT_Y_L_M] = D_OUT_Y_L_M;
      translated_regs[-OUT_Y_H_M] = D_OUT_Y_H_M;
      translated_regs[-OUT_Z_L_M] = D_OUT_Z_L_M;
      translated_regs[-OUT_Z_H_M] = D_OUT_Z_H_M;
      break;

    case device_DLHC:
      device->acc_address = DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS; // DLHC doesn't have configurable SA0 but uses same acc address as DLM/DLH with SA0 high
      device->mag_address = DLHC_DLM_DLH_MAG_ADDRESS;
      translated_regs[-OUT_X_H_M] = DLHC_OUT_X_H_M;
      translated_regs[-OUT_X_L_M] = DLHC_OUT_X_L_M;
      translated_regs[-OUT_Y_H_M] = DLHC_OUT_Y_H_M;
      translated_regs[-OUT_Y_L_M] = DLHC_OUT_Y_L_M;
      translated_regs[-OUT_Z_H_M] = DLHC_OUT_Z_H_M;
      translated_regs[-OUT_Z_L_M] = DLHC_OUT_Z_L_M;
      break;

    case device_DLM:
      device->acc_address = (sa0 == sa0_high) ? DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS : DLM_DLH_ACC_SA0_LOW_ADDRESS;
      device->mag_address = DLHC_DLM_DLH_MAG_ADDRESS;
      translated_regs[-OUT_X_H_M] = DLM_OUT_X_H_M;
      translated_regs[-OUT_X_L_M] = DLM_OUT_X_L_M;
      translated_regs[-OUT_Y_H_M] = DLM_OUT_Y_H_M;
      translated_regs[-OUT_Y_L_M] = DLM_OUT_Y_L_M;
      translated_regs[-OUT_Z_H_M] = DLM_OUT_Z_H_M;
      translated_regs[-OUT_Z_L_M] = DLM_OUT_Z_L_M;
      break;

    case device_DLH:
      device->acc_address = (sa0 == sa0_high) ? DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS : DLM_DLH_ACC_SA0_LOW_ADDRESS;
      device->mag_address = DLHC_DLM_DLH_MAG_ADDRESS;
      translated_regs[-OUT_X_H_M] = DLH_OUT_X_H_M;
      translated_regs[-OUT_X_L_M] = DLH_OUT_X_L_M;
      translated_regs[-OUT_Y_H_M] = DLH_OUT_Y_H_M;
      translated_regs[-OUT_Y_L_M] = DLH_OUT_Y_L_M;
      translated_regs[-OUT_Z_H_M] = DLH_OUT_Z_H_M;
      translated_regs[-OUT_Z_L_M] = DLH_OUT_Z_L_M;
      break;
  }
  return 1;
}

/*
Enables the LSM303's accelerometer and magnetometer. Also:
- Sets sensor full scales (gain) to default power-on values, which are
  +/- 2 g for accelerometer and +/- 1.3 gauss for magnetometer
  (+/- 4 gauss on LSM303D).
- Selects 50 Hz ODR (output data rate) for accelerometer and 7.5 Hz
  ODR for magnetometer (6.25 Hz on LSM303D). (These are the ODR
  settings for which the electrical characteristics are specified in
  the datasheets.)
- Enables high resolution modes (if available).
Note that this function will also reset other settings controlled by
the registers it writes to.
*/
void LSM303D_EnableDefault(LSM303_t * device)
{

  if (device->_device == device_D)
  {
    // Accelerometer

    // 0x00 = 0b00000000
    // AFS = 0 (+/- 2 g full scale)
    LSM303D_WriteReg(device->acc_address, CTRL2, 0x00);

    // 0x57 = 0b01010111
    // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
    LSM303D_WriteReg(device->acc_address, CTRL1, 0x57);

    // Magnetometer

    // 0x64 = 0b01100100
    // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
    LSM303D_WriteReg(device->mag_address, CTRL5, 0x64);

    // 0x20 = 0b00100000
    // MFS = 01 (+/- 4 gauss full scale)
    LSM303D_WriteReg(device->mag_address, CTRL6, 0x20);

    // 0x00 = 0b00000000
    // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
    LSM303D_WriteReg(device->mag_address, CTRL7, 0x00);
  }
  else
  {
    // Accelerometer
    
    if (device->_device == device_DLHC)
    {
      // 0x08 = 0b00001000
      // FS = 00 (+/- 2 g full scale); HR = 1 (high resolution enable)
      LSM303D_WriteReg(device->acc_address, CTRL_REG4_A, 0x08);

      // 0x47 = 0b01000111
      // ODR = 0100 (50 Hz ODR); LPen = 0 (normal mode); Zen = Yen = Xen = 1 (all axes enabled)
      LSM303D_WriteReg(device->acc_address, CTRL_REG1_A, 0x47);
    }
    else // DLM, DLH
    {
      // 0x00 = 0b00000000
      // FS = 00 (+/- 2 g full scale)
      LSM303D_WriteReg(device->acc_address, CTRL_REG4_A, 0x00);

      // 0x27 = 0b00100111
      // PM = 001 (normal mode); DR = 00 (50 Hz ODR); Zen = Yen = Xen = 1 (all axes enabled)
      LSM303D_WriteReg(device->acc_address, CTRL_REG1_A, 0x27);
    }

    // Magnetometer

    // 0x0C = 0b00001100
    // DO = 011 (7.5 Hz ODR)
    LSM303D_WriteReg(device->mag_address, CRA_REG_M, 0x0C);

    // 0x20 = 0b00100000
    // GN = 001 (+/- 1.3 gauss full scale)
    LSM303D_WriteReg(device->mag_address, CRB_REG_M, 0x20);

    // 0x00 = 0b00000000
    // MD = 00 (continuous-conversion mode)
    LSM303D_WriteReg(device->mag_address, MR_REG_M, 0x00);
  }
}

// Writes a register
void LSM303D_WriteReg(uint8_t addr, uint8_t reg, uint8_t value)
{
  TWIstart();
  TWIbyteWrite(addr);
  TWIbyteWrite(reg);
  TWIbyteWrite(value);
  TWIstop();
}

// Reads an accelerometer register
uint8_t LSM303D_ReadReg(LSM303_t * device, uint8_t addr, int32_t reg)
{
	uint8_t value;

	// if dummy register address (magnetometer Y/Z), look up actual translated address (based on device type)
	if (reg < 0)
	{
		reg = translated_regs[-reg];
	}

  TWIstart();
  TWIslaveWrite(addr);
  TWIbyteWrite((uint8_t)reg);
  TWIstop();
  
  TWIread(addr, &value, 1);

  return value;
}

// Reads the 3 accelerometer channels and stores them in vector a
void LSM303D_ReadAcc(LSM303_t * device) { ////////////////////////////////////////////////////////////////////////////////////////////////////
	struct  
	{
		uint8_t xla;
		uint8_t xha;
		uint8_t yla;
		uint8_t yha;
		uint8_t zla;
		uint8_t zha;
	} result;

  TWIstart();
  TWIslaveWrite(device->acc_address);
  // assert the MSB of the address to get the accelerometer
  // to do slave-transmit subaddress updating.
  TWIbyteWrite(OUT_X_L_A | (1 << 7));
  
  TWIread(device->acc_address, &result.xla, 6); // TODO: check this: "(result.xla++) == result.xha "

  // combine high and low bytes
  // This no longer drops the lowest 4 bits of the readings from the DLH/DLM/DLHC, which are always 0
  // (12-bit resolution, left-aligned). The D has 16-bit resolution
  device->reading.a.X = (int16_t)(result.xha << 8 | result.xla);
  device->reading.a.Y = (int16_t)(result.yha << 8 | result.yla);
  device->reading.a.Z = (int16_t)(result.zha << 8 | result.zla);
}

// Reads the 3 magnetometer channels and stores them in vector m
void LSM303D_ReadMag(LSM303_t * device)
{
	uint8_t result[6];
	TWIstart();
	TWIslaveWrite(device->mag_address);
	// If LSM303D, assert MSB to enable subaddress updating
	// OUT_X_L_M comes first on D, OUT_X_H_M on others
	TWIbyteWrite((device->_device == device_D) ? translated_regs[-OUT_X_L_M] | (1 << 7) : translated_regs[-OUT_X_H_M]);
	TWIread(device->mag_address, result, 6);
	
  uint8_t xlm, xhm, ylm, yhm, zlm, zhm;

  if (device->_device == device_D)
  {
    // D: X_L, X_H, Y_L, Y_H, Z_L, Z_H
    xlm = result[0];
    xhm = result[1];
    ylm = result[2];
    yhm = result[3];
    zlm = result[4];
    zhm = result[5];
  }
  else
  {
    // DLHC, DLM, DLH: X_H, X_L...
    xhm = result[0];
    xlm = result[1];

    if (device->_device == device_DLH)
    {
      // DLH: ...Y_H, Y_L, Z_H, Z_L
      yhm = result[2];
      ylm = result[3];
      zhm = result[4];
      zlm = result[5];
    }
    else
    {
      // DLM, DLHC: ...Z_H, Z_L, Y_H, Y_L
      zhm = result[2];
      zlm = result[3];
      yhm = result[4];
      ylm = result[5];
    }
  }

  // combine high and low bytes
  device->reading.m.X = (int16_t)(xhm << 8 | xlm);
  device->reading.m.Y = (int16_t)(yhm << 8 | ylm);
  device->reading.m.Z = (int16_t)(zhm << 8 | zlm);
}

// Reads all 6 channels of the LSM303 and stores them in the object variables
void LSM303D_Read(LSM303_t * device)
{
  LSM303D_ReadAcc(device);
  LSM303D_ReadMag(device);
}

/*
Returns the angular difference in the horizontal plane between a
default vector and north, in degrees.

The default vector here is chosen to point along the surface of the
PCB, in the direction of the top of the text on the silkscreen.
This is the +X axis on the Pololu LSM303D carrier and the -Y axis on
the Pololu LSM303DLHC, LSM303DLM, and LSM303DLH carriers.
*/
void LSM303D_Heading(LSM303_t * device)
{
  if (device->_device == device_D)
  {
    device->heading= /*(vect_int_t)*/(LSM303D_Heading_t){1, 0, 0};
  }
  else
  {
    device->heading= /*(vect_int_t)*/(LSM303D_Heading_t){0, -1, 0};
  }
}

void LSM303D_VectorNormalize(vect_float_t *a)
{
  float mag = sqrt(vector_dot(a, a)); // TODO: what's this?
  a->X /= mag;
  a->Y /= mag;
  a->Z /= mag;
}

int8_t LSM303D_TestReg(uint8_t address, regAddr reg)
{
	TWIstart();
	TWIslaveWrite(address);
	if (!TWIbyteWrite((uint8_t)reg))
	{
		return TEST_REG_ERROR;
	}
	TWIslaveRead(address);
	if (!TWIbyteRead((uint8_t)reg))
	{
		return TEST_REG_ERROR;
	}
	
}

deviceType LSM303D_GetDeviceType( LSM303_t * device)
{
	return device->_device;
}
