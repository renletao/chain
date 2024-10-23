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
#include "usart.h"

#define MAP_RANGE_DEFAULT { 5000, 33000, 35000, 64500,5000, 31000, 33000, 58500 }
#define DEBOUNCE_DELAY (150)				  // Debounce delay in milliseconds
#define ADC_CHANNEL_NUMS (2)
#define DEBUG (1)
typedef enum {
	CHAIN_JOYSTICK_GET_16ADC = 0x30,
	CHAIN_JOYSTICK_GET_8ADC = 0x31,
	CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE = 0x32,
	CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE = 0x33,
	CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE = 0x34,
	CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE = 0x35,
	CHAIN_JOYSTICK_BUTTON_PRESS = 0x40,
} chain_joystick_cmd_t;

typedef struct {
	uint16_t x_negative_min_adc_16bits;
	uint16_t x_negative_max_adc_16bits;
	uint16_t x_positive_min_adc_16bits;
	uint16_t x_positive_max_adc_16bits;
	uint16_t y_negative_min_adc_16bits;
	uint16_t y_negative_max_adc_16bits;
	uint16_t y_positive_min_adc_16bits;
	uint16_t y_positive_max_adc_16bits;
} adc_cal_t;

typedef enum {
	OPERATION_FAIL = 0x00,  	// Operation failed
	OPERATION_SUCCESS = 0x01, 	// Operation successful
} operation_t;					// Operation status

typedef enum {
  AXIS_X = 0x00,
  AXIS_Y = 0x01,
} axis_t;

void joystick_init(void);
void chain_joystick_get_16adc(void);
void chain_joystick_get_8adc(void);
void chain_joystick_get_adc_xy_mapped_range(void);
void chain_joystick_set_adc_xy_mapped_range(uint8_t *buffer, uint16_t size);
void chain_joystick_get_adc_xy_mapped_int16_value(void);
void chain_joystick_get_adc_xy_mapped_int8_value(void);
void chain_joystick_button_press(void);
#ifdef __cplusplus
}
#endif

#endif /* JOYSTICK_FUNCTION_H_ */
