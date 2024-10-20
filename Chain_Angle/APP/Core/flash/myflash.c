#include "myflash.h"

/**
 * @brief Get the page number based on the address.
 * @note This function calculates the page number by determining how far
 *       the provided address is from the starting address of the first flash page.
 *
 * @param addr The memory address to check.
 * @retval The page number corresponding to the provided address.
 */
static uint32_t get_page(uint32_t addr)
{
    return (addr - STM32G0xx_FLASH_PAGE0_STARTADDR) / STM32G0xx_PAGE_SIZE; // Return the page number
}

/**
 * @brief Modify a specific byte of an 8-byte value.
 * @note This function clears the byte at the specified index in the 64-bit data
 *       and sets it to the new value. The byte index is multiplied by 8 to
 *       calculate the correct bit position.
 *
 * @param data The pointer to the 64-bit data to be modified.
 * @param byte_index The index of the byte to modify (0-7).
 * @param new_value The new byte value to set (0-255).
 * @retval None
 */
static void set_byte_in_uint64(uint64_t *data, uint8_t byte_index, uint8_t new_value)
{
    // Clear the byte at the specified index
    *data &= ~((uint64_t)(0xFF) << (byte_index * 8)); // Clear the byte
    // Set the new byte value
    *data |= (uint64_t)new_value << (byte_index * 8); // Update the byte
}

/**
 * @brief Read a double word (64 bits) from a given memory address.
 * @note This function dereferences the provided address and returns the 64-bit
 *       value stored there. It uses the volatile pointer type to ensure that
 *       the compiler does not optimize out the read operation.
 *
 * @param address The memory address from which to read.
 * @retval The 64-bit value read from the memory address.
 */
static uint64_t my_flash_read_double_word(uint32_t address)
{
    return *((__IO uint64_t *)(address)); // Read and return the 64-bit value
}

/**
 * @brief Erase a specific flash page.
 * @note This function initiates the page erase operation for the specified flash
 *       memory address. It unlocks the flash memory, performs the erase, and
 *       then locks the flash memory again. The function returns true if the
 *       operation is successful; otherwise, it returns false.
 *
 * @param page_address The address of the page to be erased.
 * @retval Success status (true if successful, false otherwise).
 */
static bool my_flash_earse_pages(uint32_t page_address)
{
    uint32_t page_error = 0; // Variable to hold error information if the erase fails
    FLASH_EraseInitTypeDef my_flash; // Structure for flash erase initialization

    my_flash.TypeErase = FLASH_TYPEERASE_PAGES; // Specify that we are erasing pages
    my_flash.Page = get_page(page_address); // Get the page number based on the address
    my_flash.NbPages = 1; // Specify that we want to erase one page

    HAL_FLASH_Unlock(); // Unlock the flash memory for write access
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&my_flash, &page_error); // Perform the erase operation
    HAL_FLASH_Lock(); // Lock the flash memory again

    // Return true if the erase was successful, false otherwise
    return (status == HAL_OK) ? true : false;
}


/**
 * @brief Write a double word to the specified memory address.
 * @note This function unlocks the flash memory, programs a double word at
 *       the given address, and then locks the flash memory again.
 *
 * @param address The memory address to which the data will be written.
 * @param data The 64-bit data to be written.
 * @retval true if the write operation is successful, false otherwise.
 */
static bool my_flash_write_double_word(uint32_t address, uint64_t data)
{
    HAL_FLASH_Unlock(); // Unlock the flash memory for writing
    HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data); // Program the double word
    HAL_FLASH_Lock(); // Lock the flash memory again

    return (status == HAL_OK); // Return success status
}

/**
 * @brief Modify the brightness of the RGB light.
 * @note This function reads the current state of the RGB light data,
 *       modifies the specified byte with the new brightness value,
 *       erases the flash page, and writes the updated data back to flash.
 *
 * @param data The new brightness value to set (0-255).
 * @retval true if the brightness modification is successful, false otherwise.
 */
bool set_rgb_light(uint8_t data)
{
    uint64_t temp = my_flash_read_double_word(STM32G0xx_FLASH_PAGE24_STARTADDR); // Read current RGB light data
    set_byte_in_uint64(&temp, 1, data); // Modify the brightness byte
    my_flash_earse_pages(STM32G0xx_FLASH_PAGE24_STARTADDR); // Erase the flash page

    // Attempt to write the modified data back to flash until successful
    while (!my_flash_write_double_word(STM32G0xx_FLASH_PAGE24_STARTADDR, temp))
    {
    }

    // Verify if the new brightness value has been successfully set
    return (get_rgb_light() == data);
}

/**
 * @brief Get the current RGB brightness value.
 * @note This function reads the current brightness value stored at the
 *       defined RGB light address.
 *
 * @param None
 * @retval The current RGB brightness value.
 */
uint8_t get_rgb_light(void)
{
    return *((__IO uint8_t *)(RGB_LIGHT_ADDR)); // Read and return the current RGB brightness value
}

/**
 * @brief Get the bootloader version number.
 * @note This function reads the bootloader version number stored at the
 *       defined address.
 *
 * @param None
 * @retval The bootloader version number.
 */
uint8_t get_bootloader_version(void)
{
    return *((__IO uint8_t *)(BOOTLOADER_VERSION_ADDR)); // Read and return the bootloader version number
}

/**
 * @brief Modify the clockwise change status.
 * @note This function updates the clockwise status stored in flash memory
 *       with the provided value.
 *
 * @param data The value to modify the clockwise status (0 or 1).
 * @retval true if the modification is successful, false otherwise.
 */
bool set_clockwise_status(uint8_t data) {
    uint64_t temp = my_flash_read_double_word(STM32G0xx_FLASH_PAGE24_STARTADDR); // Read the current double word from flash memory
    set_byte_in_uint64(&temp, 0, data); // Set the specified byte in the double word to the new data
    my_flash_earse_pages(STM32G0xx_FLASH_PAGE24_STARTADDR); // Erase the flash page before writing

    // Continuously attempt to write the modified double word to flash until successful
    while (my_flash_write_double_word(STM32G0xx_FLASH_PAGE24_STARTADDR, temp) != true) {
    }

    uint8_t dat = get_clockwise_status(); // Retrieve the current clockwise status after writing
    if (dat == data) {
        return true; // Return true if the data matches the intended value
    }
    return false; // Return false if the data does not match
}

/**
 * @brief Get the current clockwise change status.
 * @note This function reads the clockwise status stored at the defined address.
 *
 * @param None
 * @retval The current clockwise status value (0 or 1).
 */
uint8_t get_clockwise_status(void) {
    return *((__IO uint8_t*) (CLOCKWISE_STATUS_ADDR)); // Read and return the value from the specified address
}


