/*****************************************************************************
  BH1790GLC.cpp

 Copyright (c) 2016 ROHM Co.,Ltd.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
******************************************************************************/

#include "BH1790GLC.h"

uint8_t BH1790GLC_init( BH1790GLC *dev, I2C_HandleTypeDef *i2cHandle )
{
	/* Set struct parameters */
	dev->i2cHandle = i2cHandle;
	dev->measurements[0] = 0.0f;
	dev->measurements[1] = 0.0f;
	dev->measurements[2] = 0.0f;
	dev->measurement = 0.0f;

	/* Keep count of errors */
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;


	/* First steps for communicating with sensor
	 * get manufacturer id 		0x0F
	 * get part id 				0x10
	 */
	uint8_t regData;

	status = read(dev, BH1790GLC_MANUFACTURER_ID, &regData);	//get manufacturer id
	errNum += (status != HAL_OK);
	if(regData != BH1790GLC_MANUFACTURER_ID){
		//error
		return 1;
	}

	status = read(dev, BH1790GLC_PART_ID, &regData);	//get part id
	errNum += (status != HAL_OK);
	if(regData != BH1790GLC_MANUFACTURER_ID){
		//error
		return 1;
	}

	/* START CONFIGURING THE SENSOR NOW! */


//  char rc;
//  unsigned char reg;
//  unsigned char val[3];
//
//  rc = read(BH1790GLC_PART_ID, &reg, sizeof(reg));
//  if (rc != 0) {
//    printf("Can't access BH1790GLC\n");
//    return (rc);
//  }
//  printf("BH1790GLC Part ID Value = 0x\n");
//  printf("Should be in HEX: %c\n", reg);
//
//  if (reg != BH1790GLC_PID_VAL) {
//    printf("Can't find BH1790GLC\n");
//    return (rc);
//  }
//
//  rc = read(BH1790GLC_MANUFACTURER_ID, &reg, sizeof(reg));
//  if (rc != 0) {
//    printf("Can't access BH1790GLC\n");
//    return (rc);
//  }
//  printf("BH1790GLC MANUFACTURER ID Register Value = 0x");
//  printf("Should be in HEX: %c\n", reg);
//
//  if (reg != BH1790GLC_MID_VAL) {
//    printf("Can't find BH1790GLC\n");
//    return (rc);
//  }
//
//  val[0] = BH1790GLC_MEAS_CONTROL1_VAL;
//  val[1] = BH1790GLC_MEAS_CONTROL2_VAL;
//  val[2] = BH1790GLC_MEAS_START_VAL;
//  rc = write(BH1790GLC_MEAS_CONTROL1, val, sizeof(val));
//  if (rc != 0) {
//    printf("Can't write BH1790GLC MEAS_CONTROL1-MEAS_START register\n");
//  }
//
//  return (rc);

	return 0;
}


uint8_t get_rawval( BH1790GLC *dev )
{
//  char rc;
//
//  rc = read(BH1790GLC_DATAOUT_LEDOFF, data, 4);
//  if (rc != 0) {
//    printf("Can't get BH1790GLC value\n");
//  }
//
//  return (rc);

	return 0;
}


uint8_t get_val( BH1790GLC *dev )
{
//  char rc;
//  unsigned char val[4];
//
//  rc = get_rawval(val);
//  if (rc != 0) {
//    return (rc);
//  }
//
//  data[0] = ((unsigned short)val[1] << 8) | (val[0]);
//  data[1] = ((unsigned short)val[3] << 8) | (val[2]);
//
//  return (rc);

	return 0;
}


/*
 * reg : which register in the sensor to write to
 * data : data to write (consider not having this be a pointer?
 */
HAL_StatusTypeDef write( BH1790GLC *dev, uint8_t reg, uint8_t *data)
{
	HAL_StatusTypeDef ret;

	//passing my handle, read from peripheral, get from this register, size of that register,
	// where to put the data, 1 byte of data, max delay
	ret = HAL_I2C_Mem_Write(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1), reg, 8, data, 1, HAL_MAX_DELAY);
	if(ret != HAL_OK){ return ret; }	//error check

	return HAL_OK;
}


/*
 * reg : which register in the sensor to read from
 * data : data to write (consider not having this be a pointer?
 */
HAL_StatusTypeDef read( BH1790GLC *dev, uint8_t reg, uint8_t *data)
{
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Mem_Read(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1) || 0x01, reg, 8, data, 1, HAL_MAX_DELAY);
	if(ret != HAL_OK){ return ret; }	//error check


	return HAL_OK;
}

/*
 * reg : which register in the sensor to read from
 * data : data to write (consider not having this be a pointer?
 * length : # of bytes to read
 */
HAL_StatusTypeDef many_reads( BH1790GLC *dev, uint8_t reg, uint8_t *data, uint8_t length )
{
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Mem_Read(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1) || 0x01, reg, 8, data, length, HAL_MAX_DELAY);
	if(ret != HAL_OK){ return ret; }	//error check

	return HAL_OK;
}
