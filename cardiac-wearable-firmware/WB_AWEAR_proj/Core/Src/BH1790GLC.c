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
	dev->ppg_data[0] = 0;
	dev->ppg_data[1] = 0;

	/* Keep count of errors */
	HAL_StatusTypeDef status;

	/* Take initial reads to make sure I2C is working */
	uint8_t regData;
	status = read(dev, BH1790GLC_MANUFACTURER_ID, &regData);	//get manufacturer id
	if(status != HAL_OK){
		return ERR_MID_VAL;
	}
	if(regData != BH1790GLC_MID_VAL){
		return ERR_MID_VAL;
	}

	status = read(dev, BH1790GLC_PART_ID, &regData);	//get part id
	if(status != HAL_OK){
		return ERR_PID_VAL;
	}
	if(regData != BH1790GLC_PID_VAL){
		return ERR_PID_VAL;
	}

	/* Configure the 3 registers needed to start taking measurements */
	uint8_t configData[3];
	configData[0] = BH1790GLC_MEAS_CONTROL1_VAL;	//to BH1790GLC_MEAS_CONTROL1 (0x41)
	configData[1] = BH1790GLC_MEAS_CONTROL2_VAL;	//to BH1790GLC_MEAS_CONTROL2 (0x42)
	configData[2] = BH1790GLC_MEAS_START_VAL;		//to BH1790GLC_MEAS_START (0x43)

	status = write(dev, BH1790GLC_MEAS_CONTROL1, &configData[0]);
	if(status != HAL_OK){
		return ERR_MEAS_CONTROL1;
	}
	status = write(dev, BH1790GLC_MEAS_CONTROL2, &configData[1]);
	if(status != HAL_OK){
		return ERR_MEAS_CONTROL2;
	}
	status = write(dev, BH1790GLC_MEAS_START, &configData[2]);
	if(status != HAL_OK){
		return ERR_MEAS_START;
	}

	return SUCCESS;
}

/*
 * Puts the ppg readings into the ppg_data struct variable
 */
uint8_t get_val( BH1790GLC *dev )
{
	HAL_StatusTypeDef status;
	uint8_t sensorData[4];

	status = many_reads(dev, BH1790GLC_DATAOUT_LEDOFF, sensorData, 4);
	if(status != HAL_OK){
		return ERR_DATA_OUT;		//error check
	}

	//convert the sensorData values to useful data
	dev->ppg_data[0] = ((uint16_t)sensorData[1]<<8)|(sensorData[0]);	//LED OFF
	dev->ppg_data[1] = ((uint16_t)sensorData[3]<<8)|(sensorData[2]);	//LED ON

	return SUCCESS;
}


/*
 * Writes “SWRESET=1” (Address 40h), when stop measurement or change parameter.
 * This will wipe all registers
 */
uint8_t reset_device( BH1790GLC *dev ){
	HAL_StatusTypeDef status;
	uint8_t configData = BH1790GLC_SWRESET;

	status = write(dev, BH1790GLC_RESET, &configData);
	if(status != HAL_OK){
		return ERR_RESET;
	}

	return SUCCESS;
}


/*
 * Run this periodically (according to data sheet) after reading data
 */
uint8_t param_refreshment( BH1790GLC *dev ){
	/* Keep count of errors */
	HAL_StatusTypeDef status;

	/* Configure the 3 registers needed to start taking measurements */
	uint8_t configData[3];
	configData[0] = BH1790GLC_MEAS_CONTROL1_VAL;	//to BH1790GLC_MEAS_CONTROL1 (0x41)
	configData[1] = BH1790GLC_MEAS_CONTROL2_VAL;	//to BH1790GLC_MEAS_CONTROL2 (0x42)
	configData[2] = BH1790GLC_MEAS_START_VAL;		//to BH1790GLC_MEAS_START (0x43)

	status = write(dev, BH1790GLC_MEAS_CONTROL1, &configData[0]);
	if(status != HAL_OK){
		return ERR_MEAS_CONTROL1;
	}

	status = write(dev, BH1790GLC_MEAS_CONTROL2, &configData[1]);
	if(status != HAL_OK){
		return ERR_MEAS_CONTROL2;
	}

	status = write(dev, BH1790GLC_MEAS_START, &configData[2]);
	if(status != HAL_OK){
		return ERR_MEAS_START;
	}

	return SUCCESS;

}


/*
 * Writes a single byte to a specific register address
 * dev : device
 * reg : register to write to
 * data : what to write
 */
HAL_StatusTypeDef write( BH1790GLC *dev, uint8_t reg, uint8_t *data)
{
	HAL_StatusTypeDef ret;

	//passing my handle, read from peripheral, get from this register, size of that register,
	// where to put the data, 1 byte of data, max delay
	ret = HAL_I2C_Mem_Write(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1), reg, 1, data, 1, HAL_MAX_DELAY);
	if(ret != HAL_OK){
		return ret;			//error check
	}

	return HAL_OK;
}


/*
 * Reads a single byte from a specific register address
 * dev : device
 * reg : register to read from
 * data : stores what was read
 */
HAL_StatusTypeDef read( BH1790GLC *dev, uint8_t reg, uint8_t *data)
{
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Mem_Read(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1), reg, 1, data, 1, HAL_MAX_DELAY);
	if(ret != HAL_OK){
		return ret;			//error check
	}


	return HAL_OK;
}

/*
 * Reads n bytes from a starting register address
 * dev : device
 * reg : register to read from
 * data : stores what was read
 * length : number of registers to read (in bytes)
 */
HAL_StatusTypeDef many_reads( BH1790GLC *dev, uint8_t reg, uint8_t *data, uint8_t length )
{
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Mem_Read(dev->i2cHandle, (BH1790GLC_DEVICE_ADDRESS<<1), reg, 1, data, length, HAL_MAX_DELAY);
	if(ret != HAL_OK){
		return ret;			//error check
	}

	return HAL_OK;
}
