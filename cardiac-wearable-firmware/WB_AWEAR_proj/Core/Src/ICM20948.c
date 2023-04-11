/*
 * ICM20948.c
 *
 *  Created on: Oct 26, 2018
 *      Author: cory & vaibhav
 */

// *** Three asterisks to the side of a line means this may change based on platform
#include "main.h" // ***
#include "ICM20948.h"
#include <string.h>

/*
 *
 * SPI abstraction
 *
 */
void ICM_readBytes(ICM20948 *dev, uint8_t reg, uint8_t *pData, uint16_t Size) // ***
{
	reg = reg | 0x80;
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(dev->spiHandle, &reg, 1);
	HAL_SPI_Receive_DMA(dev->spiHandle, pData, Size);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);
}

void ICM_WriteBytes(ICM20948 *dev, uint8_t reg, uint8_t *pData, uint16_t Size) // ***
{
	reg = reg & 0x7F;
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(dev->spiHandle, &reg, 1);
	HAL_SPI_Transmit_DMA(dev->spiHandle, pData, Size);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);

}

void ICM_ReadOneByte(ICM20948 *dev, uint8_t reg, uint8_t* pData) // ***
{
	reg = reg | 0x80;
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(dev->spiHandle, &reg, 1);
	while (HAL_SPI_GetState(dev->spiHandle) != HAL_SPI_STATE_READY)
		;
	HAL_SPI_Receive_DMA(dev->spiHandle, pData, 1);
	while (HAL_SPI_GetState(dev->spiHandle) != HAL_SPI_STATE_READY)
		;
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);
}

void ICM_WriteOneByte(ICM20948 *dev, uint8_t reg, uint8_t Data) // ***
{
	HAL_StatusTypeDef status;

	reg = reg & 0x7F;
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);	//set CS pin low
	status = HAL_SPI_Transmit_DMA(dev->spiHandle, &reg, 1);
	status = HAL_SPI_Transmit_DMA(dev->spiHandle, &Data, 1);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);		//set CS pin high
}

/*
 *
 * AUX I2C abstraction for magnetometer
 *
 */
void i2c_Mag_write(ICM20948 *dev, uint8_t reg,uint8_t value)
  {
  	ICM_WriteOneByte(dev, 0x7F, 0x30);

  	HAL_Delay(1);
  	ICM_WriteOneByte(dev, 0x03 ,0x0C);//mode: write

  	HAL_Delay(1);
  	ICM_WriteOneByte(dev, 0x04 ,reg);//set reg addr

  	HAL_Delay(1);
  	ICM_WriteOneByte(dev, 0x06 ,value);//send value

  	HAL_Delay(1);
  }

  static uint8_t ICM_Mag_Read(ICM20948 *dev, uint8_t reg)
  {
  	uint8_t  Data;
  	ICM_WriteOneByte(dev, 0x7F, 0x30);
    HAL_Delay(1);
  	ICM_WriteOneByte(dev, 0x03 ,0x0C|0x80);
    HAL_Delay(1);
  	ICM_WriteOneByte(dev, 0x04 ,reg);// set reg addr
    HAL_Delay(1);
  	ICM_WriteOneByte(dev, 0x06 ,0xff);//read
  	HAL_Delay(1);
  	ICM_WriteOneByte(dev, 0x7F, 0x00);
  	ICM_ReadOneByte(dev, 0x3B,&Data);
    HAL_Delay(1);
  	return Data;
  }

  void ICM20948_READ_MAG(ICM20948 *dev, int16_t magn[3])
  {
    uint8_t mag_buffer[10];

      mag_buffer[0] =ICM_Mag_Read(dev, 0x01);

      mag_buffer[1] =ICM_Mag_Read(dev, 0x11);
  	  mag_buffer[2] =ICM_Mag_Read(dev, 0x12);
  	  magn[0]=mag_buffer[1]|mag_buffer[2]<<8;
    	mag_buffer[3] =ICM_Mag_Read(dev, 0x13);
      mag_buffer[4] =ICM_Mag_Read(dev, 0x14);
    	magn[1]=mag_buffer[3]|mag_buffer[4]<<8;
  	 	mag_buffer[5] =ICM_Mag_Read(dev, 0x15);
      mag_buffer[6] =ICM_Mag_Read(dev, 0x16);
  		magn[2]=mag_buffer[5]|mag_buffer[6]<<8;

     	i2c_Mag_write(dev, 0x31,0x01);
  }

