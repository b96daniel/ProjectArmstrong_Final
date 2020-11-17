/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICROBIT_ADC_H_
#define MICROBIT_ADC_H_

#include <stdint.h>

/**
 * Available analog inputs. These inputs are mapped to the following NRF pins
 * which are connected to the given micro:bit edge pins:
 *
 *~~~
 * AIN0: 45 -> BTN_B (cannot be used for analog measurement)
 * AIN1: 46 -> MMA8653FC_INT2 (cannot be used for analog measurement)
 * AIN2: 5 -> P2
 * AIN3: 6 -> P1
 * AIN4: 7 -> P0
 * AIN5: 8 -> P3
 * AIN6: 9 -> P4
 * AIN7: 10 -> P10
 *~~~
 */
typedef enum analog_input {
	analog_input_P2 = 2,
	analog_input_P1 = 3,
	analog_input_P0 = 4,
	analog_input_P3 = 5,
	analog_input_P4 = 6,
	analog_input_P10 = 7
} analog_input_t;

/**
 * @brief      Initializes the ADC peripheral for measurements.
 */
void adc_init(void);

/**
 * @brief      Measures the analog value on an input line.
 *
 * @param[in]  analog_input  The analog input
 *
 * @return     Measured value on the range of 0-1023.
 */
int32_t adc_get_value(analog_input_t analog_input);

#endif /* MICROBIT_ADC_H_ */
