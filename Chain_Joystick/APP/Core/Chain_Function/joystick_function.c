#include "joystick_function.h"
#include <stdio.h>

__IO static uint32_t s_last_interrupt_time = 0; // Last interrupt time
__IO static uint16_t s_adc_16value_buf[ADC_CHANNEL_NUMS] = { 0 };
__IO static uint8_t s_adc_8value_buf[ADC_CHANNEL_NUMS] = { 0 };
__IO static uint32_t s_adc_collect_buf[ADC_CHANNEL_NUMS] = { 0 };
__IO static uint16_t s_adc_map_range_value[8] = MAP_RANGE_DEFAULT;
__IO static int16_t s_adc_map_16value[ADC_CHANNEL_NUMS] = { 0 };
__IO static int8_t s_adc_map_8value[ADC_CHANNEL_NUMS] = { 0 };

#ifdef DEBUG
static uint8_t s_ret_buf[256] = { 0 };
#else
static uint8_t s_ret_buf[12] = { 0 };
#endif

static uint8_t s_ret_buf_size = 0;
static adc_cal_t s_adc_cal_value = MAP_RANGE_DEFAULT;

static uint8_t s_key_status = 0; // s_key_status is a static variable to hold
// the current status of the key press

long map(long x, long in_min, long in_max, long out_min, long out_max) {
	long result;
	result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	if (result < out_min)
		result = out_min;
	else if (result > out_max)
		result = out_max;

	return result;
}
void update_adc_cal_value(void) {
	s_adc_cal_value.x_negative_min_adc_16bits = s_adc_map_range_value[0];
	s_adc_cal_value.x_negative_max_adc_16bits = s_adc_map_range_value[1];
	s_adc_cal_value.x_positive_min_adc_16bits = s_adc_map_range_value[2];
	s_adc_cal_value.x_positive_max_adc_16bits = s_adc_map_range_value[3];
	s_adc_cal_value.y_negative_min_adc_16bits = s_adc_map_range_value[4];
	s_adc_cal_value.y_negative_max_adc_16bits = s_adc_map_range_value[5];
	s_adc_cal_value.y_positive_min_adc_16bits = s_adc_map_range_value[6];
	s_adc_cal_value.y_positive_max_adc_16bits = s_adc_map_range_value[7];
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	HAL_ADC_Stop_DMA(hadc);
	// update adc value
	for (uint8_t i = 0; i < ADC_CHANNEL_NUMS; i++) {
		s_adc_16value_buf[i] = s_adc_collect_buf[i];
		s_adc_8value_buf[i] = map(s_adc_16value_buf[i], 0, 65535, 0, 255);
	}
	// map x
	if (s_adc_16value_buf[AXIS_X]
			<= s_adc_cal_value.x_negative_max_adc_16bits) {
		s_adc_map_16value[AXIS_X] = map(s_adc_16value_buf[AXIS_X],
				s_adc_cal_value.x_negative_min_adc_16bits,
				s_adc_cal_value.x_negative_max_adc_16bits, -4095, 0);
		s_adc_map_8value[AXIS_X] = map(s_adc_16value_buf[AXIS_X],
				s_adc_cal_value.x_negative_min_adc_16bits,
				s_adc_cal_value.x_negative_max_adc_16bits, -127, 0);
	} else if (s_adc_16value_buf[AXIS_X]
			>= s_adc_cal_value.x_positive_min_adc_16bits) {
		s_adc_map_16value[AXIS_X] = map(s_adc_16value_buf[AXIS_X],
				s_adc_cal_value.x_positive_min_adc_16bits,
				s_adc_cal_value.x_positive_max_adc_16bits, 0, 4095);
		s_adc_map_8value[AXIS_X] = map(s_adc_16value_buf[AXIS_X],
				s_adc_cal_value.x_positive_min_adc_16bits,
				s_adc_cal_value.x_positive_max_adc_16bits, 0, 127);
	} else {
		s_adc_map_16value[AXIS_X] = 0;
		s_adc_map_8value[AXIS_X] = 0;
	}

	// map y
	if (s_adc_16value_buf[AXIS_Y]
			<= s_adc_cal_value.y_negative_max_adc_16bits) {
		s_adc_map_16value[AXIS_Y] = map(s_adc_16value_buf[AXIS_Y],
				s_adc_cal_value.y_negative_min_adc_16bits,
				s_adc_cal_value.y_negative_max_adc_16bits, -4095, 0);
		s_adc_map_8value[AXIS_Y] = map(s_adc_16value_buf[AXIS_Y],
				s_adc_cal_value.y_negative_min_adc_16bits,
				s_adc_cal_value.y_negative_max_adc_16bits, -127, 0);
	} else if (s_adc_16value_buf[AXIS_Y]
			>= s_adc_cal_value.y_positive_min_adc_16bits) {
		s_adc_map_16value[AXIS_Y] = map(s_adc_16value_buf[AXIS_Y],
				s_adc_cal_value.y_positive_min_adc_16bits,
				s_adc_cal_value.y_positive_max_adc_16bits, 0, 4095);
		s_adc_map_8value[AXIS_Y] = map(s_adc_16value_buf[AXIS_Y],
				s_adc_cal_value.y_positive_min_adc_16bits,
				s_adc_cal_value.y_positive_max_adc_16bits, 0, 127);
	} else {
		s_adc_map_16value[AXIS_Y] = 0;
		s_adc_map_8value[AXIS_Y] = 0;
	}
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) s_adc_collect_buf, ADC_CHANNEL_NUMS);
}

