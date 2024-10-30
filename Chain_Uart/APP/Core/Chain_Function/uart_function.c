#include "uart_function.h"

static uint8_t s_ret_buf[256] = { 0 };
static uint8_t s_ret_buf_size = 0;
static uint8_t s_i2c_buffer[256] = { 0 };
static uint16_t s_adc_buffer[2] = { 0 };
static uint8_t s_adc_size = 0;
__IO gpio_config_t g_gpio_status[2] = { { .mode = CHAIN_NOT_WORK_STATUS, }, {
		.mode = CHAIN_NOT_WORK_STATUS, } };
void clear_i2c_flag(void) {
	__disable_irq();
	// 清除I2C2的Busy标志（如果需要）
	if (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_BUSY)) {
		// 需要确保I2C不在忙状态
		__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_BUSY);
	}
	// 清除其他标志（例如超时）
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_TXE);
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_BERR);
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_ARLO);
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_OVR);
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_PECERR);
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_ALERT);
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_TIMEOUT);
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_AF);  // 清除应答失败标志
	__HAL_I2C_CLEAR_FLAG(&hi2c2, I2C_FLAG_STOPF); // 清除NACK接收标志
	HAL_I2C_DeInit(&hi2c2);
	MX_I2C2_Init(g_gpio_status[0].i2c_speed);
	__enable_irq();
}

bool check_i2c_mode(void) {
	if (g_gpio_status[0].mode == CHAIN_I2C_WORK_STATUS
			&& g_gpio_status[1].mode == CHAIN_I2C_WORK_STATUS) {
		return true;
	}
	return false;
}
void chain_uart_i2c_init(i2c_speed_t speed) {
	if (speed >= CHAIN_I2C_LOW_SPEED_100KHZ
			&& speed <= CHAIN_I2C_MID_SPEED_400KHZ) {
		__disable_irq();
		HAL_I2C_MspDeInit(&hi2c2);
		HAL_ADC_MspDeInit(&hadc1);
		MX_I2C2_Init(speed);
		__enable_irq();
		g_gpio_status[0].mode = CHAIN_I2C_WORK_STATUS;
		g_gpio_status[1].mode = CHAIN_I2C_WORK_STATUS;
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		chain_command_complete_return(CHAIN_I2C_INIT, s_ret_buf,
				s_ret_buf_size);
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
		chain_command_complete_return(CHAIN_I2C_INIT, s_ret_buf,
				s_ret_buf_size);
	}
}

void chain_i2c_read(uint8_t i2c_addr, uint8_t read_len) {
	if (check_i2c_mode()) {
		if (i2c_addr <= 127) {
			HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c2,
					(i2c_addr << 1), s_i2c_buffer, read_len, 1000);
			if (status == HAL_OK) {
				s_ret_buf_size = 0;
				s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
				for (uint8_t i = 0; i < read_len; i++) {
					s_ret_buf[s_ret_buf_size++] = s_i2c_buffer[i];
				}
				chain_command_complete_return(CHAIN_I2C_READ, s_ret_buf,
						s_ret_buf_size);
			} else {
				if (status == HAL_ERROR) {
					clear_i2c_flag();
				}
				s_ret_buf_size = 0;
				s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
				chain_command_complete_return(CHAIN_I2C_READ, s_ret_buf,
						s_ret_buf_size);
			}
		} else {
			s_ret_buf_size = 0;
			s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
			chain_command_complete_return(CHAIN_I2C_READ, s_ret_buf,
					s_ret_buf_size);
		}
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_WORK_MODE_MISMATCHED;
		chain_command_complete_return(CHAIN_I2C_READ, s_ret_buf,
				s_ret_buf_size);
	}
}
void chain_i2c_write(uint8_t i2c_addr, uint8_t *buffer, uint16_t write_len) {
	if (check_i2c_mode()) {
		if (i2c_addr <= 127) {
			HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c2,
					(i2c_addr << 1), buffer, write_len, 1000);
			if (status == HAL_OK) {
				s_ret_buf_size = 0;
				s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
				chain_command_complete_return(CHAIN_I2C_WRITE, s_ret_buf,
						s_ret_buf_size);
			} else {
				if (status == HAL_ERROR) {
					clear_i2c_flag();
				}
				s_ret_buf_size = 0;
				s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
				chain_command_complete_return(CHAIN_I2C_WRITE, s_ret_buf,
						s_ret_buf_size);
			}
		} else {
			s_ret_buf_size = 0;
			s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
			chain_command_complete_return(CHAIN_I2C_WRITE, s_ret_buf,
					s_ret_buf_size);
		}
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_WORK_MODE_MISMATCHED;
		chain_command_complete_return(CHAIN_I2C_WRITE, s_ret_buf,
				s_ret_buf_size);
	}
}

