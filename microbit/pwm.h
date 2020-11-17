/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICROBIT_PWM_H_
#define MICROBIT_PWM_H_

#include <stdint.h>

/**
 * @brief      Initializes timer for PWM mode with the given period.
 *             Currently it uses TIMER2.
 *
 * @param[in]  period_us  The period us
 */
void pwm_init(uint16_t period_us);

/**
 * @brief      Initializes the channel and bonds it to a pin.
 *
 * @param[in]  pwm_channel_index  The PWM channel index (0-2)
 * @param[in]  pin                The pin
 * @param[in]  width_us           The initial pulse width in us
 */
void pwm_channel_init(uint8_t pwm_channel_index, uint32_t pin, uint16_t width_us);

/**
 * @brief      Sets the pulse width for a channel.
 *
 * @param[in]  pwm_channel_index  The PWM channel index (0-2)
 * @param[in]  width_us           The width in us
 */
void pwm_set_pulse_width(uint8_t pwm_channel_index, uint16_t width_us);

#endif /* MICROBIT_PWM_H_ */
