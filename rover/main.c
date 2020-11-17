/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.h"

state_type_t state = BARRIER;
int16_t orientation = 0;

int main(void)
{
	uart_init();
	timer_init();
	i2c_init();
	line_sensor_init();
	motor_init();
	light_sensor_init();
	ultrasonic_sensor_init();
	led_matrix_init();
	compass_init();

	timer_wait_ms(1000);

	while (1)
	{

		switch (state)
		{
		case BARRIER:			
			if (goto_barrier())
			{
				state = START;
			}
			break;

		case START:		   
			if (barrier_lifted())
			{
				state = LAMP;				
			}
			break;

		case LAMP:			
			if (goto_lamp())
			{
				state = MORSE;
			}
			break;

		case MORSE:		
			morse_communication();
			if (morse_state == MORSE_FINISH)
			{
				state = ROUTING;
			}
			break;

		case ROUTING:		
			routing_state_machine();
			break;

		case COMPASS:		
			orientation = get_orientation();			
			state = REVERSE;
			break;

		case REVERSE:		
			if (reverse_bitbot())
			{
				state = HOME;
			}
			break;

		case HOME:
			navigate_home(orientation);
			break;

		case LED_MATRIX:
			led_matrix_update();
			break;

		default:
			break;
		}

		led_matrix_update();
	}
}
