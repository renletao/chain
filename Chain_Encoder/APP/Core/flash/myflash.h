/*
* SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
*
* SPDX-License-Identifier: MIT
*/

#ifndef __MYFLASH_H
#define __MYFLASH_H

#ifdef __cplusplus

extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include "stm32g0xx_hal_flash_ex.h"

#define STM32G0xx_PAGE_SIZE (0x800)                                       // Page size: 2048 bytes (2 KB)
#define STM32G0xx_FLASH_PAGE0_STARTADDR (0x8000000)                       // Start address of flash page 0
#define STM32G0xx_FLASH_PAGE24_STARTADDR (STM32G0xx_FLASH_PAGE0_STARTADDR + 24 * STM32G0xx_PAGE_SIZE) // Start address of flash page 24
#define AB_STATUS_ADDR (STM32G0xx_FLASH_PAGE24_STARTADDR + 0)  // Address for AB status in flash memory
#define RGB_LIGHT_ADDR (STM32G0xx_FLASH_PAGE24_STARTADDR + 1)            // Address for RGB light data
#define BOOTLOADER_VERSION_ADDR (APPLICATION_ADDRESS - 1)                 // Address for bootloader version

bool set_rgb_light(uint8_t data);                                        // Set RGB light with specified data
uint8_t get_rgb_light(void);                                             // Get current RGB light value
uint8_t get_bootloader_version(void);                                    // Get bootloader version
bool set_ab_status(uint8_t data);  // Function to modify the AB status
uint8_t get_ab_status(void);        // Function to retrieve the current AB status

#ifdef __cplusplus
}
#endif

#endif /* __MYFLASH_H */
