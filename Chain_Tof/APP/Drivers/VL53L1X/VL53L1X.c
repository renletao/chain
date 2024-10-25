/*
 * VL53L1X.c
 *
 *  Created on: Oct 25, 2024
 *      Author: 15191
 */
#include "VL53L1X.h"

uint16_t fast_osc_frequency;
uint16_t osc_calibrate_val;
void print_debug(uint8_t *buffer, uint16_t size) {
	usart1_transmit_dma(buffer, 2);
}
uint8_t readReg(uint16_t reg) {
	uint8_t value;
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, (VL53L1X_ADDR << 1),
			reg, I2C_MEMADD_SIZE_16BIT, (uint8_t*) &value, 1, 100);
	return value;
}

uint16_t readReg16Bit(uint16_t reg) {
	uint8_t value[2] = { 0 };
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, (VL53L1X_ADDR << 1),
			reg, I2C_MEMADD_SIZE_16BIT, (uint8_t*) value, 2, 100);
	return ((uint16_t) value[0] << 8 | value[1]);
}

bool writeReg(uint16_t reg, uint8_t value) {
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, (VL53L1X_ADDR << 1),
			reg, I2C_MEMADD_SIZE_16BIT, (uint8_t*) &value, 1, 100);
	return status == HAL_OK ? true : false;
}

