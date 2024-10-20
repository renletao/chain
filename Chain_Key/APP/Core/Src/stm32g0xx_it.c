/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32g0xx_it.c
 * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "string.h"
#include "base_function.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern __IO uint8_t g_uart_in_rx_buf[2][BUFFER_SIZE];
extern __IO uint8_t g_uart_out_rx_buf[2][BUFFER_SIZE];
extern __IO uint8_t g_uart_in_tx_status;
extern __IO uint8_t g_uart_out_tx_status;
extern __IO uint8_t g_uart_in_transmit_commplete;
extern __IO uint8_t g_uart_out_transmit_commplete;
extern __IO uint8_t g_cmd_buf[BUFFER_SIZE];
extern __IO uint8_t g_cmd_size;
extern __IO uint8_t g_cmd_status;
extern __IO uint8_t g_tail_status;
extern __IO uint8_t g_heart_beat_record;

static uint8_t s_len = 0;
static uint8_t s_enum_please_nums = ENUM_PLEASE_NUM;
static uint8_t s_data_pack_type = 0;
static uint8_t s_enum_please_data_packet_buf[9] = {0xAA, 0x55, 0x03, 0x00, 0xFF,
												   0xFC, 0xFB, 0x55, 0xAA};
uint8_t g_heartbeat_data_pack_buf[9] = {0xAA, 0x55, 0x03, 0x00, 0xFF, 0xFD, 0xFC,
										0x55, 0xAA};
uint8_t g_heartbeat_reply_status_buf[3] = {0};
uint8_t g_reply_index = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * @brief Get the UART input data packet type based on the contents of the buffer.
 * @note This function checks the data packet in the buffer to determine its type.
 *       It verifies the packet integrity using the `pack_check` function, and then
 *       inspects specific bytes in the buffer to classify the packet type.
 *
 * @param buffer Pointer to the data buffer containing the incoming packet.
 * @param size Length of the data in the buffer.
 * @retval The type of data packet as defined by various enumeration constants,
 *         or an error code if the packet check fails.
 */
uint8_t uart_in_data_pack_type(uint8_t *buffer, uint16_t size)
{
	// Check if the packet is valid
	if (pack_check(buffer, size) == CHECK_PASS)
	{
		// Determine the packet type based on specific bytes in the buffer
		if (buffer[5] == CHAIN_ENUM)
		{
			return CHAIN_ENUM_DATA_PACK; // Packet type: Chain Enum
		}
		else if (buffer[5] == CHAIN_HEARTBEAT)
		{
			return CHAIN_HEARTBEAT_DATA_PACK; // Packet type: Chain Heartbeat
		}
		else if (buffer[4] != 1)
		{
			return CHAIN_TRANSMIT_DATA_PACK; // Packet type: Chain Transmit
		}
		else if (buffer[4] == 1)
		{
			return CHAIN_HANDLE_DATA_PACK; // Packet type: Chain Handle
		}
	}
	return ERROR_DATA_PACK; // Return error code for invalid packet
}

/**
 * @brief Get the UART output data packet type based on the contents of the buffer.
 * @note This function checks the data packet in the buffer to determine its type.
 *       It verifies the packet integrity using the `pack_check` function, and then
 *       inspects specific bytes in the buffer to classify the packet type for output.
 *
 * @param buffer Pointer to the data buffer containing the outgoing packet.
 * @param size Length of the data in the buffer.
 * @retval The type of data packet as defined by various enumeration constants,
 *         or an error code if the packet check fails.
 */
uint8_t uart_out_data_pack_type(uint8_t *buffer, uint16_t size)
{
	// Check if the packet is valid
	if (pack_check(buffer, size) == CHECK_PASS)
	{
		// Determine the packet type based on specific bytes in the buffer
		if (buffer[5] == CHAIN_HEARTBEAT)
		{
			return CHAIN_HEARTBEAT_DATA_PACK; // Packet type: Chain Heartbeat
		}
		else if (buffer[5] == CHAIN_ENUM)
		{
			return CHAIN_ENUM_RETURN_DATA_PACK; // Packet type: Chain Enum Return
		}
		else if (buffer[5] == CHAIN_ENUM_PLEASE)
		{
			return CHAIN_ENUM_PLEASE_DATA_PACK; // Packet type: Chain Enum Please
		}
		else
		{
			return CHAIN_TRANSMIT_DATA_PACK; // Packet type: Chain Transmit
		}
	}
	return ERROR_DATA_PACK; // Return error code for invalid packet
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1)
	{
	}
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles Flash global interrupt.
  */