/*
 *
 * Read magnetometer
 *
 */
void ICM_ReadMagData(ICM20948 *dev, int16_t magn[3]) {
	uint8_t mag_buffer[10];

	      mag_buffer[0] =ICM_Mag_Read(dev, 0x01);

	      mag_buffer[1] =ICM_Mag_Read(dev, 0x11);
	  	  mag_buffer[2] =ICM_Mag_Read(dev, 0x12);
	  	  magn[0]=mag_buffer[1]|mag_buffer[2]<<8;
	    	mag_buffer[3] =ICM_Mag_Read(dev, 0x13);
	      mag_buffer[4] =ICM_Mag_Read(dev, 0x14);
	    	magn[1]=mag_buffer[3]|mag_buffer[4]<<8;
	  	 	mag_buffer[5] =ICM_Mag_Read(dev, 0x15);
	      mag_buffer[6] =ICM_Mag_Read(dev, 0x16);
	  		magn[2]=mag_buffer[5]|mag_buffer[6]<<8;

	     	i2c_Mag_write(dev, 0x31,0x01);
}

/*
 *
 * Sequence to setup ICM290948 as early as possible after power on
 *
 */
void ICM_PowerOn(ICM20948 *dev, SPI_HandleTypeDef *spiHandle) {

	dev->spiHandle = spiHandle;

	dev->accel_data[0] = 0;
	dev->accel_data[1] = 0;
	dev->accel_data[2] = 0;

	dev->gyro_data[0] = 0;
	dev->gyro_data[1] = 0;
	dev->gyro_data[2] = 0;

	dev->mag_data[0] = 0;
	dev->mag_data[1] = 0;
	dev->mag_data[2] = 0;

	char uart_buffer[200];
	uint8_t whoami = 0xEA;
	uint8_t test = ICM_WHOAMI(dev);
	//if (test == whoami) {
		ICM_CSHigh();
		HAL_Delay(10);
		ICM_SelectBank(dev, USER_BANK_0);
		HAL_Delay(10);
		ICM_Disable_I2C(dev);
		HAL_Delay(10);
		ICM_SetClock(dev, (uint8_t)CLK_BEST_AVAIL);
		HAL_Delay(10);
		ICM_AccelGyroOff(dev);
		HAL_Delay(20);
		ICM_AccelGyroOn(dev);
		HAL_Delay(10);
		ICM_Initialize(dev);
	//} else {
		//sprintf(uart_buffer, "Failed WHO_AM_I.  %i is not 0xEA\r\n", test);
		//HAL_UART_Transmit_DMA(UART_BUS, (uint8_t*) uart_buffer, strlen(uart_buffer));
		//HAL_Delay(100);
	//}
}
uint16_t ICM_Initialize(ICM20948 *dev) {
		ICM_SelectBank(dev, USER_BANK_2);
		HAL_Delay(20);
		ICM_SetGyroRateLPF(dev, GYRO_RATE_250, GYRO_LPF_17HZ);
		HAL_Delay(10);

		// Set gyroscope sample rate to 100hz (0x0A) in GYRO_SMPLRT_DIV register (0x00)
		ICM_WriteOneByte(dev, 0x00, 0x0A);
		HAL_Delay(10);

		// Set accelerometer low pass filter to 136hz (0x11) and the rate to 8G (0x04) in register ACCEL_CONFIG (0x14)
		ICM_WriteOneByte(dev, 0x14, (0x04 | 0x11));

		// Set accelerometer sample rate to 225hz (0x00) in ACCEL_SMPLRT_DIV_1 register (0x10)
		ICM_WriteOneByte(dev, 0x10, 0x00);
		HAL_Delay(10);

		// Set accelerometer sample rate to 100 hz (0x0A) in ACCEL_SMPLRT_DIV_2 register (0x11)
		ICM_WriteOneByte(dev, 0x11, 0x0A);
		HAL_Delay(10);

		ICM_SelectBank(dev, USER_BANK_2);
		HAL_Delay(20);

		// Configure AUX_I2C Magnetometer (onboard ICM-20948)
		ICM_WriteOneByte(dev, 0x7F, 0x00); // Select user bank 0
		ICM_WriteOneByte(dev, 0x0F, 0x30); // INT Pin / Bypass Enable Configuration
		ICM_WriteOneByte(dev, 0x03, 0x20); // I2C_MST_EN
		ICM_WriteOneByte(dev, 0x7F, 0x30); // Select user bank 3
		ICM_WriteOneByte(dev, 0x01, 0x4D); // I2C Master mode and Speed 400 kHz
		ICM_WriteOneByte(dev, 0x02, 0x01); // I2C_SLV0 _DLY_ enable
		ICM_WriteOneByte(dev, 0x05, 0x81); // enable IIC	and EXT_SENS_DATA==1 Byte

		// Initialize magnetometer
		i2c_Mag_write(dev, 0x32, 0x01); // Reset AK8963
		HAL_Delay(1000);
		i2c_Mag_write(dev, 0x31, 0x02); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output

		return 1337;
	}

