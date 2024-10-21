#include "tim.h"
#include "RGB.h"
#include <stdlib.h>

static uint8_t s_led_num = 0;
static uint32_t *s_color_buf = NULL;
static uint8_t s_ret_buf[20] = {0};
static uint8_t s_ret_buf_size = 0;
static uint8_t s_rgb_color[3] = {0};

#define gpio_low() GPIOA->BRR = GPIO_PIN_8
#define gpio_high() GPIOA->BSRR = GPIO_PIN_8
#define delay_150ns() \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP")
#define delay_300ns() \
	delay_150ns();    \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP");     \
	__asm("NOP")
#define delay_600ns() \
	delay_300ns();    \
	delay_300ns();    \
	__asm("NOP");     \
	__asm("NOP")
#define delay_900ns() \
	delay_600ns();    \
	delay_300ns()

#define out_bit_low() \
	gpio_high();      \
	delay_300ns();    \
	gpio_low();       \
	delay_900ns()

#define out_bit_high() \
	gpio_high();       \
	delay_600ns();     \
	gpio_low();        \
	delay_600ns()

#define restart()                         \
	do                                    \
	{                                     \
		for (uint8_t i = 0; i < 133; i++) \
		{                                 \
			delay_600ns();                \
		}                                 \
	} while (0)

/**
 * @brief Initialize GPIO settings for the LED control.
 * @note This function configures GPIO pin PA8 as a push-pull output 
 *       without internal pull-up or pull-down resistors and sets the 
 *       output speed to medium frequency. 
 * 
 * @param None
 * @retval None
 */
void GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0}; // Structure to hold GPIO initialization parameters
	GPIO_InitStruct.Pin = GPIO_PIN_8; // Specify the GPIO pin to configure
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Set pin mode to output push-pull
	GPIO_InitStruct.Pull = GPIO_NOPULL; // No internal pull-up or pull-down
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; // Set speed to medium frequency
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // Initialize GPIO with the specified settings
}

/**
 * @brief Send RGB color data to the LED.
 * @note This function sends 24 bits of color data to the LED, one bit at 
 *       a time. It uses bit manipulation to determine whether to set the 
 *       output high or low based on the color value.
 * 
 * @param color 32-bit integer representing the RGB color value
 * @retval None
 */
void rgb_send_data(uint32_t color)
{
	for (uint8_t i = 0; i < 24; i++) // Loop through each bit of the 24-bit color value
	{
		if (color & (1 << (23 - i))) // Check if the current bit is set
		{
			out_bit_high(); // Set the output high if the bit is 1
		}
		else
		{
			out_bit_low(); // Set the output low if the bit is 0
		}
	}
}

/**
 * @brief Set the color value in the color buffer.
 * @note This function updates the specified index in the color buffer 
 *       with the new color value. It allows for setting multiple colors 
 *       in the buffer, which can later be used for LED display.
 * 
 * @param num Index in the color buffer to set the color
 * @param color 32-bit integer representing the RGB color value to set
 * @retval None
 */
void set_color(uint8_t num, uint32_t color)
{
	s_color_buf[num] = color; // Update the color buffer at the specified index
}


/**
 * @brief Display the current RGB values on the LED.
 * @note This function disables interrupts, sends the current RGB color data
 *       to the LEDs, and then re-enables interrupts. It also calls the 
 *       `restart` function to refresh the LED display.
 * 
 * @param None
 * @retval None
 */
void rgb_show(void)
{
	__disable_irq(); // Disable interrupts for safe operation
	for (uint8_t i = 0; i < s_led_num; i++)
	{
		rgb_send_data(s_color_buf[i]); // Send RGB data to each LED
	}
	__enable_irq(); // Re-enable interrupts after sending data
	restart(); // Refresh the LED display
}

/**
 * @brief Convert RGB color values to a format suitable for transmission.
 * @note This function calculates the RGB values based on the current 
 *       brightness level (`g_light`) and returns a combined 32-bit integer 
 *       representing the RGB color.
 * 
 * @param None
 * @retval uint32_t Combined RGB value as a single 32-bit integer
 */
uint32_t rgb_value_convert(void)
{
	uint8_t buf[3] = {0}; // Buffer to hold adjusted RGB values
	for (uint8_t i = 0; i < 3; i++)
	{
		buf[i] = s_rgb_color[i] * (g_light / 100.0); // Adjust RGB values based on brightness
	}

	return (uint32_t)buf[2] | ((uint32_t)buf[0] << 8) | ((uint32_t)buf[1] << 16); // Combine RGB values
}

