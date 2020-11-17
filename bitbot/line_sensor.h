/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BITBOT_LINE_SENSOR_H_
#define BITBOT_LINE_SENSOR_H_

#include <stdbool.h>

typedef enum line_sensor {
	line_sensor_left = 0,
	line_sensor_right
} line_sensor_t;

/**
 * @brief      Initializes the line sensors.
 */
void line_sensor_init(void);

/**
 * @brief      Queries if there's a dark line under the sensor.
 *
 * @param[in]  sensor  The sensor ID
 *
 * @return     True if there's a dark line under the sensor
 */
bool line_sensor_is_line_present(line_sensor_t sensor);

#endif /* BITBOT_LINE_SENSOR_H_ */
