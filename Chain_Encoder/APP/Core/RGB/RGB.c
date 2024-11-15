#include "RGB.h"
#include "tim.h"
#include "stdio.h"

const static RGB_Color_TypeDef BLACK = { 0, 0, 0 }; // Black RGB
static RGB_Color_TypeDef color = { 0, 0, 0 }; // Color of the RGB LED
static uint8_t s_rgb_buf[RGB_NUM + 1][24]; // 2D array to store the final PWM output array; each row contains 24 data representing one LED, with the last row as 24 zeros representing the RESET code
static uint8_t s_ret_buf[20] = { 0 }; // Buffer to hold return data
static uint8_t s_ret_buf_size = 0; // Size of the return buffer

/**
 * @brief Activate DMA for TIM1 to control RGB LED timing.
 * @note This function sets up the DMA for data transmission
 *       to control RGB LEDs using TIM1, configuring the necessary
 *       parameters and enabling interrupts.
 *
 * @param None
 * @retval None
 */
static void activate_tim1_dma(void) {
    // Set the DMA data length for RGB_NUM LEDs, each requiring 24 bits.
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, (RGB_NUM + 1) * 24);
    // Set the memory address for the DMA buffer.
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t) s_rgb_buf);
    // Set the peripheral address to the TIM1 capture/compare register.
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t) &(TIM1->CCR1));
    // Clear the global interrupt flag for channel 5.
    LL_DMA_ClearFlag_GI5(DMA1);
    // Clear the transfer complete interrupt flag for channel 5.
    LL_DMA_ClearFlag_TC5(DMA1);
    // Enable the transfer complete interrupt for channel 5.
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
    // Enable DMA requests for TIM1 channel 1.
    LL_TIM_EnableDMAReq_CC1(TIM1);
    // Enable all outputs of TIM1.
    LL_TIM_EnableAllOutputs(TIM1);
    // Set the TIM1 DMA request trigger to CC.
    LL_TIM_CC_SetDMAReqTrigger(TIM1, LL_TIM_CCDMAREQUEST_CC);
    // Enable TIM1 channel 1.
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
}

/**
 * @brief Set the color of a single RGB LED by converting the RGB structure
 *       into binary format (0s and 1s).
 * @note This function adjusts the RGB values based on a global light intensity
 *       setting before converting them to binary format.
 *
 * @param led_id The index of the LED to be set.
 * @param color The color structure containing RGB values.
 * @retval None
 */
static void rgb_setcolor(uint8_t led_id, RGB_Color_TypeDef color) {
    // Adjust RGB values based on global light intensity.
    color.R = color.R * (g_light / 100.0);
    color.G = color.G * (g_light / 100.0);
    color.B = color.B * (g_light / 100.0);

    // Convert the green component to binary and store in the buffer.
    for (uint8_t i = 0; i < 8; i++) {
        s_rgb_buf[led_id][i] = ((color.G & (1 << (7 - i))) ? (CODE_1) : CODE_0);
    }
    // Convert the red component to binary and store in the buffer.
    for (uint8_t i = 8; i < 16; i++) {
        s_rgb_buf[led_id][i] = ((color.R & (1 << (15 - i))) ? (CODE_1) : CODE_0);
    }
    // Convert the blue component to binary and store in the buffer.
    for (uint8_t i = 16; i < 24; i++) {
        s_rgb_buf[led_id][i] = ((color.B & (1 << (23 - i))) ? (CODE_1) : CODE_0);
    }
}

/**
 * @brief Reset the RGB buffer for the next data load.
 * @note This function clears the last RGB LED buffer entry.
 *
 * @param None
 * @retval None
 */
static void reset_load(void) {
    // Clear the RGB buffer for the last LED (RGB_NUM) by setting all bits to 0.
    for (uint8_t i = 0; i < 24; i++) {
        s_rgb_buf[RGB_NUM][i] = 0;
    }
}

/**
 * @brief Send an array to control the RGB LED.
 * @note This function configures the DMA for data transmission
 *       and starts the TIM1 counter for the RGB LED control.
 *
 * @param None
 * @retval None
 */
static void rgb_send_array(void) {
    // Set the data length for the DMA transfer (total bits to send)
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, (RGB_NUM + 1) * 24);

    // Enable the DMA channel for transferring the RGB data
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);

    // Start the timer to enable sending data to the RGB LEDs
    LL_TIM_EnableCounter(TIM1);
}

/**
 * @brief Turn off all RGB LEDs.
 * @note This function sets the first LED to black (off), resets
 *       the load, and sends the updated array to turn off the LEDs.
 *
 * @param None
 * @retval None
 */
