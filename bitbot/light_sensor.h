/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BITBOT_LIGHT_SENSOR_H_
#define BITBOT_LIGHT_SENSOR_H_

#include <stdint.h>

typedef enum light_sensor {
	light_sensor_left = 0,
	light_sensor_right
} light_sensor_t;

extern uint16_t ambient_light;
/**
 * @brief      Initializes the light sensors.
 */
void light_sensor_init(void);

/**
 * @brief      Queries the light sensor measurement result.
 *
 * @param[in]  sensor  The sensor ID
 *
 * @return     Measured analog value in the range of 0-1023.
 */
int32_t light_sensor_get_value(light_sensor_t sensor);

#endif /* BITBOT_LIGHT_SENSOR_H_ */
