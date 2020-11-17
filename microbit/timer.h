/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICROBIT_TIMER_H_
#define MICROBIT_TIMER_H_

#include <stdint.h>

/**
 * @brief      Initializes and start the system timer.
 */
void timer_init(void);

/**
 * @brief      Queries the current timestamp value
 *
 * @return     Timestamp in microseconds
 */
uint32_t timer_get_time_us(void);

/**
 * @brief      Queries the current timestamp value
 *
 * @return     Timestamp in milliseconds
 */
uint32_t timer_get_time_ms(void);

/**
 * @brief          Waits for the given amount of time.
 *
 * @param[in]  us  Time in microseconds
 */
void timer_wait_us(uint32_t us);

/**
 * @brief          Waits for the given amount of time.
 *
 * @param[in]  ms  Time in milliseconds
 */
void timer_wait_ms(uint32_t ms);

#endif /* MICROBIT_TIMER_H_ */
