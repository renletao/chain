/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
// DMA receive buffers
extern __IO uint8_t g_uart_in_rx_buf[2][BUFFER_SIZE]; // uart_in data buffer for receiving data via DMA
extern __IO uint8_t g_uart_out_rx_buf[2][BUFFER_SIZE]; // uart_out data buffer for receiving data via DMA
extern __IO uint8_t g_uart_in_tx_status;  // Status of the uart_in buffer, used in polling mode
extern __IO uint8_t g_uart_out_tx_status;  // Status of the uart_out buffer, used in polling mode

// Serial transmission complete flags
extern __IO uint8_t g_uart_in_transmit_commplete; // Serial 1 transmission complete flag
extern __IO uint8_t g_uart_out_transmit_commplete; // Serial 2 transmission complete flag

// Function declarations
void usart1_hart_init(void);                  // Initialize hardware configuration for USART1
void usart2_hart_init(void);                  // Initialize hardware configuration for USART2
void usart1_transmit_dma(uint8_t *data, uint16_t size); // Transmit data to USART1 using DMA
void usart2_transmit_dma(uint8_t *data, uint16_t size); // Transmit data to USART2 using DMA
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