/**
 * @brief Initialize RGB settings and allocate resources.
 * @note This function initializes the GPIO settings, restarts the LED display, 
 *       allocates memory for the color buffer, and sets the number of LEDs.
 * 
 * @param None
 * @retval None
 */
void rgb_init(void)
{
	GPIO_init(); // Initialize GPIO settings
	restart(); // Restart the LED display
	s_color_buf = (uint32_t *)calloc(RGB_NUM, sizeof(uint32_t)); // Allocate memory for color buffer
	s_led_num = RGB_NUM; // Set the number of LEDs
}

/**
 * @brief Set the RGB color values based on input buffer.
 * @note This function checks the size of the input buffer and updates the 
 *       RGB values if the size is valid. It also converts the new RGB values 
 *       for display and sends a success or failure response.
 * 
 * @param buffer Pointer to the input buffer containing RGB values
 * @param size Length of the input RGB data
 * @retval None
 */
void chain_set_rgb_value(uint8_t *buffer, uint16_t size)
{
	if (size == 3) // Check if the size of the buffer is 3
	{
		s_rgb_color[0] = buffer[0]; // Set red value
		s_rgb_color[1] = buffer[1]; // Set green value
		s_rgb_color[2] = buffer[2]; // Set blue value
		set_color(0, rgb_value_convert()); // Set the color based on converted RGB values
		rgb_show(); // Display the updated RGB values
		s_ret_buf_size = 0; // Reset response buffer size
		s_ret_buf[s_ret_buf_size++] = RGB_OPERATION_SUCCESS; // Indicate success
		chain_command_complete_return(CHAIN_SET_RGB_VALUE, s_ret_buf,
									  s_ret_buf_size); // Return success response
	}
	else
	{
		s_ret_buf_size = 0; // Reset response buffer size for failure
		s_ret_buf[s_ret_buf_size++] = RGB_OPERATION_FAIL; // Indicate failure
		chain_command_complete_return(CHAIN_SET_RGB_VALUE, s_ret_buf,
									  s_ret_buf_size); // Return failure response
	}
}


/**
 * @brief Retrieve the current RGB value.
 * @note This function sends the current RGB color values to the 
 *       command completion handler using the `chain_command_complete_return` function.
 * 
 * @param None
 * @retval None
 */
void chain_get_rgb_value(void)
{
    // Call the command complete function to return the current RGB color
    chain_command_complete_return(CHAIN_GET_RGB_VALUE, s_rgb_color,
                                  sizeof(s_rgb_color));
}

/**
 * @brief Set the brightness level of the RGB light.
 * @note This function sets the global RGB brightness value if it is 
 *       less than or equal to 100, updates the RGB light, and sends 
 *       a success response. If the brightness value is invalid, it sends 
 *       a failure response.
 * 
 * @param value The desired brightness level (0-100).
 * @retval None
 */
void chain_set_light_value(uint8_t value)
{
    // Check if the brightness value is within valid range
    if (g_light <= 100)
    {
        g_light = value; // Set the global brightness value
        set_rgb_light(g_light); // Update the RGB light
        set_color(0, rgb_value_convert()); // Set the RGB color based on the current value
        rgb_show(); // Display the RGB light with updated settings
        s_ret_buf_size = 0; // Reset response buffer size
        s_ret_buf[s_ret_buf_size++] = RGB_OPERATION_SUCCESS; // Indicate success
        // Return the command completion response
        chain_command_complete_return(CHAIN_SET_RGB_LIGHT, s_ret_buf,
                                      s_ret_buf_size);
    }
    else
    {
        s_ret_buf_size = 0; // Reset response buffer size for failure
        s_ret_buf[s_ret_buf_size++] = RGB_OPERATION_FAIL; // Indicate failure
        // Return the command completion response
        chain_command_complete_return(CHAIN_SET_RGB_LIGHT, s_ret_buf,
                                      s_ret_buf_size);
    }
}

/**
 * @brief Retrieve the current RGB light brightness value.
 * @note This function sends the current brightness level of the RGB light 
 *       to the command completion handler using the `chain_command_complete_return` function.
 * 
 * @param None
 * @retval None
 */
void chain_get_light_value(void)
{
    s_ret_buf_size = 0; // Reset response buffer size
    s_ret_buf[s_ret_buf_size++] = g_light; // Add the current brightness value
    // Return the command completion response
    chain_command_complete_return(CHAIN_GET_RGB_LIGHT, s_ret_buf,
                                  s_ret_buf_size);
}

