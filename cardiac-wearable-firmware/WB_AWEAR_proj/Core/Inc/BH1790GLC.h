/*****************************************************************************
  BH1790GLC.h

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

#include "stm32wbxx_hal.h"						// Required library for I2C

#ifndef _BH1790GLC_H_
#define _BH1790GLC_H_

/* PERIPHERAL REGISTERS */
#define BH1790GLC_DEVICE_ADDRESS      (0x5B)    // 8 bit Slave Address (1011011)
#define BH1790GLC_MID_VAL             (0xE0)	// Default Manufacturer ID value
#define BH1790GLC_PID_VAL             (0x0D)	// Default Part ID value

#define BH1790GLC_MANUFACTURER_ID     (0x0F)
#define BH1790GLC_PART_ID             (0x10)

#define BH1790GLC_MEAS_CONTROL1       (0x41)
#define BH1790GLC_MEAS_CONTROL2		  (0x42)
#define BH1790GLC_MEAS_START	      (0x43)

#define BH1790GLC_DATAOUT_LEDOFF      (0x54)

#define BH1790GLC_RESET				  (0x40)

#define BH1790GLC_MEAS_CONTROL1_RDY                     (1 << 7)
#define BH1790GLC_MEAS_CONTROL1_LED_LIGHTING_FREQ_128HZ (0 << 2)
#define BH1790GLC_MEAS_CONTROL1_RCYCLE_32HZ             (2 << 0)

#define BH1790GLC_MEAS_CONTROL2_LED_EN_00               (0 << 6)
#define BH1790GLC_MEAS_CONTROL2_LED_ON_TIME_0_3MS       (0 << 5)
#define BH1790GLC_MEAS_CONTROL2_LED_CURRENT_10MA        (12 << 0)

#define BH1790GLC_MEAS_START_MEAS_ST                    (1 << 0)

#define BH1790GLC_MEAS_CONTROL1_VAL   (BH1790GLC_MEAS_CONTROL1_RDY | BH1790GLC_MEAS_CONTROL1_LED_LIGHTING_FREQ_128HZ | BH1790GLC_MEAS_CONTROL1_RCYCLE_32HZ)
#define BH1790GLC_MEAS_CONTROL2_VAL   (BH1790GLC_MEAS_CONTROL2_LED_EN_00 | BH1790GLC_MEAS_CONTROL2_LED_ON_TIME_0_3MS | BH1790GLC_MEAS_CONTROL2_LED_CURRENT_10MA)
#define BH1790GLC_MEAS_START_VAL      (BH1790GLC_MEAS_START_MEAS_ST)

#define BH1790GLC_SWRESET								(1 << 7)

/* ERROR HANDLING */
#define SUCCESS				0

#define ERR_MID_VAL			10
#define ERR_PID_VAL			11

#define ERR_MEAS_CONTROL1	20
#define ERR_MEAS_CONTROL2	21
#define ERR_MEAS_START		22

#define ERR_DATA_OUT		30

#define ERR_RESET			90

/* SENSOR STRUCT */
typedef struct {
	I2C_HandleTypeDef *i2cHandle;

	uint16_t ppg_data[2];

} BH1790GLC;

/*
 * INITIALIZATION
 * set up for measuring in this function
 */
uint8_t BH1790GLC_init( BH1790GLC *dev, I2C_HandleTypeDef *i2cHandle );

/*
 * DATA ACQUISITION
 */
uint8_t get_val( BH1790GLC *dev );
uint8_t reset_device( BH1790GLC *dev );
uint8_t param_refreshment( BH1790GLC *dev );

/*
 * LOW-LEVEL FUNCTIONS
 * reads and writes to the peripheral
 */
HAL_StatusTypeDef write( BH1790GLC *dev, uint8_t reg, uint8_t *data );
HAL_StatusTypeDef read( BH1790GLC *dev, uint8_t reg, uint8_t *data );
HAL_StatusTypeDef many_reads( BH1790GLC *dev, uint8_t reg, uint8_t *data, uint8_t length );

#endif // _BH1790GLC_H_