bool writeReg16Bit(uint16_t reg, uint16_t value) {
	uint8_t send_data[2] = { 0 };
	send_data[0] = (uint8_t) (value >> 8);
	send_data[1] = (uint8_t) (value);
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, (VL53L1X_ADDR << 1),
			reg, I2C_MEMADD_SIZE_16BIT, (uint8_t*) send_data, 2, 100);
	return status == HAL_OK ? true : false;
}
bool writeReg32Bit(uint16_t reg, uint32_t value) {
	uint8_t send_data[4] = { 0 };
	send_data[0] = ((uint8_t) (value >> 24)); // value highest byte
	send_data[1] = ((uint8_t) (value >> 16));
	send_data[2] = ((uint8_t) (value >> 8));
	send_data[3] = ((uint8_t) (value)); // value lowest byte
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, (VL53L1X_ADDR << 1),
			reg, I2C_MEMADD_SIZE_16BIT, (uint8_t*) send_data, 4, 100);
	return status == HAL_OK ? true : false;
}
bool setDistanceMode(uint8_t mode) {
	switch (mode) {
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
bool dataReady() {
	return (readReg(GPIO__TIO_HV_STATUS) & 0x01) == 0;
}
bool vl53l1x_init(void) {
	// VL53L1_software_reset() begin
	if (writeReg(SOFT_RESET, 0x00) != true) {
		return false;
	}
	HAL_Delay(10);
	if (writeReg(SOFT_RESET, 0x01) != true) {
		return false;
	}
	// store oscillator info for later use
	fast_osc_frequency = readReg16Bit(OSC_MEASURED__FAST_OSC__FREQUENCY);
	osc_calibrate_val = readReg16Bit(RESULT__OSC_CALIBRATE_VAL);
	// static config
	// API resets PAD_I2C_HV__EXTSUP_CONFIG here, but maybe we don't want to do
	// that? (seems like it would disable 2V8 mode)
	if (writeReg16Bit(DSS_CONFIG__TARGET_TOTAL_RATE_MCPS, 0x0A00) != true) {
		return false;
	}
	if (writeReg(GPIO__TIO_HV_STATUS, 0x02) != true) {
		return false;
	}
	if (writeReg(SIGMA_ESTIMATOR__EFFECTIVE_PULSE_WIDTH_NS, 8) != true) {
		return false;
	}
	if (writeReg(SIGMA_ESTIMATOR__EFFECTIVE_AMBIENT_WIDTH_NS, 16) != true) {
		return false;
	}
	if (writeReg(ALGO__CROSSTALK_COMPENSATION_VALID_HEIGHT_MM, 0x01) != true) {
		return false;
	}
	if (writeReg(ALGO__RANGE_IGNORE_VALID_HEIGHT_MM, 0xFF) != true) {
		return false;
	}
	if (writeReg(ALGO__RANGE_MIN_CLIP, 0) != true) {
		return false;
	}
	if (writeReg(ALGO__CONSISTENCY_CHECK__TOLERANCE, 2) != true) {
		return false;
	}

	// general config
	if (writeReg16Bit(SYSTEM__THRESH_RATE_HIGH, 0x0000) != true) {
		return false;
	}
	if (writeReg16Bit(SYSTEM__THRESH_RATE_LOW, 0x0000) != true) {
		return false;
	}
	if (writeReg(DSS_CONFIG__APERTURE_ATTENUATION, 0x38) != true) {
		return false;
	}

	// timing config
	// most of these settings will be determined later by distance and timing
	// budget configuration
	if (writeReg16Bit(RANGE_CONFIG__SIGMA_THRESH, 360) != true) {
		return false;
	}
	if (writeReg16Bit(RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS, 192) != true) {
		return false;
	}

	// dynamic config
	if (writeReg(SYSTEM__GROUPED_PARAMETER_HOLD_0, 0x01) != true) {
		return false;
	}
	if (writeReg(SYSTEM__GROUPED_PARAMETER_HOLD_1, 0x01) != true) {
		return false;
	}
	if (writeReg(SD_CONFIG__QUANTIFIER, 2) != true) {
		return false;
	}
	// VL53L1_preset_mode_standard_ranging() end

	// from VL53L1_preset_mode_timed_ranging_*
	// GPH is 0 after reset, but writing GPH0 and GPH1 above seem to set GPH to 1,
	// and things don't seem to work if we don't set GPH back to 0 (which the API
	// does here).
	if (writeReg(SYSTEM__GROUPED_PARAMETER_HOLD, 0x00) != true) {
		return false;
	}
	if (writeReg(SYSTEM__SEED_CONFIG, 1) != true) {
		return false;
	}

	// from VL53L1_config_low_power_auto_mode
	if (writeReg(SYSTEM__SEQUENCE_CONFIG, 0x8B) != true) {
		return false;
	}
	if (writeReg16Bit(DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT,
			200 << 8) != true) {
		return false;
	}
	if (writeReg(DSS_CONFIG__ROI_MODE_CONTROL, 2) != true) {
		return false;
	}
	// VL53L1_set_preset_mode() end

	// default to long range, 50 ms timing budget
	// note that this is different than what the API defaults to
	setDistanceMode(2);
//	setMeasurementTimingBudget(50000);

	// VL53L1_StaticInit() end

	// the API triggers this change in VL53L1_init_and_start_range() once a
	// measurement is started; assumes MM1 and MM2 are disabled
	if (writeReg16Bit(ALGO__PART_TO_PART_RANGE_OFFSET_MM,
			readReg16Bit(MM_CONFIG__OUTER_OFFSET_MM) * 4) != true) {
		return false;
	}

	// from VL53L1_set_inter_measurement_period_ms()
	if (writeReg32Bit(SYSTEM__INTERMEASUREMENT_PERIOD,
			50 * osc_calibrate_val) != true) {
		return false;
	}
	if (writeReg(SYSTEM__INTERRUPT_CLEAR, 0x01) != true) {
		return false;
	}
	if (writeReg(SYSTEM__MODE_START, 0x40) != true) {
		return false;
	}
	return true;
}

uint16_t readResults() {
	uint8_t read_buffer[17] = { 0 };
	HAL_I2C_Mem_Read(&hi2c2, (VL53L1X_ADDR << 1), RESULT__RANGE_STATUS,
	I2C_MEMADD_SIZE_16BIT, (uint8_t*) read_buffer, 17, 50);
	print_debug(read_buffer, 17);
	return (((uint16_t) read_buffer[14] << 8) | read_buffer[15]);
}
uint16_t get_distance(void) {
//	if (dataReady() == true) {
	uint16_t result = readResults();
	writeReg(SYSTEM__INTERRUPT_CLEAR, 0x01); // sys_interrupt_clear_range
	return ((uint32_t) result * 2011 + 0x0400) / 0x0800;
//	}
//	return 0;
}
