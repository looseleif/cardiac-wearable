/*****************************************************************************
  ICM20948.h

  Apr 13, 2023
  Vaibhav Shirole
******************************************************************************/
#include "main.h"
#include "ICM20948.h"
#include <string.h>
#include "BH1790GLC.h"

/*****************************************************************************
ACCELERATION FUNCTION
******************************************************************************/
// Function to check if the acceleration threshold is exceeded and reset the sample_index
void check_accel_threshold(ICM20948 *dev) {
    int32_t magnitude = (int32_t)dev->accel_data[0] * dev->accel_data[0] +      // Calculate the magnitude of the accelerometer data
                        (int32_t)dev->accel_data[1] * dev->accel_data[1] +
                        (int32_t)dev->accel_data[2] * dev->accel_data[2];
    // Compare the calculated magnitude with the threshold
    if (magnitude >= (ACCEL_THRESHOLD * ACCEL_THRESHOLD)) {   // Instead of taking the square root to find the magnitude I'm squaring the threshold

        //dev->sample_index = 0;
    }
}

/*****************************************************************************
INIT FUNCTIONS
******************************************************************************/
/*
 * Sequence to setup ICM290948 as early as possible after power on
 */
uint8_t ICM_PowerOn(ICM20948 *dev, SPI_HandleTypeDef *spiHandle) {
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

	/* Take initial reads to make sure SPI is working */
	uint8_t ret;
	ret = ICM_WHOAMI(dev);
	if (ret != WHO_AM_I){
		return ERR_WHO_AM_I;
	}

	/* SPI is working -> start setting up the sensor */
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

	return SUCCESS;
}

uint8_t ICM_Initialize(ICM20948 *dev) {
	ICM_SelectBank(dev, USER_BANK_2);
	HAL_Delay(20);
	ICM_SetGyroRateLPF(dev, GYRO_RATE_250, GYRO_LPF_17HZ);
	HAL_Delay(10);

	// Set gyroscope sample rate to 100hz (0x0A) in GYRO_SMPLRT_DIV register (0x00)
	ICM_WriteOneByte(dev, 0x00, 0x0A);
	HAL_Delay(10);

	// Set accelerometer low pass filter to 136hz (0x11) and the rate to 16G (0x0C) in register ACCEL_CONFIG (0x14)
	ICM_WriteOneByte(dev, 0x14, (0x0C | 0x11));	// 8G --> 0x04

	// Set accelerometer sample rate to 225hz (0x00) in ACCEL_SMPLRT_DIV_1 register (0x10)
	ICM_WriteOneByte(dev, 0x10, 0x00);
	HAL_Delay(10);

	// Set accelerometer sample rate to 100 hz (0x0A) in ACCEL_SMPLRT_DIV_2 register (0x11)
	ICM_WriteOneByte(dev, 0x11, 0x0A);
	HAL_Delay(10);

	ICM_SelectBank(dev, USER_BANK_2);
	HAL_Delay(20);

//	// Configure AUX_I2C Magnetometer (onboard ICM-20948)
//	ICM_WriteOneByte(dev, 0x7F, 0x00); // Select user bank 0
//	ICM_WriteOneByte(dev, 0x0F, 0x30); // INT Pin / Bypass Enable Configuration
//	ICM_WriteOneByte(dev, 0x03, 0x20); // I2C_MST_EN
//	ICM_WriteOneByte(dev, 0x7F, 0x30); // Select user bank 3
//	ICM_WriteOneByte(dev, 0x01, 0x4D); // I2C Master mode and Speed 400 kHz
//	ICM_WriteOneByte(dev, 0x02, 0x01); // I2C_SLV0 _DLY_ enable
//	ICM_WriteOneByte(dev, 0x05, 0x81); // enable IIC	and EXT_SENS_DATA==1 Byte
//
//	// Initialize magnetometer
//	ICM_Mag_Write(dev, 0x32, 0x01); // Reset AK8963
//	HAL_Delay(1000);
//	ICM_Mag_Write(dev, 0x31, 0x02); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output

	return SUCCESS;
}


/*****************************************************************************
DATA ACQUISITION FUNCTIONS
******************************************************************************/
/*
 * Read Accelerometer and Gyro data
 */
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

	check_accel_threshold(dev);
}

/*
 * Read magnetometer
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

	ICM_Mag_Write(dev, 0x31,0x01);
}


/*****************************************************************************
CONFIG HELPERS
******************************************************************************/
uint8_t ICM_WHOAMI(ICM20948 *dev) {
	uint8_t spiData = 0x01;
	ICM_ReadOneByte(dev, 0x00, &spiData);
	return spiData;
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

void ICM_SetGyroRateLPF(ICM20948 *dev, uint8_t rate, uint8_t lpf) {
	ICM_WriteOneByte(dev, GYRO_CONFIG_1, (rate|lpf));
}


/*****************************************************************************
LOW-LEVEL FUNCTIONS
******************************************************************************/
void ICM_readBytes(ICM20948 *dev, uint8_t reg, uint8_t *pData, uint16_t Size)
{
	reg = reg | 0x80;
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->spiHandle, &reg, 1, 100);
	HAL_SPI_Receive(dev->spiHandle, pData, Size, 100);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);
}

void ICM_WriteBytes(ICM20948 *dev, uint8_t reg, uint8_t *pData, uint16_t Size)
{
	reg = reg & 0x7F;
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->spiHandle, &reg, 1, 100);
	HAL_SPI_Transmit(dev->spiHandle, pData, Size, 100);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);

}

void ICM_ReadOneByte(ICM20948 *dev, uint8_t reg, uint8_t* pData)
{
	HAL_StatusTypeDef status;

	reg = reg | 0x80;													//first bit 1 indicates read
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);	//enable SPI
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);			//turn off green LED

	status = HAL_SPI_Transmit(dev->spiHandle, &reg, 1, 100);
	status = HAL_SPI_Receive(dev->spiHandle, pData, 1, 100);

	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);		//disable SPI
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);			//turn on green LED
}

void ICM_WriteOneByte(ICM20948 *dev, uint8_t reg, uint8_t Data)
{
	HAL_StatusTypeDef status;

	reg = reg & 0x7F;													//first bit 0 indicates write
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);	//set CS pin low
	status = HAL_SPI_Transmit(dev->spiHandle, &reg, 1, 100);
	status = HAL_SPI_Transmit(dev->spiHandle, &Data, 1, 100);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);		//set CS pin high
}

/*
 * AUX abstraction for magnetometer
 * these are actually a level higher than the SPI read and writes above
 */
void ICM_Mag_Write(ICM20948 *dev, uint8_t reg,uint8_t value)
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

uint8_t ICM_Mag_Read(ICM20948 *dev, uint8_t reg)
{
	uint8_t  Data;
	ICM_WriteOneByte(dev, 0x7F, 0x30);
	HAL_Delay(1);
	ICM_WriteOneByte(dev, 0x03, 0x0C|0x80);
	HAL_Delay(1);
	ICM_WriteOneByte(dev, 0x04, reg);// set reg addr
	HAL_Delay(1);
	ICM_WriteOneByte(dev, 0x06, 0xff);//read
	HAL_Delay(1);
	ICM_WriteOneByte(dev, 0x7F, 0x00);
	ICM_ReadOneByte(dev, 0x3B, &Data);
	HAL_Delay(1);
	return Data;
}
