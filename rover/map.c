#include "map.h"

bool left = 0;
bool right = 0;
mode_type_t dest_mode = MODE_SINGLE;
line_state_t line = NEITHER;

uint32_t junction_time = 0;
uint8_t ith_junction = 0;

void init_edges(void)
{
    junction_counter = 1;
    junction_types[0] = TURN_LEFT_CODE;
}

void routing_state_machine(void)
{
    left = line_sensor_is_line_present(line_sensor_left);
    right = line_sensor_is_line_present(line_sensor_right);
    line = get_line_state(left, right);

    switch (dest_mode)
    {
    case MODE_SINGLE:
        if (line == BOTH)
        {
            dest_mode = MODE_JUNCTION;
            junction_time = timer_get_time_ms();
            if (ith_junction < JUNCTION_BUFFER_SIZE)
            {
                ith_junction++;
            }
        }
        drive(SPEED, line);
        break;

    case MODE_JUNCTION:
        if (ith_junction <= junction_counter)
        {
            if (line == BOTH)
            {
                drive(SPEED, junction_types[ith_junction - 1]);
            }
            else
            {
                drive(SPEED, line);
            }
        }
        else
        {
            dest_mode = MODE_END;
            break;
        }
        if ((!(line == BOTH)) && ((timer_get_time_ms() - junction_time) > JTIME_THRESH))
        {
            dest_mode = MODE_SINGLE;
        }
        break;

    case MODE_END:
        drive(0, line);
        dest_mode = MODE_SINGLE;
        ith_junction = 0;
        for (uint8_t k = 0; k < junction_counter; k++)
        {
            if (junction_types[k] == TURN_RIGHT_CODE)
            {
                junction_types[k] = TURN_LEFT_CODE;
            }
            else if (junction_types[k] == TURN_LEFT_CODE)
            {
                junction_types[k] = TURN_RIGHT_CODE;
            }
        }
        state = COMPASS;
        break;
    }
}

void navigate_home(int16_t orient)
{
    left = line_sensor_is_line_present(line_sensor_left);
    right = line_sensor_is_line_present(line_sensor_right);
    line = get_line_state(left, right);

    switch (dest_mode)
    {

    case MODE_SINGLE:
        if (line == BOTH)
        {
            dest_mode = MODE_JUNCTION;
            junction_time = timer_get_time_ms();
            if (ith_junction < JUNCTION_BUFFER_SIZE)
            {
                ith_junction++;
            }
        }
        drive(SPEED, line);
        break;

    case MODE_JUNCTION:
        if (ith_junction <= junction_counter)
        {
            if (line == BOTH)
            {
                drive(SPEED, junction_types[junction_counter - ith_junction]); 
            }
            else
            {
                drive(SPEED, line);
            }
        }
        else
        {
            dest_mode = MODE_END;
            break;
        }
        if ((!(line == BOTH)) && ((timer_get_time_ms() - junction_time) > JTIME_THRESH))
        {
            dest_mode = MODE_SINGLE;
        }
        break;

    case MODE_END:
        drive(0, line);

        if (orient > 0)
        {
            led_matrix_display_symbol(PLUS_SYMBOL);
        }
        else
        {
            led_matrix_display_symbol(MINUS_SYMBOL);
        }
        state = LED_MATRIX;
        break;
    }
}
