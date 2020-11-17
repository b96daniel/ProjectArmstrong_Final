/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "light_sensor.h"
#include "nrf_gpio.h"
#include "microbit/adc.h"
#include "microbit/timer.h"
#include <assert.h>

#define LIGHT_SENSOR_SELECT_PIN     (16)
#define LIGHT_SENSOR_ANALOG_PIN     (2)
#define LIGHT_SENSOR_ANALOG_INPUT   (analog_input_P2)

uint16_t ambient_light = 0;

void light_sensor_init(void) {
	adc_init();
	nrf_gpio_cfg_output(LIGHT_SENSOR_SELECT_PIN);
	nrf_gpio_cfg_input(LIGHT_SENSOR_ANALOG_PIN, NRF_GPIO_PIN_NOPULL);

	timer_wait_ms(2000);
	
	for (int i = 0; i < 5; i++)
	{
		ambient_light += light_sensor_get_value(light_sensor_left);
		ambient_light += light_sensor_get_value(light_sensor_right);
	}
	ambient_light /= 10;
}

int32_t light_sensor_get_value(light_sensor_t sensor) {
	assert(sensor == light_sensor_left || sensor == light_sensor_right);

	nrf_gpio_pin_write(LIGHT_SENSOR_SELECT_PIN, (sensor == light_sensor_right));
	return adc_get_value(LIGHT_SENSOR_ANALOG_INPUT);
}
