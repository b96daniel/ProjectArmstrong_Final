/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pwm.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_ppi.h"
#include "nrf_timer.h"
#include <assert.h>

static volatile uint32_t period = 0;
static volatile uint32_t pulse_width[3] = {0, 0, 0};
static NRF_TIMER_Type * const timer = NRF_TIMER2;

void TIMER2_IRQHandler(void) {
	timer->EVENTS_COMPARE[3] = 0;

	timer->CC[0] = pulse_width[0];
	timer->CC[1] = pulse_width[1];
	timer->CC[2] = pulse_width[2];
	timer->CC[3] = period;

	timer->TASKS_START = 1;
}

void pwm_init(uint16_t period_us) {
	/* Stop timer */
	nrf_timer_task_trigger(timer, NRF_TIMER_TASK_STOP);

	/* Enable constant latency mode */
	NRF_POWER->TASKS_CONSTLAT = 1;

	/* PAN-73 workaround */
	*(uint32_t *) 0x4000AC0C = 0;

	/* Init variables */
	pulse_width[0] = pulse_width[1] = pulse_width[2] = 1;
	period = period_us;

	/* Configuring timer: 16 bit, 1MHz */
	nrf_timer_mode_set(timer, NRF_TIMER_MODE_TIMER);
	nrf_timer_bit_width_set(timer, NRF_TIMER_BIT_WIDTH_16);
	nrf_timer_frequency_set(timer, NRF_TIMER_FREQ_1MHz);

	/* Setting compare register defaults */
	nrf_timer_cc_write(timer, NRF_TIMER_CC_CHANNEL0, pulse_width[0]);
	nrf_timer_cc_write(timer, NRF_TIMER_CC_CHANNEL1, pulse_width[1]);
	nrf_timer_cc_write(timer, NRF_TIMER_CC_CHANNEL2, pulse_width[2]);
	nrf_timer_cc_write(timer, NRF_TIMER_CC_CHANNEL3, period_us); // Reset

	/* Enabling only compare 3 clear and stop shortcuts */
	timer->SHORTS = 0;
	nrf_timer_shorts_enable(timer, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK);
	nrf_timer_shorts_enable(timer, NRF_TIMER_SHORT_COMPARE3_STOP_MASK);

	/* Clearing event flags */
	nrf_timer_event_clear(timer, NRF_TIMER_EVENT_COMPARE0);
	nrf_timer_event_clear(timer, NRF_TIMER_EVENT_COMPARE1);
	nrf_timer_event_clear(timer, NRF_TIMER_EVENT_COMPARE2);
	nrf_timer_event_clear(timer, NRF_TIMER_EVENT_COMPARE3);

	/* Reset timer */
	nrf_timer_task_trigger(timer, NRF_TIMER_TASK_CLEAR);

	/* Enable timer interrupt at the end of the period */
	nrf_timer_int_enable(timer, NRF_TIMER_INT_COMPARE3_MASK);
	NVIC_SetPriority(TIMER2_IRQn, 1);
	NVIC_EnableIRQ(TIMER2_IRQn);

	/* PAN-73 workaround */
	*(uint32_t *) 0x4000AC0C = 1;

	/* Starting timer */
	nrf_timer_task_trigger(timer, NRF_TIMER_TASK_START);
}

void pwm_channel_init(uint8_t pwm_channel_index, uint32_t pin, uint16_t width_us) {
	uint8_t ppi_channel;

	assert(pwm_channel_index < 3);

	/* Setting default pulse width */
	pwm_set_pulse_width(pwm_channel_index, width_us);

	/* Setting GPIO mode to output */
	nrf_gpio_cfg_output(pin);

	/* Define toggle task for the pin */
	nrf_gpiote_task_configure(pwm_channel_index, pin, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
	nrf_gpiote_task_enable(pwm_channel_index);

	/* Enable two PPI channels for the two toggle events */
	ppi_channel = pwm_channel_index * 2;
	nrf_ppi_channel_endpoint_setup(ppi_channel,
			(uint32_t) nrf_timer_event_address_get(timer, offsetof(NRF_TIMER_Type, EVENTS_COMPARE[pwm_channel_index])),
			nrf_gpiote_task_addr_get(offsetof(NRF_GPIOTE_Type, TASKS_OUT[pwm_channel_index])));
	nrf_ppi_channel_endpoint_setup(ppi_channel + 1,
			(uint32_t) nrf_timer_event_address_get(timer, NRF_TIMER_EVENT_COMPARE3),
			nrf_gpiote_task_addr_get(offsetof(NRF_GPIOTE_Type, TASKS_OUT[pwm_channel_index])));
	nrf_ppi_channel_enable(ppi_channel);
	nrf_ppi_channel_enable(ppi_channel + 1);
}

void pwm_set_pulse_width(uint8_t pwm_channel_index, uint16_t width_us) {
	assert(pwm_channel_index < 3);

	if (width_us == 0) {
		width_us = 1;
	} else if (width_us >= period) {
		width_us = period - 1;
	}

	pulse_width[pwm_channel_index] = width_us;
}
