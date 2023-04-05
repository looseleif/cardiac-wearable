/**
  ******************************************************************************
  * @file    UART/UART_Printf/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example shows how to retarget the C library printf function 
  *          to the UART.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <math.h>
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
   
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = ODD parity
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance          = USARTx;
  
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Output a message on Hyperterminal using printf function */
  printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");


  uint16_t raw_data[160] = {

		  21893,
		  21887,
		  21887,
		  21882,
		  21904,
		  21929,
		  21926,
		  21948,
		  21965,
		  21982,
		  21998,
		  21998,
		  21995,
		  22020,
		  22023,
		  22032,
		  22023,
		  22035,
		  22045,
		  22041,
		  22026,
		  21952,
		  21857,
		  21817,
		  21818,
		  21834,
		  21832,
		  21838,
		  21856,
		  21842,
		  21831,
		  21825,
		  21828,
		  21834,
		  21848,
		  21867,
		  21896,
		  21897,
		  21902,
		  21896,
		  21914,
		  21917,
		  21923,
		  21941,
		  21930,
		  21947,
		  21961,
		  21965,
		  21981,
		  21967,
		  21873,
		  21785,
		  21742,
		  21756,
		  21780,
		  21791,
		  21788,
		  21797,
		  21812,
		  21803,
		  21798,
		  21793,
		  21807,
		  21823,
		  21829,
		  21848,
		  21874,
		  21879,
		  21894,
		  21920,
		  21932,
		  21930,
		  21941,
		  21966,
		  21961,
		  21975,
		  21991,
		  21997,
		  22017,
		  21943,
		  21840,
		  21800,
		  21794,
		  21812,
		  21816,
		  21828,
		  21827,
		  21836,
		  21845,
		  21841,
		  21836,
		  21834,
		  21862,
		  21881,
		  21897,
		  21904,
		  21929,
		  21934,
		  21944,
		  21963,
		  21978,
		  21983,
		  21990,
		  22006,
		  22021,
		  22024,
		  22041,
		  22043,
		  21968,
		  21867,
		  21800,
		  21810,
		  21843,
		  21846,
		  21837,
		  21873,
		  21872,
		  21868,
		  21860,
		  21867,
		  21880,
		  21914,
		  21910,
		  21943,
		  21959,
		  21967,
		  21969,
		  21979,
		  21998,
		  22002,
		  22020,
		  22030,
		  22027,
		  22047,
		  22067,
		  22048,
		  22087,
		  22062,
		  21996,
		  21881,
		  21852,
		  21844,
		  21884,
		  21890,
		  21901,
		  21904,
		  21902,
		  21916,
		  21917,
		  21922,
		  21915,
		  21957,
		  21956,
		  21963,
		  21985,
		  22011,
		  22021,
		  22038,
		  22030,
		  22038
  };

  uint16_t smooth_array[160];
  int deriv_array[159];

  uint16_t raw_data_1;
  float smooth_data_1 = 0;
  float lpf_beta = 0.15;

  for(int i = 0; i < 160; i++){

	  raw_data_1 = raw_data[i];
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


  for(int j = 0; j < 159; j++){



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

  for(int k = 0; k<20; k++){

	  printf("PEAK DETECTED: %d\n", peak_vals[k]);

  }





  while (1)
  {

      // Function that brings Fresh Data into RawData


  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows: 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
   /* Activate the OverDrive to reach the 180 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
