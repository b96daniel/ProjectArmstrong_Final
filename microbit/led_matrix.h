/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICROBIT_LED_MATRIX_H_
#define MICROBIT_LED_MATRIX_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum led_matrix_symbol{
    MINUS_SYMBOL = -1,
    PLUS_SYMBOL = 1,
    HEART_SYMBOL = 2
}led_matrix_symbol_t;

/**
 * @brief      Initializes the LED matrix handler GPIOs of the micro:bit.
 */
void led_matrix_init(void);

/**
 * @brief      Sets the state of the LED at the given coordinate.
 *             (0, 0) is the top left corner.
 *
 * @param[in]  x      X coordinate: 0 - left column, 4 - right column
 * @param[in]  y      Y coordinate: 0 - top row, 4 - bottom row
 * @param[in]  state  The state
 */
void led_matrix_set_state(uint8_t x, uint8_t y, bool state);

/**
 * @brief      Handler function of the LED matrix. Shall be called at least at
               every 20 milliseconds to avoid flickering on the LED matrix.
 */
void led_matrix_update(void);

void led_matrix_display_symbol(led_matrix_symbol_t symbol);

#endif /* MICROBIT_LED_MATRIX_H_ */
