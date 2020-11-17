/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "button.h"
#include "nrf_gpio.h"
#include <assert.h>
#include <stdint.h>

#define GPIO_BUTTON_A   (17)
#define GPIO_BUTTON_B   (26)

void button_init(void) {
	/* Button are pulled externally */
	nrf_gpio_cfg_input(GPIO_BUTTON_A, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(GPIO_BUTTON_B, NRF_GPIO_PIN_NOPULL);
}

bool button_get_state(button_t button) {
	uint32_t gpio = 0;

	assert(button == button_a || button == button_b);

	gpio = (button == button_a) ? GPIO_BUTTON_A : GPIO_BUTTON_B;

	return !nrf_gpio_pin_read(gpio);
}