void joystick_init(void) {
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) s_adc_collect_buf, ADC_CHANNEL_NUMS);
}

void chain_joystick_get_16adc(void) {
#ifdef DEBUG
	s_ret_buf[0] = '\0';
	sprintf(s_ret_buf, "x_16adc: %d   y_16adc: %d\n", s_adc_16value_buf[0],
			s_adc_16value_buf[1]);
	usart1_transmit_dma((uint8_t*) s_ret_buf, strlen(s_ret_buf));
#else
    chain_command_complete_return(CHAIN_JOYSTICK_GET_16ADC,
                                    (uint8_t *)s_adc_16value_buf, 4);
#endif
}

void chain_joystick_get_8adc(void) {
#ifdef DEBUG
	s_ret_buf[0] = '\0';
	sprintf(s_ret_buf, "x_8adc: %d   y_8adc: %d\n", s_adc_8value_buf[0],
			s_adc_8value_buf[1]);
	usart1_transmit_dma((uint8_t*) s_ret_buf, strlen(s_ret_buf));
#else
    chain_command_complete_return(CHAIN_JOYSTICK_GET_8ADC,
                                   (uint8_t *)s_adc_8value_buf, 2);
#endif
}

void chain_joystick_get_adc_xy_mapped_range(void) {
#ifdef DEBUG
	s_ret_buf[0] = '\0';
	sprintf(s_ret_buf, "mapped_range");
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t temp[10];
		sprintf(temp, "%d ", s_adc_map_range_value[i]);
		strcat(s_ret_buf, temp);  // 将每个元素追加到buffer中
	}
	usart1_transmit_dma((uint8_t*) s_ret_buf, strlen(s_ret_buf));
#else
    chain_command_complete_return(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE,
                                    (uint8_t *)s_adc_map_range_value, 16);
#endif
}

void chain_joystick_set_adc_xy_mapped_range(uint8_t *buffer, uint16_t size) {
#ifdef DEBUG
	s_ret_buf[0] = '\0';
	if(size == 16){
		sprintf(s_ret_buf, "set success\n");
	}else{
		sprintf(s_ret_buf, "set error\n");
	}
	usart1_transmit_dma((uint8_t*) s_ret_buf, strlen(s_ret_buf));
#else
    if (size == 16) {
        for (uint8_t i = 0; i < 16; i += 2) {
          s_adc_map_range_value[i / 2] =
              ((uint16_t)buffer[i + 1] << 8) | (uint16_t)buffer[i];
        }
        update_adc_cal_value();
        s_ret_buf_size = 0; // Reset response buffer size
        s_ret_buf[s_ret_buf_size++] = OPERATION_SUCCESS; // Indicate success
        chain_command_complete_return(CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE,
                                      s_ret_buf,
                                      s_ret_buf_size); // Return success response
      } else {
        s_ret_buf_size = 0; // Reset response buffer size for failure
        s_ret_buf[s_ret_buf_size++] = OPERATION_FAIL; // Indicate failure
        chain_command_complete_return(CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE,
                                      s_ret_buf,
                                      s_ret_buf_size); // Return failure response
      }
#endif
}

void chain_joystick_get_adc_xy_mapped_int16_value(void) {
#ifdef DEBUG
	s_ret_buf[0] = '\0';
	sprintf(s_ret_buf, "x_map_16value: %d   y_map_16value: %d\n",
			s_adc_map_16value[0], s_adc_map_16value[1]);
	usart1_transmit_dma((uint8_t*) s_ret_buf, strlen(s_ret_buf));
#else
    chain_command_complete_return(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE,
                                    (uint8_t *)s_adc_map_16value, 4);
#endif
}

void chain_joystick_get_adc_xy_mapped_int8_value(void) {
#ifdef DEBUG
	s_ret_buf[0] = '\0';
	sprintf(s_ret_buf, "x_map_8value: %d   y_map_8value: %d\n",
			s_adc_map_8value[0], s_adc_map_8value[1]);
	usart1_transmit_dma((uint8_t*) s_ret_buf, strlen(s_ret_buf));
#else
    chain_command_complete_return(CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE,
                                    (uint8_t *)s_adc_map_8value, 2);
#endif
}

void chain_joystick_button_press(void) {
#ifdef DEBUG
	s_ret_buf[0] = '\0';
	sprintf(s_ret_buf, "button press\n");
	usart1_transmit_dma((uint8_t*) s_ret_buf, strlen(s_ret_buf));
#else
    s_key_status =
          0x11; // Sets s_key_status to 0x11, indicating the key press down state
      chain_command_complete_return(
          CHAIN_JOYSTICK_BUTTON_PRESS, &s_key_status,
          1); // Sends the command with the current key status
#endif
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == BTN1_Pin) {
		if(HAL_GetTick() - s_last_interrupt_time > DEBOUNCE_DELAY){
			s_last_interrupt_time = HAL_GetTick();
			chain_joystick_button_press();
		}
	}
}
