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
#include "main.h"

char init(void)
{
  char rc;
  unsigned char reg;
  unsigned char val[3];

  rc = read(BH1790GLC_PART_ID, &reg, sizeof(reg));
  if (rc != 0) {
    printf("Can't access BH1790GLC\n");
    return (rc);
  }
  printf("BH1790GLC Part ID Value = 0x\n");
  printf("Should be in HEX: %c\n", reg);

  if (reg != BH1790GLC_PID_VAL) {
    printf("Can't find BH1790GLC\n");
    return (rc);
  }

  rc = read(BH1790GLC_MANUFACTURER_ID, &reg, sizeof(reg));
  if (rc != 0) {
    printf("Can't access BH1790GLC\n");
    return (rc);
  }
  printf("BH1790GLC MANUFACTURER ID Register Value = 0x");
  printf("Should be in HEX: %c\n", reg);

  if (reg != BH1790GLC_MID_VAL) {
    printf("Can't find BH1790GLC\n");
    return (rc);
  }

  val[0] = BH1790GLC_MEAS_CONTROL1_VAL;
  val[1] = BH1790GLC_MEAS_CONTROL2_VAL;
  val[2] = BH1790GLC_MEAS_START_VAL;  
  rc = write(BH1790GLC_MEAS_CONTROL1, val, sizeof(val));
  if (rc != 0) {
    printf("Can't write BH1790GLC MEAS_CONTROL1-MEAS_START register\n");
  }
  
  return (rc);
}

char get_rawval(unsigned char *data)
{
  char rc;

  rc = read(BH1790GLC_DATAOUT_LEDOFF, data, 4);
  if (rc != 0) {
    printf("Can't get BH1790GLC value\n");
  }

  return (rc);
}

char get_val(unsigned short *data)
{
  char rc;
  unsigned char val[4];

  rc = get_rawval(val);
  if (rc != 0) {
    return (rc);
  }

  data[0] = ((unsigned short)val[1] << 8) | (val[0]);
  data[1] = ((unsigned short)val[3] << 8) | (val[2]);

  return (rc);  
}

char write(unsigned char memory_address, unsigned char *data, unsigned char size)
{
  char rc;

  HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&hi2c1, BH1790GLC_DEVICE_ADDRESS, memory_address, size, HAL_MAX_DELAY);
  if(ret != HAL_OK){
	  rc = 1;			//0 means success
  }else{
	  rc = 0;
  }

  return (rc);
}

char read(unsigned char memory_address, unsigned char *data, int size)
{
  char rc;
  unsigned char cnt;

  HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&hi2c1, BH1790GLC_DEVICE_ADDRESS, memory_address, size, HAL_MAX_DELAY);
  if(ret != HAL_OK){
	  rc = 1;			//0 means success
  }else{
	  rc = 0;
  }

  ret = HAL_I2C_Master_Receive(&hi2c1, BH1790GLC_DEVICE_ADDRESS, memory_address, size, HAL_MAX_DELAY);

  return (0);
}

