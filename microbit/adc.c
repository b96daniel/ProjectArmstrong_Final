/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "adc.h"
#include "nrf_adc.h"
#include <assert.h>

void adc_init(void) {
	nrf_adc_config_t adc_config = {
		.resolution = NRF_ADC_CONFIG_RES_10BIT,
		.scaling = NRF_ADC_CONFIG_SCALING_INPUT_ONE_THIRD,
		.reference = NRF_ADC_CONFIG_REF_SUPPLY_ONE_THIRD
	};

	nrf_adc_configure(&adc_config);
}

int32_t adc_get_value(analog_input_t analog_input) {
	static const nrf_adc_config_input_t input_enum_map[] = {
		NRF_ADC_CONFIG_INPUT_0,
		NRF_ADC_CONFIG_INPUT_1,
		NRF_ADC_CONFIG_INPUT_2,
		NRF_ADC_CONFIG_INPUT_3,
		NRF_ADC_CONFIG_INPUT_4,
		NRF_ADC_CONFIG_INPUT_5,
		NRF_ADC_CONFIG_INPUT_6,
		NRF_ADC_CONFIG_INPUT_7
	};

	assert(analog_input_P2 <= analog_input && analog_input <= analog_input_P10);

	return nrf_adc_convert_single(input_enum_map[analog_input]);
}
