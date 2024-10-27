#include "VL53L1X.h"

static ResultBuffer results;     // Buffer to store results from range measurements
static RangingData ranging_data; // Structure to hold ranging data

static const uint32_t TimingGuard = 4528;  // Timing guard value for timing-related operations
static const uint16_t TargetRate = 0x0A00; // Target rate for the sensor configuration

static uint16_t fast_osc_frequency; // Variable to store the frequency of the fast oscillator
static uint16_t osc_calibrate_val;  // Variable to hold the calibration value for the oscillator

/**
 * @brief Read an 8-bit register.
 * @note This function reads a single byte from the specified register.
 *
 * @param reg The register address to read from.
 * @retval The value read from the register.
 */
uint8_t readReg(uint16_t reg)
{
    uint8_t value = 0;               // Variable to store the read value
    uint8_t reg_addr[2];             // Array to hold the register address
    reg_addr[0] = (reg >> 8) & 0xFF; // High byte of the register address
    reg_addr[1] = reg & 0xFF;        // Low byte of the register address

    // Transmit the register address and receive the value
    HAL_I2C_Master_Transmit(&hi2c2, VL53L1X_ADDR << 1, reg_addr, 2, 500);
    HAL_I2C_Master_Receive(&hi2c2, VL53L1X_ADDR << 1, &value, 1, 500);

    return value; // Return the read value
}

/**
 * @brief Read a 16-bit register.
 * @note This function reads two bytes from the specified register and combines them.
 *
 * @param reg The register address to read from.
 * @retval The 16-bit value read from the register.
 */
uint16_t readReg16Bit(uint16_t reg)
{
    uint8_t value[2];                // Array to store the read value (2 bytes)
    uint8_t reg_addr[2];             // Array to hold the register address
    reg_addr[0] = (reg >> 8) & 0xFF; // High byte of the register address
    reg_addr[1] = reg & 0xFF;        // Low byte of the register address

    // Transmit the register address and receive the value
    HAL_I2C_Master_Transmit(&hi2c2, VL53L1X_ADDR << 1, reg_addr, 2, 500);
    HAL_I2C_Master_Receive(&hi2c2, VL53L1X_ADDR << 1, value, 2, 500);

    return (value[0] << 8) | value[1]; // Combine high and low bytes and return
}

/**
 * @brief Write to an 8-bit register.
 * @note This function writes a single byte to the specified register.
 *
 * @param reg The register address to write to.
 * @param value The value to write to the register.
 * @retval True if the operation was successful, false otherwise.
 */
bool writeReg(uint16_t reg, uint8_t value)
{
    uint8_t data[3];             // Array to hold register address and data
    data[0] = (reg >> 8) & 0xFF; // High byte of the register address
    data[1] = reg & 0xFF;        // Low byte of the register address
    data[2] = value;             // Data to write

    // Transmit the register address and data
    return HAL_I2C_Master_Transmit(&hi2c2, VL53L1X_ADDR << 1, data, 3, 500) == HAL_OK;
}

/**
 * @brief Write to a 16-bit register.
 * @note This function writes two bytes to the specified register.
 *
 * @param reg The register address to write to.
 * @param value The 16-bit value to write to the register.
 * @retval True if the operation was successful, false otherwise.
 */
bool writeReg16Bit(uint16_t reg, uint16_t value)
{
    uint8_t data[4];               // Array to hold register address and data
    data[0] = (reg >> 8) & 0xFF;   // High byte of the register address
    data[1] = reg & 0xFF;          // Low byte of the register address
    data[2] = (value >> 8) & 0xFF; // High byte of the value
    data[3] = value & 0xFF;        // Low byte of the value

    // Transmit the register address and data
    return HAL_I2C_Master_Transmit(&hi2c2, VL53L1X_ADDR << 1, data, 4, 500) == HAL_OK;
}

/**
 * @brief Write to a 32-bit register.
 * @note This function writes four bytes to the specified register.
 *
 * @param reg The register address to write to.
 * @param value The 32-bit value to write to the register.
 * @retval True if the operation was successful, false otherwise.
 */
