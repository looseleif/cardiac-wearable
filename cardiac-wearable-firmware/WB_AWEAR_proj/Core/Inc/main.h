/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"
#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_4
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_0
#define LD2_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_1
#define LD3_GPIO_Port GPIOB
#define JTMS_Pin GPIO_PIN_13
#define JTMS_GPIO_Port GPIOA
#define JTCK_Pin GPIO_PIN_14
#define JTCK_GPIO_Port GPIOA
#define B2_Pin GPIO_PIN_0
#define B2_GPIO_Port GPIOD
#define B3_Pin GPIO_PIN_1
#define B3_GPIO_Port GPIOD
#define LD1_Pin GPIO_PIN_5
#define LD1_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_6
#define STLINK_RX_GPIO_Port GPIOB
#define STLINK_TX_Pin GPIO_PIN_7
#define STLINK_TX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

extern uint8_t AFIB_DETECTED;		//initialized in main.c


#define ICM_CS_Pin GPIO_PIN_4	//previously PB3, set GPIO_Output on PB4 for now
#define ICM_CS_GPIO_Port GPIOB

#define ICM_SCK_Pin GPIO_PIN_1
#define ICM_SCK_GPIO_Port GPIOA
#define ICM_MISO_Pin GPIO_PIN_6
#define ICM_MISO_GPIO_Port GPIOA
#define ICM_MOSI_Pin GPIO_PIN_7
#define ICM_MOSI_GPIO_Port GPIOA

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
