/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BITBOT_ULTRASONIC_SENSOR_H_
#define BITBOT_ULTRASONIC_SENSOR_H_

#include <stdint.h>

/**
 * @brief      Initializes a peripherals to be able to use the ultrasonic
 *             sensor.
 */
void ultrasonic_sensor_init(void);

/**
 * @brief      Triggers a measurement and measures the delay of the echo
 *             signal. The function blocks for a distance dependent time.
 *             If there's no response from the sensor the functions timeouts
 *             after 25ms.
 *
 * @return     Measured distance in cm or UINT32_MAX if the measurement fails
 */
uint32_t ultrasonic_sensor_get_distance_cm(void);

#endif /* BITBOT_ULTRASONIC_SENSOR_H_ */
