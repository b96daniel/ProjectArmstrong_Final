/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "led_matrix.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include <assert.h>
#include <stdint.h>

#define LED_COUNT (25)

static volatile bool led_states[LED_COUNT] = { false };
static const uint32_t led_col_gpios[] = { 4, 5, 6, 7, 8, 9, 10, 11, 12 };
static const uint32_t led_row_gpios[] = { 13, 14, 15 };
static const int led_col_counts[] = { 9, 7, 9 };
static const int led_index_lut[LED_COUNT] = {
	0, 2, 4, 19, 18, 17, 16, 15, 11,
	14, 10, 12, 1, 3, 23, 21,
	22, 24, 20, 5, 6, 7, 8, 9, 13
};

void led_matrix_init(void) {
	uint8_t i;

	for (i = 0; i < sizeof(led_states) / sizeof(led_states[0]); i++) {
		led_states[i] = false;
	}

	for (i = 0; i < sizeof(led_col_gpios) / sizeof(led_col_gpios[0]); i++) {
		nrf_gpio_cfg_output(led_col_gpios[i]);
	}

	for (i = 0; i < sizeof(led_row_gpios) / sizeof(led_row_gpios[0]); i++) {
		nrf_gpio_cfg_output(led_row_gpios[i]);
	}
}

void led_matrix_set_state(uint8_t x, uint8_t y, bool state) {
	assert(x < 5 && y < 5);

	led_states[y * 5 + x] = state;
}

void led_matrix_update(void) {
	uint8_t row, col;
	uint8_t i = 0;
	uint8_t led_state_index;

	for (row = 0; row < 3; row++) {
		for (col = 0; col < led_col_counts[row]; col++) {
			led_state_index = led_index_lut[i++];
			nrf_gpio_pin_write(led_col_gpios[col], !led_states[led_state_index]);
		}
		nrf_gpio_pin_set(led_row_gpios[row]);
		nrf_delay_ms(1);
		nrf_gpio_pin_clear(led_row_gpios[row]);
		nrf_delay_ms(1);
	}
}

void led_matrix_clear(void){
	for(uint8_t i = 0; i < 5; i++){
		for(uint8_t j = 0; j < 5; i++){
			led_matrix_set_state(i,j,false);
		}
	}
}

void led_matrix_display_symbol(led_matrix_symbol_t symbol){
	//led_matrix_clear();
	switch(symbol){
		case MINUS_SYMBOL:
			for(uint8_t i = 0; i < 5; i++){
				led_matrix_set_state(i,2,true);
			}
			break;
		
		case PLUS_SYMBOL:
			for(uint8_t i = 0; i < 5; i++){
				led_matrix_set_state(2,i,true);
			}
			for(uint8_t i = 0; i < 5; i++){
				led_matrix_set_state(i,2,true);
			}
			break;

		case HEART_SYMBOL:
			led_matrix_set_state(1,0,true);
			led_matrix_set_state(3,0,true);
			led_matrix_set_state(0,1,true);
			led_matrix_set_state(2,1,true);
			led_matrix_set_state(4,1,true);
			led_matrix_set_state(0,2,true);
			led_matrix_set_state(4,2,true);
			led_matrix_set_state(1,3,true);
			led_matrix_set_state(3,3,true);
			led_matrix_set_state(2,4,true);
			break;
	}
}
