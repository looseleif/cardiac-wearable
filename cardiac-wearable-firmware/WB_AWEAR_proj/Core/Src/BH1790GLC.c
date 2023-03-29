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

	uint8_t regData;

	status = read(dev, BH1790GLC_MANUFACTURER_ID, &regData);	//get manufacturer id
	errNum += (status != HAL_OK);
	if(regData != BH1790GLC_MID_VAL){ return ERR_MID_VAL; }

	status = read(dev, BH1790GLC_PART_ID, &regData);	//get part id
	errNum += (status != HAL_OK);
	if(regData != BH1790GLC_PID_VAL){ return ERR_PID_VAL; }

	uint8_t configData[3];
	configData[0] = BH1790GLC_MEAS_CONTROL1_VAL;	//to BH1790GLC_MEAS_CONTROL1 (0x41)
	configData[1] = BH1790GLC_MEAS_CONTROL2_VAL;	//to BH1790GLC_MEAS_CONTROL2 (0x42)
	configData[2] = BH1790GLC_MEAS_START_VAL;		//to BH1790GLC_MEAS_START (0x43)

	status = write(dev, BH1790GLC_MEAS_CONTROL1, &configData[0]);
	errNum += (status != HAL_OK);

	status = write(dev, BH1790GLC_MEAS_CONTROL2, &configData[1]);
	errNum += (status != HAL_OK);

	status = write(dev, BH1790GLC_MEAS_START, &configData[2]);
	errNum += (status != HAL_OK);

	uint8_t writeCheck[3];
	status = many_reads(dev, BH1790GLC_MEAS_CONTROL1, writeCheck, 3);	//check config registers
	if(writeCheck[0] != configData[0]){
		return ERR_MEAS_CONTROL1;
	}
	if(writeCheck[1] != configData[1]){
		return ERR_MEAS_CONTROL2;
	}
	if(writeCheck[2] != configData[2]){
		return ERR_MEAS_START;
	}

	return 0;
}


HAL_StatusTypeDef get_rawval( BH1790GLC *dev, uint8_t *val )
{
//  char rc;
//
//  rc = read(BH1790GLC_DATAOUT_LEDOFF, data, 4);
//  if (rc != 0) {
//    printf("Can't get BH1790GLC value\n");
//  }
//
//  return (rc);

	HAL_StatusTypeDef status;

	status = many_reads(dev, BH1790GLC_DATAOUT_LEDOFF, val, 4);
	if(status != HAL_OK){
		//error
		return 1;
	}

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

	HAL_StatusTypeDef status;
	uint8_t val[4];

	status = get_rawval(dev, val);


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
	ret = HAL_I2C_Mem_Write(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1), reg, 1, data, 1, HAL_MAX_DELAY);
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

	ret = HAL_I2C_Mem_Read(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1), reg, 1, data, 1, HAL_MAX_DELAY);
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

	ret = HAL_I2C_Mem_Read(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1), reg, 1, data, length, HAL_MAX_DELAY);
	if(ret != HAL_OK){ return ret; }	//error check

	return HAL_OK;
}
