/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "i2c.h"
#include "nrf_gpio.h"
#include "nrf_twi.h"
#include "timer.h"

#define I2C_SCL_PIN (0)
#define I2C_SDA_PIN (30)

static NRF_TWI_Type * const twi = NRF_TWI0;
static const uint32_t timeout = 100; /* ms */

static void clear_bus(uint32_t scl_pin, uint32_t sda_pin) {
	uint8_t i = 0;

	nrf_gpio_pin_dir_set(scl_pin, NRF_GPIO_PIN_DIR_INPUT);
	nrf_gpio_pin_dir_set(sda_pin, NRF_GPIO_PIN_DIR_INPUT);

	nrf_gpio_pin_set(scl_pin);
	nrf_gpio_pin_set(sda_pin);

	nrf_gpio_pin_dir_set(scl_pin, NRF_GPIO_PIN_DIR_OUTPUT);
	nrf_gpio_pin_dir_set(sda_pin, NRF_GPIO_PIN_DIR_OUTPUT);

	timer_wait_us(4);

	for(i = 0; i < 9; i++) {
		if (nrf_gpio_pin_read(sda_pin)) {
			if(i == 0) {
				return;
			} else {
				break;
			}
		}
		nrf_gpio_pin_clear(scl_pin);
		timer_wait_us(4);
		nrf_gpio_pin_set(scl_pin);
		timer_wait_us(4);
	}
	nrf_gpio_pin_clear(sda_pin);
	timer_wait_us(4);
}

static int wait_event(nrf_twi_events_t event) {
	uint32_t start_time = timer_get_time_ms();

	while (timer_get_time_ms() - start_time < timeout) {
		if (nrf_twi_event_check(twi, event)) {
			nrf_twi_event_clear(twi, event);
			return 0;
		}
		if (nrf_twi_event_check(twi, NRF_TWI_EVENTS_ERROR)) {
			nrf_twi_event_clear(twi, NRF_TWI_EVENTS_ERROR);
			return -1;
		}
	}

	return -1;
}

void i2c_init(void) {
	timer_init();

	nrf_gpio_cfg(I2C_SCL_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT,
				 NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0D1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_cfg(I2C_SDA_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT,
				 NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0D1, NRF_GPIO_PIN_NOSENSE);

	clear_bus(I2C_SCL_PIN, I2C_SDA_PIN);

	nrf_gpio_pin_dir_set(I2C_SCL_PIN, NRF_GPIO_PIN_DIR_INPUT);
	nrf_gpio_pin_dir_set(I2C_SDA_PIN, NRF_GPIO_PIN_DIR_INPUT);

	nrf_twi_frequency_set(twi, NRF_TWI_FREQ_100K);
	nrf_twi_pins_set(twi, I2C_SCL_PIN, I2C_SDA_PIN);

	nrf_twi_event_clear(twi, NRF_TWI_EVENTS_RXDREADY);
	nrf_twi_event_clear(twi, NRF_TWI_EVENTS_TXDSENT);
	nrf_twi_event_clear(twi, NRF_TWI_EVENTS_ERROR);

	nrf_twi_enable(twi);
}

int i2c_write(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, size_t length) {
	size_t i = 0;

	nrf_twi_address_set(twi, dev_addr);

	nrf_twi_txd_set(twi, reg_addr);
	nrf_twi_task_set(twi, NRF_TWI_TASKS_STARTTX);
	if (wait_event(NRF_TWI_EVENTS_TXDSENT) < 0) {
		return -1;
	}

	for (i = 0; i < length; i++) {
		nrf_twi_txd_set(twi, data[i]);

		if (wait_event(NRF_TWI_EVENTS_TXDSENT) < 0) {
			return -1;
		}
	}

	nrf_twi_task_set(twi, NRF_TWI_TASKS_STOP);
	wait_event(NRF_TWI_EVENTS_STOPPED);

	return length;
}

int i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t length) {
	size_t i = 0;

	nrf_twi_address_set(twi, dev_addr);

	nrf_twi_txd_set(twi, reg_addr);
	nrf_twi_task_set(twi, NRF_TWI_TASKS_STARTTX);
	if (wait_event(NRF_TWI_EVENTS_TXDSENT) < 0) {
		return -1;
	}

	if (length > 1) {
		nrf_twi_shorts_clear(twi, NRF_TWI_SHORTS_BB_STOP_MASK);
		nrf_twi_shorts_set(twi, NRF_TWI_SHORTS_BB_SUSPEND_MASK);
	} else {
		nrf_twi_shorts_set(twi, NRF_TWI_SHORTS_BB_STOP_MASK);
		nrf_twi_shorts_clear(twi, NRF_TWI_SHORTS_BB_SUSPEND_MASK);
	}

	nrf_twi_task_set(twi, NRF_TWI_TASKS_STARTRX);
	for (i = 0; i < length; i++) {
		if (wait_event(NRF_TWI_EVENTS_RXDREADY) < 0) {
			return -1;
		}

		data[i] = nrf_twi_rxd_get(twi);

		/* If the next byte will be the last, reconfigure shorts */
		if (i + 2 == length) {
			nrf_twi_shorts_set(twi, NRF_TWI_SHORTS_BB_STOP_MASK);
		}
		nrf_twi_task_set(twi, NRF_TWI_TASKS_RESUME);
	}

	wait_event(NRF_TWI_EVENTS_STOPPED);
	nrf_twi_shorts_clear(twi, NRF_TWI_SHORTS_BB_STOP_MASK);
	nrf_twi_shorts_clear(twi, NRF_TWI_SHORTS_BB_SUSPEND_MASK);

	return length;
}
