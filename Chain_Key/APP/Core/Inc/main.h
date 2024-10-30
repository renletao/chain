/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_iwdg.h"
#include "stm32g0xx_ll_tim.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_dma.h"

#include "stm32g0xx_ll_exti.h"

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

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Key_Pin GPIO_PIN_0
#define Key_GPIO_Port GPIOB
#define Key_EXTI_IRQn EXTI0_1_IRQn

/* USER CODE BEGIN Private defines */
#define APPLICATION_ADDRESS (0x08002000)  // App starting address
#define BUFFER_SIZE (512)                 // Buffer size
#define ENUM_PLEASE_NUM (3)               // Power on and send enumeration requests three times in a row.
#define RGB_LIGHT_BASE (40)               // The default RGB brightness is 40%

typedef enum {
	BUFFER_DATA_READY = 0x00, 	// Active data processing
	BUFFER_NO_DATA = 0x01,    	// No data processing
} tx_buffer_t;                  // Send buffer state

typedef enum {
	CMD_SPACE_IDLE_STATUS = 0x00,	// Idle state
	CMD_SPACE_BUSY_STATUS = 0x01,	// Busy state
} cmd_space_t;                     	// Instruction space state

typedef enum {
	CHAIN_NON_TAIL_DEVICE = 0x00, // Non-tail device
	CHAIN_TAIL_DEVICE             // Tail device
} tail_device_t; 				  // Tail device type

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
