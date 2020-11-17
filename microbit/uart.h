/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICROBIT_UART_H_
#define MICROBIT_UART_H_

#include <stdint.h>

/**
 * @brief      Initializes UART0 for 9600 baud communication. The connection is
 *             available through USB VCP.
 */
void uart_init(void);

/**
 * @brief      Sends a byte through UART.
 *
 * @param[in]  data  The data byte
 */
void uart_tx_byte(uint8_t data);

/**
 * @brief      Receives a byte from the UART.
 *
 * @return     The received data byte
 */
uint8_t uart_rx_byte(void);

#endif /* MICROBIT_UART_H_ */
