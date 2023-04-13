/*****************************************************************************
  ICM20948.h

  Apr 13, 2023
  Vaibhav Shirole
******************************************************************************/
#include "stm32wbxx_hal.h"						// Required library for SPI


/*****************************************************************************
PERIPHERAL REGISTERS
******************************************************************************/
#ifndef ICM20948_H_
#define ICM20948_H_

#define USER_BANK_SEL	(0x7F)
#define USER_BANK_0		(0x00)
#define USER_BANK_1		(0x10)
#define USER_BANK_2		(0x20)
#define USER_BANK_3		(0x30)

#define PWR_MGMT_1 		(0x06)
#define PWR_MGMT_2		(0x07)
#define GYRO_CONFIG_1	(0x01)


#define CLK_BEST_AVAIL	(0x01)
#define GYRO_RATE_250	(0x00)
#define GYRO_LPF_17HZ 	(0x29)

/*****************************************************************************
SENSOR OBJECT
******************************************************************************/
typedef struct {
	SPI_HandleTypeDef *spiHandle;
	uint16_t accel_data[3];
	uint16_t gyro_data[3];
	int16_t mag_data[3];

} ICM20948;


/*****************************************************************************
INIT FUNCTIONS
******************************************************************************/
void ICM_PowerOn(ICM20948 *dev, SPI_HandleTypeDef *spiHandle);
uint16_t ICM_Initialize(ICM20948 *dev);


/*****************************************************************************
DATA ACQUISITION FUNCTIONS
******************************************************************************/
void ICM_ReadAccelGyroData(ICM20948 *dev);
void ICM_ReadMagData(ICM20948 *dev, int16_t magn[3]);


/*****************************************************************************
CONFIG HELPERS
******************************************************************************/
uint8_t ICM_WHOAMI(ICM20948 *dev);
void ICM_SelectBank(ICM20948 *dev, uint8_t bank);
void ICM_Disable_I2C(ICM20948 *dev);
void ICM_CSHigh(void);
void ICM_CSLow(void);
void ICM_SetClock(ICM20948 *dev, uint8_t clk);
void ICM_AccelGyroOff(ICM20948 *dev);
void ICM_AccelGyroOn(ICM20948 *dev);
void ICM_SetGyroRateLPF(ICM20948 *dev, uint8_t rate, uint8_t lpf);


/*****************************************************************************
LOW-LEVEL FUNCTIONS
- don't need to worry about these
******************************************************************************/
void ICM_readBytes(ICM20948 *dev, uint8_t reg, uint8_t *pData, uint16_t Size);
void ICM_WriteBytes(ICM20948 *dev, uint8_t reg, uint8_t *pData, uint16_t Size);
void ICM_ReadOneByte(ICM20948 *dev, uint8_t reg, uint8_t* pData);
void ICM_WriteOneByte(ICM20948 *dev, uint8_t reg, uint8_t Data);
void ICM_Mag_Write(ICM20948 *dev, uint8_t reg,uint8_t value);
uint8_t ICM_Mag_Read(ICM20948 *dev, uint8_t reg);

#endif /* ICM20948_H_ */
