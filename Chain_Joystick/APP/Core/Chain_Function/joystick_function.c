#include "joystick_function.h"

__IO static uint16_t s_adc_12value_buf[ADC_CHANNEL_NUMS] = { 0 };
__IO static uint8_t s_adc_8value_buf[ADC_CHANNEL_NUMS] = { 0 };
__IO static uint32_t s_adc_collect_buf[ADC_CHANNEL_NUMS] = { 0 };
static uint8_t s_key_status = 0; // s_key_status is a static variable to hold the current status of the key press

static long map(long x, long in_min, long in_max, long out_min, long out_max) {
  long result;
  result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  if (result < out_min){
	  result = out_min;
  }
  else if (result > out_max){
	  result = out_max;
  }
  return result;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	int16_t tmp = 0;

	HAL_ADC_Stop_DMA(hadc);
	// 更新adc值
	for(uint8_t i =0 ;i<ADC_CHANNEL_NUMS;i++){
		s_adc_12value_buf[i] = s_adc_collect_buf[i];
		s_adc_8value_buf[i] = (s_adc_12value_buf[i],0,65535,0,255);
	}

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) s_adc_collect_buf, ADC_CHANNEL_NUMS);
}

void joystick_init(void) {
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) s_adc_collect_buf, ADC_CHANNEL_NUMS);
}

void chain_joystick_get_12adc(void) {
//	chain_command_complete_return(CHAIN_JOYSTICK_GET_12ADC,
//									  (uint8_t *)s_adc_12value_buf, 4);
	usart1_transmit_dma((uint8_t *)s_adc_12value_buf, 4);
}

void chain_joystick_get_8adc(void) {
	chain_command_complete_return(CHAIN_JOYSTICK_GET_8ADC,
										  (uint8_t *)s_adc_8value_buf, 2);
}

void chain_joystick_get_adc_xy_mapped_range(void) {

}

void chain_joystick_set_adc_xy_mapped_range(uint8_t *buffer, uint16_t size) {

}

void chain_joystick_get_adc_xy_mapped_int12_value(void) {

}

void chain_joystick_get_adc_xy_mapped_int8_value(void) {

}

void chain_joystick_button_press(void) {
	s_key_status = 0x11; // Sets s_key_status to 0x11, indicating the key press down state
	chain_command_complete_return(CHAIN_JOYSTICK_BUTTON_PRESS, &s_key_status,
			1); // Sends the command with the current key status
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == BTN1_Pin) {
		chain_joystick_button_press();
	}
}