void chain_i2c_mem_read(uint8_t i2c_addr, uint16_t reg_addr, uint8_t reg_len,
		uint8_t read_len) {
	if (check_i2c_mode()) {
		if (i2c_addr <= 127 && (reg_len >= 1 && reg_len <= 2)) {
			HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, (i2c_addr << 1),
					reg_addr, reg_len, s_i2c_buffer, read_len, 1000);
			if (status == HAL_OK) {
				s_ret_buf_size = 0;
				s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
				for (uint8_t i = 0; i < read_len; i++) {
					s_ret_buf[s_ret_buf_size++] = s_i2c_buffer[i];
				}
				chain_command_complete_return(CHAIN_I2C_MEM_READ, s_ret_buf,
						s_ret_buf_size);
			} else {
				if (status == HAL_ERROR) {
					clear_i2c_flag();
				}
				s_ret_buf_size = 0;
				s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
				chain_command_complete_return(CHAIN_I2C_MEM_READ, s_ret_buf,
						s_ret_buf_size);
			}
		} else {
			s_ret_buf_size = 0;
			s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
			chain_command_complete_return(CHAIN_I2C_MEM_READ, s_ret_buf,
					s_ret_buf_size);
		}
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_WORK_MODE_MISMATCHED;
		chain_command_complete_return(CHAIN_I2C_MEM_READ, s_ret_buf,
				s_ret_buf_size);
	}
}
void chain_i2c_mem_write(uint8_t i2c_addr, uint16_t reg_addr, uint8_t reg_len,
		uint8_t write_len, uint8_t *buffer) {
	if (check_i2c_mode()) {
		if (i2c_addr <= 127 && (reg_len >= 1 && reg_len <= 2)) {
			HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2,
					(i2c_addr << 1), reg_addr, reg_len, buffer, write_len,
					1000);
			if (status == HAL_OK) {
				s_ret_buf_size = 0;
				s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
				chain_command_complete_return(CHAIN_I2C_MEM_WRITE, s_ret_buf,
						s_ret_buf_size);
			} else {
				if (status == HAL_ERROR) {
					clear_i2c_flag();
				}
				s_ret_buf_size = 0;
				s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
				chain_command_complete_return(CHAIN_I2C_MEM_WRITE, s_ret_buf,
						s_ret_buf_size);
			}
		} else {
			s_ret_buf_size = 0;
			s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
			chain_command_complete_return(CHAIN_I2C_MEM_WRITE, s_ret_buf,
					s_ret_buf_size);
		}
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_WORK_MODE_MISMATCHED;
		chain_command_complete_return(CHAIN_I2C_MEM_WRITE, s_ret_buf,
				s_ret_buf_size);
	}
}

void chain_get_i2c_device_addr(void) {
	if (check_i2c_mode()) {
		uint8_t i2c_addr_num = 0;
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++];
		s_ret_buf[s_ret_buf_size++];
		for (uint8_t i = 1; i < 128; i++) {
			HAL_StatusTypeDef result = HAL_I2C_IsDeviceReady(&hi2c2,
					(uint16_t) (i << 1), 1, 10);
			if (result == HAL_OK) {
				i2c_addr_num++;
				s_ret_buf[s_ret_buf_size++] = i;
			}
		}
		if(i2c_addr_num == 0){
			clear_i2c_flag();
			s_ret_buf[0] = CHAIN_UART_OPERATION_FAIL;
		}else{
			s_ret_buf[0] = CHAIN_UART_OPERATION_SUCCESS;
		}
		s_ret_buf[1] = i2c_addr_num;
		chain_command_complete_return(CHAIN_I2C_GET_DEVICE_ADDR, s_ret_buf,
				s_ret_buf_size);
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_WORK_MODE_MISMATCHED;
		chain_command_complete_return(CHAIN_I2C_GET_DEVICE_ADDR, s_ret_buf,
				s_ret_buf_size);
	}
}

