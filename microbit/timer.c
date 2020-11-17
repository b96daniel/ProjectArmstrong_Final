/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "timer.h"
#include "nrf_timer.h"
#include <stdbool.h>

static NRF_TIMER_Type * const timer = NRF_TIMER0;
static bool initialized = false;

void timer_init(void) {
	/* Preventing multiple initializations */
	if (initialized) {
		return;
	}

	/* Timer init */
	nrf_timer_mode_set(timer, NRF_TIMER_MODE_TIMER);
	nrf_timer_bit_width_set(timer, NRF_TIMER_BIT_WIDTH_32);
	nrf_timer_frequency_set(timer, NRF_TIMER_FREQ_1MHz);

	/* Setting CC1 as a trigger for reaload */
	nrf_timer_cc_write(timer, NRF_TIMER_CC_CHANNEL1, 0xffffffffU);
	timer->SHORTS = 0;
	nrf_timer_shorts_enable(timer, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK);

	/* Start timer */
	nrf_timer_task_trigger(timer, NRF_TIMER_TASK_START);

	initialized = true;
}

uint32_t timer_get_time_us(void) {
	/* Triggering capture 0 task to get the counter's value */
	nrf_timer_task_trigger(timer, NRF_TIMER_TASK_CAPTURE0);
	return nrf_timer_cc_read(timer, NRF_TIMER_CC_CHANNEL0);
}


uint32_t timer_get_time_ms(void) {
	return timer_get_time_us() / 1000;
}

void timer_wait_us(uint32_t us) {
	uint32_t start_time = timer_get_time_us();

	while (timer_get_time_us() - start_time < us) {
	}
}

void timer_wait_ms(uint32_t ms) {
	timer_wait_us(ms * 1000);
}