bool writeReg32Bit(uint16_t reg, uint32_t value)
{
    uint8_t data[6];                // Array to hold register address and data
    data[0] = (reg >> 8) & 0xFF;    // High byte of the register address
    data[1] = reg & 0xFF;           // Low byte of the register address
    data[2] = (value >> 24) & 0xFF; // Highest byte of the value
    data[3] = (value >> 16) & 0xFF; // Next highest byte
    data[4] = (value >> 8) & 0xFF;  // Next lowest byte
    data[5] = value & 0xFF;         // Lowest byte of the value

    // Transmit the register address and data
    return HAL_I2C_Master_Transmit(&hi2c2, VL53L1X_ADDR << 1, data, 6, 500) == HAL_OK;
}

/**
 * @brief Set the distance mode for the sensor.
 * @note This function configures the sensor's settings based on the selected mode.
 *
 * @param mode The distance mode to set (0: short range, 1: standard range, 2: long range).
 * @retval True if the mode was set successfully, false if the mode is unrecognized.
 */
bool setDistanceMode(uint8_t mode)
{
    switch (mode)
    {
    case 0:
        // from VL53L1_preset_mode_standard_ranging_short_range()
        // timing config
        writeReg(RANGE_CONFIG__VCSEL_PERIOD_A, 0x07);
        writeReg(RANGE_CONFIG__VCSEL_PERIOD_B, 0x05);
        writeReg(RANGE_CONFIG__VALID_PHASE_HIGH, 0x38);

        // dynamic config
        writeReg(SD_CONFIG__WOI_SD0, 0x07);
        writeReg(SD_CONFIG__WOI_SD1, 0x05);
        writeReg(SD_CONFIG__INITIAL_PHASE_SD0, 6); // tuning parm default
        writeReg(SD_CONFIG__INITIAL_PHASE_SD1, 6); // tuning parm default

        break;

    case 1:
        // from VL53L1_preset_mode_standard_ranging()
        // timing config
        writeReg(RANGE_CONFIG__VCSEL_PERIOD_A, 0x0B);
        writeReg(RANGE_CONFIG__VCSEL_PERIOD_B, 0x09);
        writeReg(RANGE_CONFIG__VALID_PHASE_HIGH, 0x78);

        // dynamic config
        writeReg(SD_CONFIG__WOI_SD0, 0x0B);
        writeReg(SD_CONFIG__WOI_SD1, 0x09);
        writeReg(SD_CONFIG__INITIAL_PHASE_SD0, 10); // tuning parm default
        writeReg(SD_CONFIG__INITIAL_PHASE_SD1, 10); // tuning parm default

        break;

    case 2: // long
        // from VL53L1_preset_mode_standard_ranging_long_range()
        // timing config
        writeReg(RANGE_CONFIG__VCSEL_PERIOD_A, 0x0F);
        writeReg(RANGE_CONFIG__VCSEL_PERIOD_B, 0x0D);
        writeReg(RANGE_CONFIG__VALID_PHASE_HIGH, 0xB8);

        // dynamic config
        writeReg(SD_CONFIG__WOI_SD0, 0x0F);
        writeReg(SD_CONFIG__WOI_SD1, 0x0D);
        writeReg(SD_CONFIG__INITIAL_PHASE_SD0, 14); // tuning parm default
        writeReg(SD_CONFIG__INITIAL_PHASE_SD1, 14); // tuning parm default

        break;

    default:
        // unrecognized mode - do nothing
        return false;
    }

    return true;
}

/**
 * @brief Check if data is ready to be read.
 * @note This function checks the GPIO status to determine if data is ready.
 *
 * @retval True if data is ready, false otherwise.
 */
bool dataReady()
{
    return (readReg(GPIO__TIO_HV_STATUS) & 0x01) == 0;
}

/**
 * @brief Calculate the macro period based on the VCSEL period.
 * @note This function calculates the macro period in microseconds using the VCSEL period.
 *
 * @param vcsel_period The VCSEL period to use for the calculation.
 * @retval The calculated macro period in microseconds.
 */