void GPIO_OUTPUT_Init(gpio_pin_t gpio_pin, gpio_level_t gpio_level,
		gpio_mode_t gpio_mode, gpio_up_mode_t gpio_up_mode,
		gpio_speed_t gpio_speed) {
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; // 输出模式
	// GPIO配置
	switch (gpio_pin) {
	case CHAIN_GPIO_PIN_12:
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
		GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
		break;
	case CHAIN_GPIO_PIN_11:
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
		GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
		break;
	default:
		break;
	}
	// 输出水平配置
	switch (gpio_level) {
	case CHAIN_GPIO_RESET:
		LL_GPIO_ResetOutputPin(GPIOA, GPIO_InitStruct.Pin);
		break;
	case CHAIN_GPIO_SET:
		LL_GPIO_SetOutputPin(GPIOA, GPIO_InitStruct.Pin);
		break;
	default:
		break;
	}
	// 输出模式配置
	switch (gpio_mode) {
	case CHAIN_GPIO_OUTPUT_PUSHPULL:
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		break;
	case CHAIN_GPIO_OUTPUT_OPENDRAIN:
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
		break;
	default:
		break;
	}
	// 上拉下拉配置
	switch (gpio_up_mode) {
	case CHAIN_GPIO_PULL_UP:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
		break;
	case CHAIN_GPIO_PULL_DOWN:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
		break;
	case CHAIN_GPIO_PULL_NO:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		break;
	default:
		break;
	}
	// 输出速度配置
	switch (gpio_speed) {
	case CHAIN_GPIO_SPEED_FREQ_LOW:
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
		break;
	case CHAIN_GPIO_SPEED_FREQ_MEDIUM:
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
		break;
	case CHAIN_GPIO_SPEED_FREQ_HIGH:
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
		break;
	case CHAIN_GPIO_SPEED_FREQ_VERY_HIGH:
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
		break;
	default:
		break;
	}
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void GPIO_OUTPUT_DeInit(uint16_t GPIO_Pin) {
	HAL_GPIO_DeInit(GPIOA, GPIO_Pin);
}

void chian_uart_output_init(uint8_t *buffer, uint16_t size) {
	if (size == 5) {
		if (g_gpio_status[0].mode == CHAIN_I2C_WORK_STATUS
				&& g_gpio_status[1].mode == CHAIN_I2C_WORK_STATUS) {
			__disable_irq();
			HAL_I2C_MspDeInit(&hi2c2);
			g_gpio_status[0].mode = CHAIN_NOT_WORK_STATUS;
			g_gpio_status[1].mode = CHAIN_NOT_WORK_STATUS;
			GPIO_OUTPUT_Init(buffer[0], buffer[1], buffer[2], buffer[3],
					buffer[4]);
			__enable_irq();
		} else if (g_gpio_status[0].mode == CHAIN_ADC_WORK_STATUS
				&& g_gpio_status[1].mode == CHAIN_ADC_WORK_STATUS) {
			__disable_irq();
			HAL_ADC_MspDeInit(&hadc1);
			g_gpio_status[buffer[0] - 1].mode = CHAIN_NOT_WORK_STATUS;
			MX_ADC1_Init();
			MX_ADC1_Init();
			MX_ADC1_Init();
			GPIO_OUTPUT_Init(buffer[0], buffer[1], buffer[2], buffer[3],
					buffer[4]);
			__enable_irq();
		} else {
			__disable_irq();
			GPIO_OUTPUT_Init(buffer[0], buffer[1], buffer[2], buffer[3],
					buffer[4]);
			__enable_irq();
		}
		g_gpio_status[buffer[0] - 1].mode = CHAIN_OUTPUT_WORK_STATUS;
		g_gpio_status[buffer[0] - 1].gpio_level = buffer[1];
		g_gpio_status[buffer[0] - 1].gpio_mode = buffer[2];
		g_gpio_status[buffer[0] - 1].gpio_up_mode = buffer[3];
		g_gpio_status[buffer[0] - 1].gpio_speed = buffer[4];
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		chain_command_complete_return(CHAIN_GPIO_OUTPUT_INIT, s_ret_buf,
				s_ret_buf_size);
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
		chain_command_complete_return(CHAIN_GPIO_OUTPUT_INIT, s_ret_buf,
				s_ret_buf_size);
	}
}

void GPIO_INPUT_Init(gpio_pin_t gpio, gpio_up_mode_t gpio_up_mode) {
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	// GPIO配置
	switch (gpio) {
	case CHAIN_GPIO_PIN_12:
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
		GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
		break;
	case CHAIN_GPIO_PIN_11:
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
		GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
		break;
	default:
		break;
	}
	// 上拉下拉配置
	switch (gpio_up_mode) {
	case CHAIN_GPIO_PULL_UP:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
		break;
	case CHAIN_GPIO_PULL_DOWN:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
		break;
	case CHAIN_GPIO_PULL_NO:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		break;
	default:
		break;
	}
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void GPIO_INPUT_DeInit(uint16_t GPIO_Pin) {
	HAL_GPIO_DeInit(GPIOA, GPIO_Pin);
}

void chain_uart_input_init(uint8_t *buffer, uint16_t size) {
	if (size == 2) {
		if (g_gpio_status[0].mode == CHAIN_I2C_WORK_STATUS
				&& g_gpio_status[1].mode == CHAIN_I2C_WORK_STATUS) {
			__disable_irq();
			HAL_I2C_MspDeInit(&hi2c2);
			g_gpio_status[0].mode = CHAIN_NOT_WORK_STATUS;
			g_gpio_status[1].mode = CHAIN_NOT_WORK_STATUS;
			GPIO_INPUT_Init(buffer[0], buffer[1]);
			__enable_irq();
		} else if (g_gpio_status[0].mode == CHAIN_ADC_WORK_STATUS
				&& g_gpio_status[1].mode == CHAIN_ADC_WORK_STATUS) {
			__disable_irq();
			HAL_ADC_MspDeInit(&hadc1);
			g_gpio_status[buffer[0] - 1].mode = CHAIN_NOT_WORK_STATUS;
			MX_ADC1_Init();
			MX_ADC1_Init();
			MX_ADC1_Init();
			GPIO_INPUT_Init(buffer[0], buffer[1]);
			__enable_irq();
		} else {
			__disable_irq();
			GPIO_INPUT_DeInit(buffer[0]);
			GPIO_INPUT_Init(buffer[0], buffer[1]);
			__enable_irq();
		}
		g_gpio_status[buffer[0] - 1].mode = CHAIN_INPUT_WORK_STATUS;
		g_gpio_status[buffer[0] - 1].gpio_up_mode = buffer[1];
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		chain_command_complete_return(CHAIN_GPIO_INPUT_INIT, s_ret_buf,
				s_ret_buf_size);
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_WORK_MODE_MISMATCHED;
		chain_command_complete_return(CHAIN_GPIO_READ_GPIO_LEVEL, s_ret_buf,
				s_ret_buf_size);
	}
}
bool check_input_mode(gpio_pin_t gpio_pin) {
	if (g_gpio_status[gpio_pin - 1].mode == CHAIN_INPUT_WORK_STATUS) {
		return true;
	}
	return false;
}

void chain_uart_read_pin_level(gpio_pin_t gpio_pin) {
	if (check_input_mode(gpio_pin)) {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		if (gpio_pin == CHAIN_GPIO_PIN_11) {
			s_ret_buf[s_ret_buf_size++] = HAL_GPIO_ReadPin(GPIOA,
			LL_GPIO_PIN_11);
		} else if (gpio_pin == CHAIN_GPIO_PIN_12) {
			s_ret_buf[s_ret_buf_size++] = HAL_GPIO_ReadPin(GPIOA,
			LL_GPIO_PIN_12);
		}
		chain_command_complete_return(CHAIN_GPIO_READ_GPIO_LEVEL, s_ret_buf,
				s_ret_buf_size);
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_WORK_MODE_MISMATCHED;
		chain_command_complete_return(CHAIN_GPIO_INPUT_INIT, s_ret_buf,
				s_ret_buf_size);
	}
}
void GPIO_NVIC_Init(gpio_pin_t gpio, gpio_up_mode_t gpio_up_mode,
		gpio_nvic_trigger_detection_t trigger_detection) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; // 初始化结构体
	// GPIO配置
	switch (gpio) {
	case CHAIN_GPIO_PIN_12:
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
		GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
		break;
	case CHAIN_GPIO_PIN_11:
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
		GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
		break;
	default:
		break;
	}
	// GPIO配置
	switch (trigger_detection) {
	case CHAIN_GPIO_MODE_IT_RISING:
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		break;
	case CHAIN_GPIO_MODE_IT_FALLING:
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		break;
	case CHAIN_GPIO_MODE_IT_RISING_FALLING:
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		break;
	default:
		break;
	}
	// 上拉下拉配置
	switch (gpio_up_mode) {
	case CHAIN_GPIO_PULL_UP:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
		break;
	case CHAIN_GPIO_PULL_DOWN:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
		break;
	case CHAIN_GPIO_PULL_NO:
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		break;
	default:
		break;
	}
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void GPIO_NVIC_DeInit(uint16_t GPIO_Pin) {
	HAL_GPIO_DeInit(GPIOA, GPIO_Pin);
}

// 外部中断回调函数，上升沿触发
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_12) {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_GPIO_PIN_12;
		s_ret_buf[s_ret_buf_size++] = CHAIN_GPIO_MODE_IT_RISING;
		chain_command_complete_return(CHAIN_GPIO_EXTERNAL_NVIC_RETURN,
				s_ret_buf, s_ret_buf_size);
	}
	if (GPIO_Pin == GPIO_PIN_11) {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_GPIO_PIN_11;
		s_ret_buf[s_ret_buf_size++] = CHAIN_GPIO_MODE_IT_RISING;
		chain_command_complete_return(CHAIN_GPIO_EXTERNAL_NVIC_RETURN,
				s_ret_buf, s_ret_buf_size);
	}
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}
// 外部中断回调函数，下降沿触发
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_12) {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_GPIO_PIN_12;
		s_ret_buf[s_ret_buf_size++] = CHAIN_GPIO_MODE_IT_FALLING;
		chain_command_complete_return(CHAIN_GPIO_EXTERNAL_NVIC_RETURN,
				s_ret_buf, s_ret_buf_size);
	}
	if (GPIO_Pin == GPIO_PIN_11) {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_GPIO_PIN_11;
		s_ret_buf[s_ret_buf_size++] = CHAIN_GPIO_MODE_IT_FALLING;
		chain_command_complete_return(CHAIN_GPIO_EXTERNAL_NVIC_RETURN,
				s_ret_buf, s_ret_buf_size);
	}
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}
void EXTI4_15_IRQHandler(void) {
	if (g_gpio_status[0].mode == CHAIN_NVIC_WORK_STATUS) {
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
	}
	if (g_gpio_status[1].mode == CHAIN_NVIC_WORK_STATUS) {
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
	}
}

