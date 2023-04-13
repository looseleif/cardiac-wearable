/*****************************************************************************
  BH1790GLC.cpp

  Apr 13, 2023
  Vaibhav Shirole
******************************************************************************/
#include "BH1790GLC.h"


/*****************************************************************************
INIT FUNCTION
******************************************************************************/
uint8_t BH1790GLC_init( BH1790GLC *dev, I2C_HandleTypeDef *i2cHandle )
{
	/* Set struct parameters */
	dev->i2cHandle = i2cHandle;
	dev->ppg_data[0] = 0;
	dev->ppg_data[1] = 0;

	dev->samples_index = 0;

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


/*****************************************************************************
DATA ACQUISITION FUNCTIONS
******************************************************************************/
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

uint8_t add_sample( BH1790GLC *dev ){


	dev->ppg_samples[dev->samples_index] = dev->ppg_data[1];
	dev->samples_index++;

	if(dev->samples_index==500){

		dev->samples_index = 0;
		return 1;

	}

	return 0;
}

uint8_t ppg_calculate( BH1790GLC *dev ){

	  uint16_t smooth_array[500];
	  int deriv_array[499];

	  uint16_t raw_data_1;
	  float smooth_data_1 = 0;
	  float lpf_beta = 0.08;

	  for(int i = 0; i < 500; i++){

		  raw_data_1 = dev->ppg_samples[i];
		  smooth_data_1 = smooth_data_1 - (lpf_beta*(smooth_data_1 - raw_data_1));
		  smooth_array[i] = smooth_data_1;
		  if(i!=0){


			  int deriv = (int)smooth_array[i] - (int)smooth_array[i-1];
			  if(deriv<255 || deriv>-255){

				 deriv_array[i] = deriv;

			  } else {

				 deriv_array[i] = deriv_array[i-1];

			  }

		  }

	  }

	  int pos = 0;
	  int neg = 0;

	  int pos_check = 0;
	  int neg_check = 0;

	  int peak_vals[20] = {};
	  int peak_count = 0;


	  for(int j = 0; j < 499; j++){



		  if(deriv_array[j]>0 && pos_check==0){

			  neg = 0;
			  pos++;

			  if(pos==3){

				  pos_check = 1;

			  }

		  } else if (deriv_array[j]<0 && pos_check==1) {

			  pos = 0;
			  neg++;

			  if(neg==3){

				  neg_check = 1;
				  peak_vals[peak_count] = j;
				  peak_count++;

				  pos_check = 0;
				  neg_check = 0;

			  }


		  } else if (deriv_array[j]<0 && pos_check==0){
			  pos = 0;
		  }

	  }

	  printf("{");

	  for(int k = 0; k<20; k++){

		  printf("[%d],", peak_vals[k]);

	  }
	  printf("}\n");

	  double sum = 0;

	  for(int k =1; k<peak_count; k++){

		  sum += peak_vals[k] - peak_vals[k-1];

	  }

	  printf("{{{%d}}}\n",(uint8_t)sum/(peak_count-1));

}


/*****************************************************************************
LOW-LEVEL FUNCTIONS
******************************************************************************/
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

//uint16_t raw_data[160] = {
//
//		  21893,
//		  21887,
//		  21887,
//		  21882,
//		  21904,
//		  21929,
//		  21926,
//		  21948,
//		  21965,
//		  21982,
//		  21998,
//		  21998,
//		  21995,
//		  22020,
//		  22023,
//		  22032,
//		  22023,
//		  22035,
//		  22045,
//		  22041,
//		  22026,
//		  21952,
//		  21857,
//		  21817,
//		  21818,
//		  21834,
//		  21832,
//		  21838,
//		  21856,
//		  21842,
//		  21831,
//		  21825,
//		  21828,
//		  21834,
//		  21848,
//		  21867,
//		  21896,
//		  21897,
//		  21902,
//		  21896,
//		  21914,
//		  21917,
//		  21923,
//		  21941,
//		  21930,
//		  21947,
//		  21961,
//		  21965,
//		  21981,
//		  21967,
//		  21873,
//		  21785,
//		  21742,
//		  21756,
//		  21780,
//		  21791,
//		  21788,
//		  21797,
//		  21812,
//		  21803,
//		  21798,
//		  21793,
//		  21807,
//		  21823,
//		  21829,
//		  21848,
//		  21874,
//		  21879,
//		  21894,
//		  21920,
//		  21932,
//		  21930,
//		  21941,
//		  21966,
//		  21961,
//		  21975,
//		  21991,
//		  21997,
//		  22017,
//		  21943,
//		  21840,
//		  21800,
//		  21794,
//		  21812,
//		  21816,
//		  21828,
//		  21827,
//		  21836,
//		  21845,
//		  21841,
//		  21836,
//		  21834,
//		  21862,
//		  21881,
//		  21897,
//		  21904,
//		  21929,
//		  21934,
//		  21944,
//		  21963,
//		  21978,
//		  21983,
//		  21990,
//		  22006,
//		  22021,
//		  22024,
//		  22041,
//		  22043,
//		  21968,
//		  21867,
//		  21800,
//		  21810,
//		  21843,
//		  21846,
//		  21837,
//		  21873,
//		  21872,
//		  21868,
//		  21860,
//		  21867,
//		  21880,
//		  21914,
//		  21910,
//		  21943,
//		  21959,
//		  21967,
//		  21969,
//		  21979,
//		  21998,
//		  22002,
//		  22020,
//		  22030,
//		  22027,
//		  22047,
//		  22067,
//		  22048,
//		  22087,
//		  22062,
//		  21996,
//		  21881,
//		  21852,
//		  21844,
//		  21884,
//		  21890,
//		  21901,
//		  21904,
//		  21902,
//		  21916,
//		  21917,
//		  21922,
//		  21915,
//		  21957,
//		  21956,
//		  21963,
//		  21985,
//		  22011,
//		  22021,
//		  22038,
//		  22030,
//		  22038
//};
