/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ultrasonic_sensor.h"
#include "microbit/timer.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define ULTRASONIC_SENSOR_PIN   (21)

void ultrasonic_sensor_init(void) {
	timer_init();
}

uint32_t ultrasonic_sensor_get_distance_cm(void) {
	static const uint32_t cm_divisor = 58; /* 1 / 340m/s * 2 = 58cm/us */
	static const uint32_t max_distance = 400;
	static const uint32_t rise_timeout = 1000; /* Rising edge time: ~470us */
	static const uint32_t timeout = max_distance * cm_divisor;
	uint32_t start = 0;
	uint32_t end = 0;

	nrf_gpio_cfg_output(ULTRASONIC_SENSOR_PIN);

	/* Waiting after setting output to 0 */
	nrf_gpio_pin_clear(ULTRASONIC_SENSOR_PIN);
	nrf_delay_us(2); /* Non-critical precision */

	/* Sending 10us pulse for triggering the sensor */
	nrf_gpio_pin_set(ULTRASONIC_SENSOR_PIN);
	nrf_delay_us(10); /* Non-critical precision */
	nrf_gpio_pin_clear(ULTRASONIC_SENSOR_PIN);

	/*
	 * Setting pin to input mode as the trigger and echo pins are multiplexed
	 * to the same pin.
	 */
	nrf_gpio_cfg_input(ULTRASONIC_SENSOR_PIN, NRF_GPIO_PIN_NOPULL);

	/*
	 * After sending the 10us trigger signal the sensor should start its
	 * response pulse (echo). The pulse width is proportional to the distance.
	 * If the rising edge doesn't arrive within the given amount of time
	 * (rise_timeout) it is most likely because of the bit:bot is turned off or
	 * the batteries are dead.
	 */

	/* Waiting for rising edge */
	start = timer_get_time_us();
	while(!nrf_gpio_pin_read(ULTRASONIC_SENSOR_PIN)) {
		if(timer_get_time_us() - start > rise_timeout) {
			/* Check bit:bot power switch and batteries! */
			return 0xffffffff;
		}
	}

	/* Waiting for falling edge*/
	start = timer_get_time_us();
	while(nrf_gpio_pin_read(ULTRASONIC_SENSOR_PIN)) {
		if(timer_get_time_us() - start > timeout) {
			return 0xffffffff;
		}
	}
	end = timer_get_time_us();

	/* The distance is proportional to the positive pulse width */
	return (end - start) / cm_divisor;
}