void chian_uart_nvic_init(uint8_t *buffer, uint16_t size) {
	if (size == 3) {
		if (g_gpio_status[0].mode == CHAIN_I2C_WORK_STATUS
				&& g_gpio_status[1].mode == CHAIN_I2C_WORK_STATUS) {
			__disable_irq();
			HAL_I2C_MspDeInit(&hi2c2);
			g_gpio_status[0].mode = CHAIN_NOT_WORK_STATUS;
			g_gpio_status[1].mode = CHAIN_NOT_WORK_STATUS;
			GPIO_NVIC_Init(buffer[0], buffer[1], buffer[2]);
			__enable_irq();
		} else if (g_gpio_status[0].mode == CHAIN_ADC_WORK_STATUS
				&& g_gpio_status[1].mode == CHAIN_ADC_WORK_STATUS) {
			__disable_irq();
			HAL_ADC_MspDeInit(&hadc1);
			g_gpio_status[buffer[0] - 1].mode = CHAIN_NOT_WORK_STATUS;
			MX_ADC1_Init();
			MX_ADC1_Init();
			MX_ADC1_Init();
			GPIO_NVIC_Init(buffer[0], buffer[1], buffer[2]);
			__enable_irq();
		} else {
			__disable_irq();
			GPIO_NVIC_Init(buffer[0], buffer[1], buffer[2]);
			__enable_irq();
		}

		g_gpio_status[buffer[0] - 1].mode = CHAIN_NVIC_WORK_STATUS;
		g_gpio_status[buffer[0] - 1].gpio_up_mode = buffer[1];
		g_gpio_status[buffer[0] - 1].gpio_nvic_trigger_detection = buffer[2];
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		chain_command_complete_return(CHAIN_GPIO_EXTERNAL_NVIC_INIT, s_ret_buf,
				s_ret_buf_size);
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
		chain_command_complete_return(CHAIN_GPIO_EXTERNAL_NVIC_INIT, s_ret_buf,
				s_ret_buf_size);
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) // DMA采集完成中断服务函数
{
	s_ret_buf_size = 0;
	if (g_gpio_status[0].mode == CHAIN_ADC_WORK_STATUS
			&& g_gpio_status[1].mode == CHAIN_ADC_WORK_STATUS) {
		s_ret_buf[s_ret_buf_size++] = 2;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		s_ret_buf[s_ret_buf_size++] = (uint8_t) (s_adc_buffer[0] & 0xFF);
		s_ret_buf[s_ret_buf_size++] = (uint8_t) ((s_adc_buffer[0] >> 8) & 0xFF);
		s_ret_buf[s_ret_buf_size++] = (uint8_t) (s_adc_buffer[1] & 0xFF);
		s_ret_buf[s_ret_buf_size++] = (uint8_t) ((s_adc_buffer[1] >> 8) & 0xFF);
	} else if (g_gpio_status[0].mode == CHAIN_ADC_WORK_STATUS) {
		s_ret_buf[s_ret_buf_size++] = 1;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		s_ret_buf[s_ret_buf_size++] = (uint8_t) (s_adc_buffer[0] & 0xFF);
		s_ret_buf[s_ret_buf_size++] = (uint8_t) ((s_adc_buffer[0] >> 8) & 0xFF);
	} else if (g_gpio_status[1].mode == CHAIN_ADC_WORK_STATUS) {
		s_ret_buf[s_ret_buf_size++] = 1;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		s_ret_buf[s_ret_buf_size++] = (uint8_t) (s_adc_buffer[0] & 0xFF);
		s_ret_buf[s_ret_buf_size++] = (uint8_t) ((s_adc_buffer[0] >> 8) & 0xFF);
	}
	chain_command_complete_return(CHAIN_GPIO_ADC_READ, s_ret_buf,
			s_ret_buf_size);
}

void chain_uart_adc_init(uint8_t *buffer, uint16_t size) {
	if (size == 2) {
		if (buffer[0] == CHAIN_ADC_CHANNEL_ENABLE) {
			g_gpio_status[0].mode = CHAIN_ADC_WORK_STATUS;
			g_gpio_status[0].adc_channel = CHAIN_ADC_CHANNEL_ENABLE;
		}
		if (buffer[1] == CHAIN_ADC_CHANNEL_ENABLE) {
			g_gpio_status[1].mode = CHAIN_ADC_WORK_STATUS;
			g_gpio_status[1].adc_channel = CHAIN_ADC_CHANNEL_ENABLE;
		}
		MX_ADC1_Init();
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_SUCCESS;
		chain_command_complete_return(CHAIN_GPIO_ADC_INIT, s_ret_buf,
				s_ret_buf_size);
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_UART_OPERATION_FAIL;
		chain_command_complete_return(CHAIN_GPIO_ADC_INIT, s_ret_buf,
				s_ret_buf_size);
	}
}

bool check_adc_mode(void) {
	if (g_gpio_status[0].mode == CHAIN_ADC_WORK_STATUS
			|| g_gpio_status[1].mode == CHAIN_ADC_WORK_STATUS) {
		return true;
	}
	return false;
}

void chian_uart_adc_read(void) {
	if (check_adc_mode()) {
		if (g_gpio_status[0].mode == CHAIN_ADC_WORK_STATUS
				&& g_gpio_status[1].mode == CHAIN_ADC_WORK_STATUS) {
			s_adc_size = 2;
		} else if (g_gpio_status[0].mode == CHAIN_ADC_WORK_STATUS) {
			s_adc_size = 1;
		} else if (g_gpio_status[1].mode == CHAIN_ADC_WORK_STATUS) {
			s_adc_size = 1;
		}
		HAL_ADCEx_Calibration_Start(&hadc1); // AD校准
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*) s_adc_buffer, s_adc_size);
	} else {
		s_ret_buf_size = 0;
		s_ret_buf[s_ret_buf_size++] = CHAIN_WORK_MODE_MISMATCHED;
		chain_command_complete_return(CHAIN_GPIO_ADC_INIT, s_ret_buf,
				s_ret_buf_size);
	}
}

void chain_get_gpio_work_status(void) {
	s_ret_buf_size = 0;
	s_ret_buf[s_ret_buf_size++] = g_gpio_status[0].mode;
	s_ret_buf[s_ret_buf_size++] = g_gpio_status[1].mode;
	chain_command_complete_return(CHAIN_GET_WORK_STATION, s_ret_buf,
			s_ret_buf_size);
}