void FLASH_IRQHandler(void)
{
  /* USER CODE BEGIN FLASH_IRQn 0 */

  /* USER CODE END FLASH_IRQn 0 */
  HAL_FLASH_IRQHandler();
  /* USER CODE BEGIN FLASH_IRQn 1 */

  /* USER CODE END FLASH_IRQn 1 */
}

/**
  * @brief This function handles EXTI line 0 and line 1 interrupts.
  */
void EXTI0_1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_1_IRQn 0 */

  /* USER CODE END EXTI0_1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(Key_Pin);
  /* USER CODE BEGIN EXTI0_1_IRQn 1 */

  /* USER CODE END EXTI0_1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */
	if (LL_DMA_IsActiveFlag_TC2(DMA1))
	{
		LL_DMA_ClearFlag_TC2(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
		g_uart_in_transmit_commplete = 1;
	}
  /* USER CODE END DMA1_Channel2_3_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 4, channel 5 and DMAMUX1 interrupts.
  */
void DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Ch4_5_DMAMUX1_OVR_IRQn 0 */
	if (LL_DMA_IsActiveFlag_TC4(DMA1))
	{
		LL_DMA_ClearFlag_TC4(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
		g_uart_out_transmit_commplete = 1;
	}
	if (LL_DMA_IsActiveFlag_TC5(DMA1))
	{
		LL_DMA_ClearFlag_GI5(DMA1);
		LL_DMA_ClearFlag_TC5(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
		LL_TIM_DisableCounter(TIM1);
	}
  /* USER CODE END DMA1_Ch4_5_DMAMUX1_OVR_IRQn 0 */

  /* USER CODE BEGIN DMA1_Ch4_5_DMAMUX1_OVR_IRQn 1 */

  /* USER CODE END DMA1_Ch4_5_DMAMUX1_OVR_IRQn 1 */
}

/**
  * @brief This function handles TIM14 global interrupt.
  */
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */
	if (LL_TIM_IsActiveFlag_UPDATE(TIM14) == SET)
	{
		s_enum_please_nums--;
		LL_TIM_ClearFlag_UPDATE(TIM14); // Clear the timer update flag
		usart1_transmit_dma(s_enum_please_data_packet_buf,
							sizeof(s_enum_please_data_packet_buf));
		if (s_enum_please_nums == 0)
		{
			LL_TIM_DisableIT_UPDATE(TIM14); // DISABLE TIM14
			LL_TIM_DisableCounter(TIM14);	// DISABLE TIM14
			LL_TIM_ClearFlag_UPDATE(TIM16); // Clear update TIM16
			LL_TIM_EnableIT_UPDATE(TIM16);	// ENABLE TIM16
			LL_TIM_EnableCounter(TIM16);	// ENABLE TIM16
		}
	}
  /* USER CODE END TIM14_IRQn 0 */
  /* USER CODE BEGIN TIM14_IRQn 1 */

  /* USER CODE END TIM14_IRQn 1 */
}

/**
  * @brief This function handles TIM16 global interrupt.
  */
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */
	if (LL_TIM_IsActiveFlag_UPDATE(TIM16) == SET)
	{
		LL_TIM_ClearFlag_UPDATE(TIM16);
		usart2_transmit_dma(g_heartbeat_data_pack_buf, sizeof(g_heartbeat_data_pack_buf));
		g_heartbeat_reply_status_buf[g_reply_index] = 0;
		g_reply_index++;				   // Increment the reply index
		g_reply_index = g_reply_index % 3; // Wrap around if index exceeds 2

		if (g_heartbeat_reply_status_buf[0] == 0 && g_heartbeat_reply_status_buf[1] == 0 && g_heartbeat_reply_status_buf[2] == 0)
		{
			if (g_tail_status == CHAIN_NON_TAIL_DEVICE)
			{
				LL_TIM_EnableIT_UPDATE(TIM14); // ENABLE TIM14
				LL_TIM_EnableCounter(TIM14);   // ENABLE TIM14
				s_enum_please_nums = ENUM_PLEASE_NUM;
			}
			g_tail_status = CHAIN_TAIL_DEVICE;
		}
		else
		{
			g_tail_status = CHAIN_NON_TAIL_DEVICE;
		}
	}
  /* USER CODE END TIM16_IRQn 0 */
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	if (LL_USART_IsActiveFlag_IDLE(USART1) && LL_USART_IsEnabledIT_IDLE(USART1))
	{
		// Clear the idle interrupt flag
		LL_USART_ClearFlag_IDLE(USART1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
		s_len = BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_1);
		// Switch buffer and enable DMA reception
		g_uart_in_tx_status = 1 - g_uart_in_tx_status;
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1,
								(uint32_t)g_uart_in_rx_buf[g_uart_in_tx_status]);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, BUFFER_SIZE);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
		LL_USART_EnableDMAReq_RX(USART1);
		if (s_len > 0)
		{
			s_data_pack_type = uart_in_data_pack_type(
				(uint8_t *)g_uart_in_rx_buf[1 - g_uart_in_tx_status], s_len);
			switch (s_data_pack_type)
			{
			case CHAIN_ENUM_DATA_PACK:
				chain_enum_handle((uint8_t *)g_uart_in_rx_buf[1 - g_uart_in_tx_status],
								  s_len);
				break;
			case CHAIN_HEARTBEAT_DATA_PACK:
				chain_heartbeat_in_receive_handle();
				break;
			case CHAIN_TRANSMIT_DATA_PACK:
				chain_out_relay_handle(
					(uint8_t *)g_uart_in_rx_buf[1 - g_uart_in_tx_status], s_len);
				break;
			case CHAIN_HANDLE_DATA_PACK:
				chain_deal_data_packet_handle(
					(uint8_t *)g_uart_in_rx_buf[1 - g_uart_in_tx_status], s_len);
				break;
			default:
				break;
			}
		}
	}
  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	if (LL_USART_IsActiveFlag_IDLE(USART2) && LL_USART_IsEnabledIT_IDLE(USART2))
	{
		// Clear the idle interrupt flag
		LL_USART_ClearFlag_IDLE(USART2);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
		s_len = BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_3);
		// Switch buffer and enable DMA reception
		g_uart_out_tx_status = 1 - g_uart_out_tx_status;
		LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3,
								(uint32_t)g_uart_out_rx_buf[g_uart_out_tx_status]);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, BUFFER_SIZE);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
		LL_USART_EnableDMAReq_RX(USART2);
		if (s_len > 0)
		{
			s_data_pack_type = uart_out_data_pack_type(
				(uint8_t *)g_uart_out_rx_buf[1 - g_uart_out_tx_status], s_len);
			switch (s_data_pack_type)
			{
			case CHAIN_HEARTBEAT_DATA_PACK:
				chain_heartbeat_out_receive_handle();
				break;
			case CHAIN_ENUM_RETURN_DATA_PACK:
				chain_enum_return_handle(
					(uint8_t *)g_uart_out_rx_buf[1 - g_uart_out_tx_status], s_len);
				break;
			case CHAIN_ENUM_PLEASE_DATA_PACK:
				chain_enum_please_handle(
					(uint8_t *)g_uart_out_rx_buf[1 - g_uart_out_tx_status], s_len);
				break;
			case CHAIN_TRANSMIT_DATA_PACK:
				chain_in_relay_handle(
					(uint8_t *)g_uart_out_rx_buf[1 - g_uart_out_tx_status], s_len);
				break;
			default:
				break;
			}
		}
	}
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