void turn_off_all_handle(void) {
    // Set the first RGB LED color to black (off)
    rgb_setcolor(0, BLACK);

    // Reset the load for the RGB LED control
    reset_load();

    // Send the updated RGB array to turn off the LEDs
    rgb_send_array();
}

/**
 * @brief Initialize the RGB LED.
 * @note This function activates DMA for TIM1 and turns off
 *       all RGB LEDs to initialize the system properly.
 *
 * @param None
 * @retval None
 */
void rgb_init(void) {
    // Activate DMA settings for TIM1 to prepare for RGB control
    activate_tim1_dma();

    // Turn off all RGB LEDs during initialization
    turn_off_all_handle();

    // Delay for a short period to allow for stabilization
    HAL_Delay(1);
}

/**
 * @brief Set RGB values based on the provided buffer.
 * @param buffer Pointer to an array containing RGB values.
 * @param size The size of the RGB data (should be 3 for R, G, B).
 * @retval None
 */
void chain_set_rgb_value(uint8_t *buffer, uint16_t size) {
    // Check if the provided size is exactly 3 (R, G, B)
    if (size == 3) {
        // Assign the RGB values from the buffer to the color structure
        color.R = buffer[0];
        color.G = buffer[1];
        color.B = buffer[2];

        // Set the RGB color to the first LED
        rgb_setcolor(0, color);

        // Reset the load to prepare for sending new data
        reset_load();

        // Send the updated RGB array to reflect the new color
        rgb_send_array();

       // Prepare the response buffer indicating success
       s_ret_buf_size = 0;
       s_ret_buf[s_ret_buf_size++] = RGB_OPERATION_SUCCESS;

       // Send a command complete response
       chain_command_complete_return(CHAIN_SET_RGB_VALUE, s_ret_buf,
                                     s_ret_buf_size);
    } else {
        // Prepare the response buffer indicating failure due to incorrect size
       s_ret_buf_size = 0;
       s_ret_buf[s_ret_buf_size++] = RGB_OPERATION_FAIL;

       // Send a command complete response for failure
       chain_command_complete_return(CHAIN_SET_RGB_VALUE, s_ret_buf,
                                     s_ret_buf_size);
    }
}

/**
 * @brief  Get the current RGB values
 * @param  None
 * @retval None
 */
void chain_get_rgb_value(void) {
    // Clear the size of the return buffer
    s_ret_buf_size = 0;

    // Store the current RGB values into the return buffer
    s_ret_buf[s_ret_buf_size++] = color.R;  // Get the red component
    s_ret_buf[s_ret_buf_size++] = color.G;  // Get the green component
    s_ret_buf[s_ret_buf_size++] = color.B;  // Get the blue component

    // Send command complete response with current RGB values
    chain_command_complete_return(CHAIN_GET_RGB_VALUE, s_ret_buf, s_ret_buf_size);
}

/**
 * @brief  Set the brightness of the RGB light
 * @param  g_light_value New brightness value, range from 0 to 100
 * @retval None
 */
void chain_set_light_value(uint8_t g_light_value) {
    // Check if the new brightness value is within the valid range (0-100)
    if (g_light_value <= 100) {
        // Update the global brightness value
        g_light = g_light_value;

        // Set the brightness of the RGB light
        set_rgb_light(g_light);

        // Set the current color to the first LED
        rgb_setcolor(0, color);

        // Reset loading status to prepare for sending data
        reset_load();

        // Send RGB data
        rgb_send_array();

        // Clear the return buffer and mark the operation as successful
        s_ret_buf_size = 0;
        s_ret_buf[s_ret_buf_size++] = RGB_OPERATION_SUCCESS;

        // Send command complete response indicating success
        chain_command_complete_return(CHAIN_SET_RGB_LIGHT, s_ret_buf, s_ret_buf_size);
    } else {
        // Clear the return buffer and mark the operation as failed
        s_ret_buf_size = 0;
        s_ret_buf[s_ret_buf_size++] = RGB_OPERATION_FAIL;

        // Send command complete response indicating failure
        chain_command_complete_return(CHAIN_SET_RGB_LIGHT, s_ret_buf, s_ret_buf_size);
    }
}

/**
 * @brief  Get the current brightness of the RGB light
 * @param  None
 * @retval None
 */
void chain_get_light_value(void) {
    // Clear the size of the return buffer
    s_ret_buf_size = 0;

    // Store the current brightness value into the return buffer
    s_ret_buf[s_ret_buf_size++] = g_light;

    // Send command complete response with the current brightness value
    chain_command_complete_return(CHAIN_GET_RGB_LIGHT, s_ret_buf, s_ret_buf_size);
}

