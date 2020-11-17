/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "uart.h"
#include "nrf51.h"
#include "nrf_gpio.h"
#include "nrf_uart.h"

#define UART_TX_PIN (24)
#define UART_RX_PIN (25)

void uart_init(void) {
	nrf_uart_baudrate_set(NRF_UART0, NRF_UART_BAUDRATE_9600);
	nrf_uart_configure(NRF_UART0, NRF_UART_PARITY_EXCLUDED, NRF_UART_HWFC_DISABLED);
	nrf_uart_txrx_pins_set(NRF_UART0, UART_TX_PIN, UART_RX_PIN);
	nrf_uart_enable(NRF_UART0);

	nrf_gpio_cfg_output(UART_TX_PIN);
	nrf_gpio_cfg_input(UART_RX_PIN, NRF_GPIO_PIN_NOPULL);
}

void uart_tx_byte(uint8_t data) {
	nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_TXDRDY);
	nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTTX);
	nrf_uart_txd_set(NRF_UART0, data);
	while (!nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_TXDRDY)) {
	}
}

uint8_t uart_rx_byte(void) {
	nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_RXDRDY);
	nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTRX);
	while (!nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_RXDRDY)) {
	}
	return nrf_uart_rxd_get(NRF_UART0);
}
