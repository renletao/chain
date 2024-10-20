/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef ENCODER_FUNCTION_H_
#define ENCODER_FUNCTION_H_

#ifdef __cplusplus

extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include "base_function.h"
#include "tim.h"
#include "myflash.h"
#define TIM_ENCODE_DEFAULT (0) // Default timer encoder value

typedef enum {
    ENCODER_AB = 0x00, // AB, increases clockwise
    ENCODER_BA = 0x01, // BA, decreases clockwise
} ab_t; // AB pin status

typedef enum {
    NO_OVERFLOW = 0x00,          // No overflow
    POSITIVE_OVERFLOW = 0x01,   // Positive overflow
    NEGATIVE_OVERFLOW = 0x02,   // Negative overflow
} overflow_t; // Overflow status

typedef enum {
    OPERATION_FAIL = 0x00,      // Operation failed
    OPERATION_SUCCESS = 0x01,   // Operation succeeded
} operation_t; // Operation status

typedef enum {
    CHAIN_GET_ENCODER_VALUE = 0x10,         // Get encoder value
    CHAIN_GET_INC_ENCODER_VALUE = 0x11,     // Get encoder increment value
    CHAIN_GET_BUTTON_VALUE = 0x12,           // Get button state
    CHAIN_RESET_ENCODER_VALUE = 0x13,        // Reset encoder value
    CHAIN_RESET_INC_ENCODER_VALUE = 0x14,    // Reset encoder increment value
    CHAIN_SET_AB_STATUS = 0x15,              // Set AB status, 0->AB, 1->BA
    CHAIN_GET_AB_STATUS = 0x16,              // Get AB status, 0->AB, 1->BA
} chain_encoder_cmd_t; // Command types

// External variable definitions
extern __IO int16_t g_encoder_count;        // Encoder value
extern __IO int16_t g_increment_count;      // Encoder increment value
extern __IO uint8_t g_button_press_state;   // Button press state
extern __IO uint8_t g_encoder_direct;       // AB direction, 0->AB, 1->BA

// Function declarations
void encoder_init(void);                              // Initialize encoder
void encoder_update(void);                            // Update encoder status
void chain_get_encoder_value_handle(void);           // Handle getting encoder value
void chain_get_inc_encoder_value_handle(void);       // Handle getting encoder increment value
void chain_get_button_value_handle(void);            // Handle getting button state
void chain_reset_encoder_value_handle(void);         // Handle resetting encoder value
void chain_reset_inc_encoder_value_handle(void);     // Handle resetting encoder increment value
void chain_set_ab_status_handle(uint8_t status);    // Handle setting AB status, 0->AB, 1->BA
void chain_get_ab_status_handle(void);               // Handle getting AB status, 0->AB, 1->BA


#ifdef __cplusplus
}
#endif

#endif /* ENCODER_FUNCTION_H_ */
