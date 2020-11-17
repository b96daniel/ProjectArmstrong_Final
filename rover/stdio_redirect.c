/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "microbit/uart.h"
#include <stddef.h>
#include <stdint.h>

/**
 * @brief      Redirects the output to the UART port.
 *
 * @param[in]  fd     Unused, all file descriptors are redirected
 * @param[in]  buf    The buffer
 * @param[in]  count  Buffer size
 *
 * @return     Returns the number of written bytes
 */
int _write (int fd, const void *buf, size_t count) {
	size_t i = 0;

	(void)fd;

	for (i = 0; i < count; i++) {
		uart_tx_byte(((uint8_t *)buf)[i]);
	}

	return count;
}

/**
 * @brief      Redirects the UART port as the input.
 *
 * @param[in]   fd     Unused, all file descriptors are redirected
 * @param[out]  buf    The buffer
 * @param[in]   count  Buffer size
 *
 * @return     Returns the number of received bytes
 */
int _read (int fd, void *buf, size_t count) {
	size_t i = 0;
	char c = '\0';

	(void)fd;

	for (i = 0; i < count; ) {
		c = uart_rx_byte();
		((uint8_t *)buf)[i++] = c;

		/* The implementation expects \r as the line ending. */
		if (c == '\r') {
			/* Stopping on EOL */
			break;
		}

		/* Echo */
		uart_tx_byte(c);
	}

	return i;
}
