#include "tof_function.h"

__IO uint16_t g_tof_distance = 0; // Global variable to hold the TOF distance measurement
__IO uint8_t g_tof_mode = TOF_LONG; // Global variable to hold the current TOF mode (default is LONG)

// Buffer for debugging; will be used only in DEBUG mode
#if DEBUG
uint8_t strbuf[128] = {0}; // String buffer for debug output
#else
__IO static uint8_t s_operation_status = 0; // Operation status variable for non-debug mode
#endif

/**
 * @brief Get the current TOF distance measurement.
 * @note This function retrieves the TOF distance and sends it back through UART or debug output.
 * 
 * @param None
 * @retval None
 */
void chain_tof_get_distance(void)
{
#if DEBUG
	strbuf[0] = '0'; // Initialize the first character
	sprintf((char *)strbuf, "distance : %d", g_tof_distance); // Format distance message
	usart1_transmit_dma(strbuf, strlen((char *)strbuf)); // Transmit the message via UART
#else
	chain_command_complete_return(CHAIN_TOF_GET_DISTANCE, (uint8_t *)&g_tof_distance, 1); // Return distance via command
#endif
}

/**
 * @brief Set the TOF mode.
 * @note This function changes the mode of the TOF sensor based on the provided parameter.
 *       It initializes the sensor and updates the global mode variable.
 * 
 * @param mode The desired TOF mode (TOF_SHORT, TOF_MEDIUM, TOF_LONG).
 * @retval None
 */
void chain_tof_set_mode(uint8_t mode)
{
#if DEBUG
	if (mode == TOF_SHORT || mode == TOF_MEDIUM || mode == TOF_LONG)
	{
		bool init_status = vl53l1xInit(mode); // Initialize the TOF sensor
		if (init_status)
		{
			g_tof_mode = mode; // Update global TOF mode
			set_tof_mode(mode); // Set the TOF mode
			strbuf[0] = '0'; // Initialize the first character
			sprintf((char *)strbuf, "set success"); // Format success message
			usart1_transmit_dma(strbuf, strlen((char *)strbuf)); // Transmit success message
		}
		else
		{
			strbuf[0] = '0'; // Initialize the first character
			sprintf((char *)strbuf, "set fail"); // Format failure message
			usart1_transmit_dma(strbuf, strlen((char *)strbuf)); // Transmit failure message
		}
	}
	else
	{
		strbuf[0] = '0'; // Initialize the first character
		sprintf((char *)strbuf, "set fail"); // Format failure message for invalid mode
		usart1_transmit_dma(strbuf, strlen((char *)strbuf)); // Transmit failure message
	}
#else
	if (mode == TOF_SHORT || mode == TOF_MEDIUM || mode == TOF_LONG)
	{
		bool init_status = vl53l1xInit(mode); // Initialize the TOF sensor
		if (init_status)
		{
			g_tof_mode = mode; // Update global TOF mode
			set_tof_mode(mode); // Set the TOF mode
			s_operation_status = TOF_OPERATION_SUCCESS; // Set operation status to success
			chain_command_complete_return(CHAIN_TOF_SET_MODE, (uint8_t *)&s_operation_status, 1); // Return success status
		}
		else
		{
			s_operation_status = TOF_OPERATION_FAIL; // Set operation status to failure
			chain_command_complete_return(CHAIN_TOF_SET_MODE, (uint8_t *)&s_operation_status, 1); // Return failure status
		}
	}
	else
	{
		s_operation_status = TOF_OPERATION_FAIL; // Set operation status to failure for invalid mode
		chain_command_complete_return(CHAIN_TOF_SET_MODE, (uint8_t *)&s_operation_status, 1); // Return failure status
	}
#endif
}

/**
 * @brief Get the current TOF mode.
 * @note This function retrieves the current TOF mode and sends it back through UART or debug output.
 * 
 * @param None
 * @retval None
 */
void chain_tof_get_mode(void)
{
#if DEBUG
	strbuf[0] = '0'; // Initialize the first character
	sprintf((char *)strbuf, "mode : %d", g_tof_mode); // Format mode message
	usart1_transmit_dma(strbuf, strlen((char *)strbuf)); // Transmit the message via UART
#else
	chain_command_complete_return(CHAIN_TOF_GET_MODE, (uint8_t *)&g_tof_mode, 1); // Return current mode via command
#endif
}
