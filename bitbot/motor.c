/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "motor.h"
#include "nrf_gpio.h"
#include "microbit/pwm.h"
#include <assert.h>

#define MOTOR_LEFT_PWM_PIN  (3)
#define MOTOR_LEFT_DIR_PIN  (18)
#define MOTOR_RIGHT_PWM_PIN (2)
#define MOTOR_RIGHT_DIR_PIN (20)

#define MOTOR_PERIOD_US (20000)

void motor_init(void) {
	/* PWM */
	pwm_init(20000);
	pwm_channel_init(motor_left, MOTOR_LEFT_PWM_PIN, 0);
	pwm_channel_init(motor_right, MOTOR_RIGHT_PWM_PIN, 0);

	/* Direction */
	nrf_gpio_cfg_output(MOTOR_LEFT_DIR_PIN);
	nrf_gpio_cfg_output(MOTOR_RIGHT_DIR_PIN);
}

void motor_set_percent(motor_t motor, int8_t speed) {
	uint32_t width_us = 0;
	uint32_t dir_pin = 0;

	assert(motor == motor_left || motor == motor_right);

	/* Setting direction pin according to the sign of the speed */
	dir_pin = (motor == motor_left) ? MOTOR_LEFT_DIR_PIN : MOTOR_RIGHT_DIR_PIN;
	nrf_gpio_pin_write(dir_pin, (speed >= 0) ? 0 : 1);

	if (speed < -100) {
		speed = -100;
	} else if (speed > 100) {
		speed = 100;
	}

	/*
	 * In reverse direction 100% duty cycle means stopped state and 0% means
	 * full speed so it is mapped the following way: -100 -> 0, 0 -> 100
	 */
	if (speed < 0) {
		speed = speed + 100;
	}

	width_us = MOTOR_PERIOD_US * speed / 100;

	pwm_set_pulse_width(motor, width_us);
}
