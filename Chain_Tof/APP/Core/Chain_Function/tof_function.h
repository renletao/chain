/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TOF_FUNCTION_H_
#define TOF_FUNCTION_H_

#ifdef __cplusplus

extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "base_function.h"
#include "VL53L1X.h"
#include "myflash.h"

#define TOF_BASE_MODE (0x02) // Base mode for the TOF sensor
#define DEBUG (0)			 // Debug mode indicator

	typedef enum
	{
		CHAIN_TOF_GET_DISTANCE = 0x50, // Command to get the distance measurement
		CHAIN_TOF_SET_MODE = 0x51,	   // Command to set the mode of the TOF sensor
		CHAIN_TOF_GET_MODE = 0x52,	   // Command to get the current mode of the TOF sensor
	} chain_tof_cmd_t;

	typedef enum
	{
		TOF_SHORT = 0x00,  // Short range mode
		TOF_MEDIUM = 0x01, // Medium range mode
		TOF_LONG = 0x02,   // Long range mode
	} tof_mode_t;

	typedef enum
	{
		TOF_OPERATION_FAIL = 0x00,	  // Operation failed
		TOF_OPERATION_SUCCESS = 0x01, // Operation successful
	} tof_operation_t;				  // Operation status

	void chain_tof_get_distance(void);	   // Function to initiate and retrieve the distance measurement from the TOF sensor
	void chain_tof_set_mode(uint8_t mode); // Function to set the operation mode of the TOF sensor (e.g., short, medium, long)
	void chain_tof_get_mode(void);		   // Function to get the current operation mode of the TOF sensor

#ifdef __cplusplus
}
#endif

#endif /* TOF_FUNCTION_H_ */
