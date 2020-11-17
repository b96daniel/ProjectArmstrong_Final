#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "bitbot/line_sensor.h"
#include "bitbot/motor.h"
#include "microbit/timer.h"
#include <stdint.h>

#define K_P	60
#define K_D	0.1

extern int mean_speed;

typedef enum line_state{
	RIGHT = -1,
	NEITHER = 0,
	LEFT = 1,
	BOTH = 2
} line_state_t;

typedef enum reverse_state{
	REVERSE_START,
	REVERSE_BLIND
}reverse_state_t;

line_state_t get_line_state(bool left_sensor_value, bool right_sensor_value);
void line_following(line_state_t line_state);
void set_motor_diff(int diff);
uint8_t reverse_bitbot(void);

#endif