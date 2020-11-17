/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BITBOT_MOTOR_H_
#define BITBOT_MOTOR_H_

#include <stdint.h>

typedef enum motor {
	motor_left = 0,
	motor_right
} motor_t;

/**
 * @brief      Initializes the motor controller peripherals.
 */
void motor_init(void);

/**
 * @brief      Sets the motor speed
 *
 * @param[in]  motor  The motor ID
 * @param[in]  speed  The speed in percent. The sign of the speed chooses the
 *                    direction. Positive values move the robot forward and
 *                    negative values move it backward.
 */
void motor_set_percent(motor_t motor, int8_t speed);

#endif /* BITBOT_MOTOR_H_ */