uint32_t calcMacroPeriod(uint8_t vcsel_period)
{
    // from VL53L1_calc_pll_period_us()
    // fast osc frequency in 4.12 format; PLL period in 0.24 format
    uint32_t pll_period_us = ((uint32_t)0x01 << 30) / fast_osc_frequency;

    // from VL53L1_decode_vcsel_period()
    uint8_t vcsel_period_pclks = (vcsel_period + 1) << 1;

    // VL53L1_MACRO_PERIOD_VCSEL_PERIODS = 2304
    uint32_t macro_period_us = (uint32_t)2304 * pll_period_us;
    macro_period_us >>= 6;
    macro_period_us *= vcsel_period_pclks;
    macro_period_us >>= 6;

    return macro_period_us;
}

/**
 * @brief Encode the timeout value for the register.
 * @note This function encodes a timeout value into the register format.
 *
 * @param timeout_mclks The timeout value in MCLKs.
 * @retval The encoded timeout value in the register format.
 */
uint16_t encodeTimeout(uint32_t timeout_mclks)
{
    // encoded format: "(LSByte * 2^MSByte) + 1"

    uint32_t ls_byte = 0;
    uint16_t ms_byte = 0;

    if (timeout_mclks > 0)
    {
        ls_byte = timeout_mclks - 1;

        while ((ls_byte & 0xFFFFFF00) > 0)
        {
            ls_byte >>= 1;
            ms_byte++;
        }

        return (ms_byte << 8) | (ls_byte & 0xFF);
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Convert timeout in microseconds to MCLKs.
 * @note This function converts a timeout value from microseconds to MCLKs
 *       using the specified macro period.
 *
 * @param timeout_us Timeout value in microseconds.
 * @param macro_period_us The macro period in microseconds.
 * @retval The corresponding timeout value in MCLKs.
 */
uint32_t timeoutMicrosecondsToMclks(uint32_t timeout_us, uint32_t macro_period_us)
{
    return (((uint32_t)timeout_us << 12) + (macro_period_us >> 1)) / macro_period_us;
}

/**
 * @brief Set the measurement timing budget for the sensor.
 * @note This function configures the timing budget, ensuring it is above the guard value.
 *
 * @param budget_us The desired timing budget in microseconds.
 * @retval True if the timing budget was set successfully, false if it was invalid.
 */
bool setMeasurementTimingBudget(uint32_t budget_us)
{
    // assumes PresetMode is LOWPOWER_AUTONOMOUS

    if (budget_us <= TimingGuard)
    {
        return false;
    }

    uint32_t range_config_timeout_us = budget_us -= TimingGuard;
    if (range_config_timeout_us > 1100000)
    {
        return false; // FDA_MAX_TIMING_BUDGET_US * 2
    }

    range_config_timeout_us /= 2;

    // VL53L1_calc_timeout_register_values() begin

    uint32_t macro_period_us;

    // "Update Macro Period for Range A VCSEL Period"
    macro_period_us = calcMacroPeriod(readReg(RANGE_CONFIG__VCSEL_PERIOD_A));

    // "Update Phase timeout - uses Timing A"
    uint32_t phasecal_timeout_mclks = timeoutMicrosecondsToMclks(1000, macro_period_us);
    if (phasecal_timeout_mclks > 0xFF)
    {
        phasecal_timeout_mclks = 0xFF;
    }
    writeReg(PHASECAL_CONFIG__TIMEOUT_MACROP, phasecal_timeout_mclks);

    // "Update MM Timing A timeout"
    writeReg16Bit(MM_CONFIG__TIMEOUT_MACROP_A,
                  encodeTimeout(timeoutMicrosecondsToMclks(1, macro_period_us)));

    // "Update Range Timing A timeout"
    writeReg16Bit(RANGE_CONFIG__TIMEOUT_MACROP_A,
                  encodeTimeout(timeoutMicrosecondsToMclks(range_config_timeout_us, macro_period_us)));

    // "Update Macro Period for Range B VCSEL Period"
    macro_period_us = calcMacroPeriod(readReg(RANGE_CONFIG__VCSEL_PERIOD_B));

    // "Update MM Timing B timeout"
    writeReg16Bit(MM_CONFIG__TIMEOUT_MACROP_B,
                  encodeTimeout(timeoutMicrosecondsToMclks(1, macro_period_us)));

    // "Update Range Timing B timeout"
    writeReg16Bit(RANGE_CONFIG__TIMEOUT_MACROP_B,
                  encodeTimeout(timeoutMicrosecondsToMclks(range_config_timeout_us, macro_period_us)));

    // VL53L1_calc_timeout_register_values() end

    return true;
}

/**
 * @brief Initialize the VL53L1X sensor.
 * @note This function performs the necessary steps to initialize the sensor
 *       and prepare it for measurements.
 *
 * @retval True if initialization was successful, false otherwise.
 */
bool vl53l1xInit(uint8_t mode)
{
    // VL53L1_software_reset() begin
    if (writeReg(SOFT_RESET, 0x00) != true)
    {
        return false;
    }
    HAL_Delay(10);
    if (writeReg(SOFT_RESET, 0x01) != true)
    {
        return false;
    }
    // store oscillator info for later use
    fast_osc_frequency = readReg16Bit(OSC_MEASURED__FAST_OSC__FREQUENCY);
    osc_calibrate_val = readReg16Bit(RESULT__OSC_CALIBRATE_VAL);

    // static config
    if (writeReg16Bit(DSS_CONFIG__TARGET_TOTAL_RATE_MCPS, TargetRate) != true)
    {
        return false;
    }
    if (writeReg(GPIO__TIO_HV_STATUS, 0x02) != true)
    {
        return false;
    }
    if (writeReg(SIGMA_ESTIMATOR__EFFECTIVE_PULSE_WIDTH_NS, 8) != true)
    {
        return false;
    }
    if (writeReg(SIGMA_ESTIMATOR__EFFECTIVE_AMBIENT_WIDTH_NS, 16) != true)
    {
        return false;
    }
    if (writeReg(ALGO__CROSSTALK_COMPENSATION_VALID_HEIGHT_MM, 0x01) != true)
    {
        return false;
    }
    if (writeReg(ALGO__RANGE_IGNORE_VALID_HEIGHT_MM, 0xFF) != true)
    {
        return false;
    }
    if (writeReg(ALGO__RANGE_MIN_CLIP, 0) != true)
    {
        return false;
    }
    if (writeReg(ALGO__CONSISTENCY_CHECK__TOLERANCE, 2) != true)
    {
        return false;
    }

    // general config
    if (writeReg16Bit(SYSTEM__THRESH_RATE_HIGH, 0x0000) != true)
    {
        return false;
    }
    if (writeReg16Bit(SYSTEM__THRESH_RATE_LOW, 0x0000) != true)
    {
        return false;
    }
    if (writeReg(DSS_CONFIG__APERTURE_ATTENUATION, 0x38) != true)
    {
        return false;
    }

    // timing config
    if (writeReg16Bit(RANGE_CONFIG__SIGMA_THRESH, 360) != true)
    {
        return false;
    }
    if (writeReg16Bit(RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS, 192) != true)
    {
        return false;
    }

    // dynamic config
    if (writeReg(SYSTEM__GROUPED_PARAMETER_HOLD_0, 0x01) != true)
    {
        return false;
    }
    if (writeReg(SYSTEM__GROUPED_PARAMETER_HOLD_1, 0x01) != true)
    {
        return false;
    }
    if (writeReg(SD_CONFIG__QUANTIFIER, 2) != true)
    {
        return false;
    }

    // GPH is 0 after reset, but writing GPH0 and GPH1 above seem to set GPH to 1,
    // and things don't seem to work if we don't set GPH back to 0 (which the API does here).
    if (writeReg(SYSTEM__GROUPED_PARAMETER_HOLD, 0x00) != true)
    {
        return false;
    }
    if (writeReg(SYSTEM__SEED_CONFIG, 1) != true)
    {
        return false;
    }

    // from VL53L1_config_low_power_auto_mode
    if (writeReg(SYSTEM__SEQUENCE_CONFIG, 0x8B) != true)
    {
        return false;
    }
    if (writeReg16Bit(DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT, 200 << 8) != true)
    {
        return false;
    }
    if (writeReg(DSS_CONFIG__ROI_MODE_CONTROL, 2) != true)
    {
        return false;
    }

    // default to long range, 50 ms timing budget
    setDistanceMode(mode);
    setMeasurementTimingBudget(50000);

    // the API triggers this change in VL53L1_init_and_start_range() once a
    // measurement is started; assumes MM1 and MM2 are disabled
    if (writeReg16Bit(ALGO__PART_TO_PART_RANGE_OFFSET_MM,
                      readReg16Bit(MM_CONFIG__OUTER_OFFSET_MM) * 4) != true)
    {
        return false;
    }

    // from VL53L1_set_inter_measurement_period_ms()
    if (writeReg32Bit(SYSTEM__INTERMEASUREMENT_PERIOD, 50 * osc_calibrate_val) != true)
    {
        return false;
    }
    if (writeReg(SYSTEM__INTERRUPT_CLEAR, 0x01) != true)
    {
        return false;
    }
    if (writeReg(SYSTEM__MODE_START, 0x40) != true)
    {
        return false;
    }
    return true;
}

/**
 * @brief Read results from the VL53L1X sensor.
 * @note This function reads measurement results from the sensor and stores them in the results structure.
 */
void readResults()
{
    uint8_t reg_addr[2];
    reg_addr[0] = (RESULT__RANGE_STATUS >> 8) & 0xFF; // reg high byte
    reg_addr[1] = RESULT__RANGE_STATUS & 0xFF;        // reg low byte

    HAL_I2C_Master_Transmit(&hi2c2, VL53L1X_ADDR << 1, reg_addr, 2, 500);

    uint8_t buffer[17];
    HAL_I2C_Master_Receive(&hi2c2, VL53L1X_ADDR << 1, buffer, 17, 500);

    results.range_status = buffer[0];

    // report_status: not used
    //    uint8_t report_status = buffer[1];

    results.stream_count = buffer[2];

    results.dss_actual_effective_spads_sd0 = (uint16_t)buffer[3] << 8; // high byte
    results.dss_actual_effective_spads_sd0 |= buffer[4];               // low byte

    // peak_signal_count_rate_mcps_sd0: not used
    //    uint8_t peak_signal_count_rate_sd0_high = buffer[5];
    //    uint8_t peak_signal_count_rate_sd0_low = buffer[6];

    results.ambient_count_rate_mcps_sd0 = (uint16_t)buffer[7] << 8; // high byte
    results.ambient_count_rate_mcps_sd0 |= buffer[8];               // low byte

    // sigma_sd0: not used
    //    uint8_t sigma_sd0_high = buffer[9];
    //    uint8_t sigma_sd0_low = buffer[10];

    // phase_sd0: not used
    //    uint8_t phase_sd0_high = buffer[11];
    //    uint8_t phase_sd0_low = buffer[12];

    results.final_crosstalk_corrected_range_mm_sd0 = (uint16_t)buffer[13] << 8; // high byte
    results.final_crosstalk_corrected_range_mm_sd0 |= buffer[14];               // low byte

    results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 = (uint16_t)buffer[15] << 8; // high byte
    results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 |= buffer[16];               // low byte
}

/**
 * @brief Update Dynamic SPAD Selection based on current measurements.
 * @note This function calculates and updates the DSS configuration based on
 *       the effective SPAD count and count rates from the sensor.
 */
void updateDSS()
{
    uint16_t spadCount = results.dss_actual_effective_spads_sd0;

    if (spadCount != 0)
    {
        // "Calc total rate per spad"
        uint32_t totalRatePerSpad =
            (uint32_t)results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 + results.ambient_count_rate_mcps_sd0;

        // "clip to 16 bits"
        if (totalRatePerSpad > 0xFFFF)
        {
            totalRatePerSpad = 0xFFFF;
        }

        // "shift up to take advantage of 32 bits"
        totalRatePerSpad <<= 16;
        totalRatePerSpad /= spadCount;

        if (totalRatePerSpad != 0)
        {
            // "get the target rate and shift up by 16"
            uint32_t requiredSpads = ((uint32_t)TargetRate << 16) / totalRatePerSpad;

            // "clip to 16 bit"
            if (requiredSpads > 0xFFFF)
            {
                requiredSpads = 0xFFFF;
            }

            // "override DSS config"
            writeReg16Bit(DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT, requiredSpads);
            // DSS_CONFIG__ROI_MODE_CONTROL should already be set to REQUESTED_EFFFECTIVE_SPADS

            return;
        }
    }

    // If we reached this point, it means something above would have resulted in a divide by zero.
    // "We want to gracefully set a spad target, not just exit with an error"

    // "set target to mid point"
    writeReg16Bit(DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT, 0x8000);
}

/**
 * @brief Convert count rate from fixed point 9.7 format to float.
 * @note This function takes a 16-bit fixed point count rate and converts it to a floating-point representation.
 *
 * @param count_rate_fixed The fixed point count rate to convert.
 * @retval The converted floating-point count rate.
 */
float countRateFixedToFloat(uint16_t count_rate_fixed)
{
    return (float)count_rate_fixed / (1 << 7);
}

/**
 * @brief Get the ranging data from the VL53L1X sensor.
 * @note This function copies the results from the sensor to the ranging data structure,
 *       applying necessary corrections and setting the range status based on sensor results.
 */
void getRangingData()
{
    // VL53L1_copy_sys_and_core_results_to_range_results() begin

    uint16_t range = results.final_crosstalk_corrected_range_mm_sd0;

    // "apply correction gain"
    // gain factor of 2011 is tuning parm default
    // (VL53L1_TUNINGPARM_LITE_RANGING_GAIN_FACTOR_DEFAULT)
    // Basically, this appears to scale the result by 2011/2048, or about 98%
    // (with the 1024 added for proper rounding).
    ranging_data.range_mm = ((uint32_t)range * 2011 + 0x0400) / 0x0800;

    // VL53L1_copy_sys_and_core_results_to_range_results() end

    // set range_status in ranging_data based on value of RESULT__RANGE_STATUS register
    // mostly based on ConvertStatusLite()
    switch (results.range_status)
    {
    case 17: // MULTCLIPFAIL
    case 2:  // VCSELWATCHDOGTESTFAILURE
    case 1:  // VCSELCONTINUITYTESTFAILURE
    case 3:  // NOVHVVALUEFOUND
        // from SetSimpleData()
        ranging_data.range_status = HardwareFail;
        break;

    case 13: // USERROICLIP
        // from SetSimpleData()
        ranging_data.range_status = MinRangeFail;
        break;

    case 18: // GPHSTREAMCOUNT0READY
        ranging_data.range_status = SynchronizationInt;
        break;

    case 5: // RANGEPHASECHECK
        ranging_data.range_status = OutOfBoundsFail;
        break;

    case 4: // MSRCNOTARGET
        ranging_data.range_status = SignalFail;
        break;

    case 6: // SIGMATHRESHOLDCHECK
        ranging_data.range_status = SigmaFail;
        break;

    case 7: // PHASECONSISTENCY
        ranging_data.range_status = WrapTargetFail;
        break;

    case 12: // RANGEIGNORETHRESHOLD
        ranging_data.range_status = XtalkSignalFail;
        break;

    case 8: // MINCLIP
        ranging_data.range_status = RangeValidMinRangeClipped;
        break;

    case 9: // RANGECOMPLETE
        if (results.stream_count == 0)
        {
            ranging_data.range_status = RangeValidNoWrapCheckFail;
        }
        else
        {
            ranging_data.range_status = RangeValid;
        }
        break;

    default:
        ranging_data.range_status = None;
    }

    // from SetSimpleData()
    ranging_data.peak_signal_count_rate_MCPS = countRateFixedToFloat(
        results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0);
    ranging_data.ambient_count_rate_MCPS = countRateFixedToFloat(
        results.ambient_count_rate_mcps_sd0);
}

/**
 * @brief Get the distance measurement from the VL53L1X sensor.
 * @retval The measured distance in millimeters. Returns -1 if data is not ready.
 */
uint16_t getDistance(void)
{
    if (dataReady())
    {
        readResults();
        updateDSS();
        getRangingData();
        writeReg(SYSTEM__INTERRUPT_CLEAR, 0x01); // sys_interrupt_clear_range
        return ranging_data.range_mm;
    }
    return -1;
}
