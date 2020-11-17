/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICROBIT_BUTTON_H_
#define MICROBIT_BUTTON_H_

#include <stdbool.h>

typedef enum button {
	button_a = 0,
	button_b
} button_t;

/**
 * @brief      Initializes button GPIOs.
 */
void button_init(void);

/**
 * @brief      Queries the state of the button.
 *
 * @param[in]  button  The button
 *
 * @return     True if it is pushed
 */
bool button_get_state(button_t button);

#endif /* MICROBIT_BUTTON_H_ */
