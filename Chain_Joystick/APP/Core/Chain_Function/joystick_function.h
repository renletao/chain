/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef JOYSTICK_FUNCTION_H_
#define JOYSTICK_FUNCTION_H_

#ifdef __cplusplus

extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "base_function.h"
#include "myflash.h"

#define ADC_CHANNEL_NUMS (2)

typedef enum {
	CHAIN_JOYSTICK_GET_12ADC = 0x30,
	CHAIN_JOYSTICK_GET_8ADC = 0x31,
	CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE = 0x32,
	CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE = 0x33,
	CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT12_VALUE = 0x34,
	CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE = 0x35,
	CHAIN_JOYSTICK_BUTTON_PRESS = 0x40,
}chain_joystick_cmd_t;

void joystick_init(void);
void chain_joystick_get_12adc(void);
void chain_joystick_get_8adc(void);
void chain_joystick_get_adc_xy_mapped_range(void);
void chain_joystick_set_adc_xy_mapped_range(uint8_t *buffer,uint16_t size);
void chain_joystick_get_adc_xy_mapped_int12_value(void);
void chain_joystick_get_adc_xy_mapped_int8_value(void);
void chain_joystick_button_press(void);
#ifdef __cplusplus
}
#endif

#endif /* JOYSTICK_FUNCTION_H_ */
