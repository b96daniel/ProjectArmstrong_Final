/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "line_sensor.h"
#include "nrf_gpio.h"
#include <assert.h>

#define LINE_SENSOR_LEFT_PIN    (26)
#define LINE_SENSOR_RIGHT_PIN   (17)

void line_sensor_init(void) {
	nrf_gpio_cfg_input(LINE_SENSOR_LEFT_PIN, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(LINE_SENSOR_RIGHT_PIN, NRF_GPIO_PIN_NOPULL);
}

bool line_sensor_is_line_present(line_sensor_t sensor) {
	uint32_t pin = 0;

	assert(sensor == line_sensor_left || sensor == line_sensor_right);

	pin = (sensor == line_sensor_left) ? LINE_SENSOR_LEFT_PIN : LINE_SENSOR_RIGHT_PIN;

	return nrf_gpio_pin_read(pin);
}