void ICM_ReadAccelGyroData(ICM20948 *dev) {
	uint8_t raw_data[12];
	ICM_readBytes(dev, 0x2D, raw_data, 12);

	dev->accel_data[0] = (raw_data[0] << 8) | raw_data[1];
	dev->accel_data[1] = (raw_data[2] << 8) | raw_data[3];
	dev->accel_data[2] = (raw_data[4] << 8) | raw_data[5];

	dev->gyro_data[0] = (raw_data[6] << 8) | raw_data[7];
	dev->gyro_data[1] = (raw_data[8] << 8) | raw_data[9];
	dev->gyro_data[2] = (raw_data[10] << 8) | raw_data[11];

	dev->accel_data[0] = dev->accel_data[0] / 8;
	dev->accel_data[1] = dev->accel_data[1] / 8;
	dev->accel_data[2] = dev->accel_data[2] / 8;

	dev->gyro_data[0] = dev->gyro_data[0] / 250;
	dev->gyro_data[1] = dev->gyro_data[1] / 250;
	dev->gyro_data[2] = dev->gyro_data[2] / 250;
}
void ICM_SelectBank(ICM20948 *dev, uint8_t bank) {
	ICM_WriteOneByte(dev, USER_BANK_SEL, bank);
}
void ICM_Disable_I2C(ICM20948 *dev) {
	ICM_WriteOneByte(dev, 0x03, 0x78);
}
void ICM_CSHigh(void) {
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, SET);
}
void ICM_CSLow(void) {
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, RESET);
}
void ICM_SetClock(ICM20948 *dev, uint8_t clk) {
	ICM_WriteOneByte(dev, PWR_MGMT_1, clk);
}
void ICM_AccelGyroOff(ICM20948 *dev) {
	ICM_WriteOneByte(dev, PWR_MGMT_2, (0x38 | 0x07));
}
void ICM_AccelGyroOn(ICM20948 *dev) {
	ICM_WriteOneByte(dev, 0x07, (0x00 | 0x00));
}
uint8_t ICM_WHOAMI(ICM20948 *dev) {
	uint8_t spiData = 0x01;
	ICM_ReadOneByte(dev, 0x00, &spiData);
	return spiData;
}
void ICM_SetGyroRateLPF(ICM20948 *dev, uint8_t rate, uint8_t lpf) {
	ICM_WriteOneByte(dev, GYRO_CONFIG_1, (rate|lpf));
}
/*
 *
 * Read Accelerometer and Gyro data
 *
 */
