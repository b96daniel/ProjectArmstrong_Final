#include "motor_control.h"

line_state_t prev_line = NEITHER;
reverse_state_t reverse_state = REVERSE_START;
int control_value = 0;
int prev_control_value = 0;
int mean_speed = 50;
int motor_right_speed, motor_left_speed;

uint32_t reverse_time = 0;

line_state_t get_line_state(bool left_sensor_value, bool right_sensor_value)
{
	line_state_t ret;

	// Neither
	if (!(left_sensor_value || right_sensor_value))
		ret = NEITHER;
	// Both
	else if (left_sensor_value && right_sensor_value)
		ret = BOTH;
	// Left
	else if (left_sensor_value)
		ret = LEFT;
	// Right
	else
		ret = RIGHT;

	return ret;
}

void line_following(line_state_t line)
{
	if (line == BOTH)
	{
		motor_set_percent(motor_left, 0);
		motor_set_percent(motor_right, 0);
	}
	else
	{
		control_value = K_P * line - prev_control_value * K_D;
		if (control_value > 100)
			control_value = 100;
		else if (control_value < -100)
			control_value = -100;
		set_motor_diff(control_value);
		prev_control_value = control_value;
	}
}

void set_motor_diff(int diff)
{
	if (diff > 100)
		diff = 100;
	else if (diff < -100)
		diff = -100;

	motor_left_speed = mean_speed - diff;
	motor_right_speed = mean_speed + diff;

	if (motor_left_speed > 100)
	{
		motor_left_speed = 100;
		motor_right_speed = 100 + 2 * diff;
	}
	else if (motor_left_speed < -100)
	{
		motor_left_speed = -100;
		motor_right_speed = -100 + 2 * diff;
	}
	else if (motor_right_speed > 100)
	{
		motor_right_speed = 100;
		motor_left_speed = 100 - 2 * diff;
	}
	else if (motor_right_speed < -100)
	{
		motor_right_speed = -100;
		motor_left_speed = -100 - 2 * diff;
	}

	motor_set_percent(motor_left, motor_left_speed);
	motor_set_percent(motor_right, motor_right_speed);
}

uint8_t reverse_bitbot(void)
{
	bool local_left = line_sensor_is_line_present(line_sensor_left);
	bool local_right = line_sensor_is_line_present(line_sensor_right);
	line_state_t local_line_state = get_line_state(local_left, local_right);
	if (reverse_state == REVERSE_START)
	{
		motor_set_percent(motor_left, 30);
		motor_set_percent(motor_right, -30);
		timer_wait_ms(200);
		if (local_line_state == NEITHER)
		{
			reverse_time = timer_get_time_ms();
			reverse_state = REVERSE_BLIND;
		}
	}
	else
	{
		motor_set_percent(motor_left, 30);
		motor_set_percent(motor_right, -30);
		timer_wait_ms(50);
		if ( (local_line_state == LEFT) || (local_line_state == RIGHT) )
		{
			reverse_state = REVERSE_START;
			return 1;
		}
	}
	return 0;
}