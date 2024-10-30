/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __RGB_H__
#define __RGB_H__

#ifdef __cplusplus

extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "base_function.h"
#include "myflash.h"

#define RGB_NUM (1)  // Number of LEDs

extern __IO uint8_t g_light; // RGB brightness

typedef enum {
	RGB_OPERATION_FAIL = 0x00,  	// Operation failed
	RGB_OPERATION_SUCCESS = 0x01, 	// Operation successful
} rgb_operation_t;					// Operation status

typedef enum {
	CHAIN_SET_RGB_VALUE = 0x20,  	// Set RGB value
	CHAIN_GET_RGB_VALUE = 0x21,  	// Get RGB value
	CHAIN_SET_RGB_LIGHT = 0x22,  	// Set RGB brightness
	CHAIN_GET_RGB_LIGHT = 0x23,  	// Obtain RGB brightness
} chain_rgb_cmd_t; 					// RGB instruction type

void rgb_init(void); 												// Initialize RGB settings
void chain_set_rgb_value(uint8_t *buffer, uint16_t size); 			// Set RGB value from buffer
void chain_get_rgb_value(void); 									// Get current RGB value
void chain_set_light_value(uint8_t value); 							// Set the brightness of the RGB light
void chain_get_light_value(void); 									// Get current RGB light brightness


#ifdef __cplusplus
}
#endif

#endif /* __RGB_H__ */
