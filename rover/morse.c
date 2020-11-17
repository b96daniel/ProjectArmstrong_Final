#include "morse.h"

uint8_t first_rising_edge = 1;
uint32_t morse_line_changed_ms = 0;

uint8_t morse_line = LIGHT_OFF;
uint8_t prev_morse_line = LIGHT_OFF;

morse_state_t morse_state = MORSE_LINE_LOW;

junction_type_t junction_types[JUNCTION_BUFFER_SIZE] = {0};
uint16_t junction_counter = 0;

uint8_t morse_data = 0;
uint8_t last_morse_pulse = 0;

uint8_t get_light_sensors_level(void)
{
    uint16_t left_value = light_sensor_get_value(light_sensor_left);
    uint16_t right_value = light_sensor_get_value(light_sensor_right);
    uint16_t mean_value = (left_value + right_value) / 2;
    //printf("%i\t%i\t", left_value, right_value);

    if (mean_value > LIGHT_SENSOR_TRIGGER_LEVEL && left_value > (LIGHT_SENSOR_TRIGGER_LEVEL / 2) && right_value > (LIGHT_SENSOR_TRIGGER_LEVEL / 2))
    {
        /* TEST PRINT BEGIN */
        //printf("HIGH\r\n");
        /* TEST PRINT END */
        return LIGHT_ON;
    }
    else
    {
        /* TEST PRINT BEGIN */
        //printf("LOW\r\n");
        /* TEST PRINT END */
        return LIGHT_OFF;
    }
}

void morse_communication(void)
{
    /* TEST PRINT BEGIN */
    //printf("%i\t%i\t", (int)timer_get_time_ms(), morse_state);
    /* TEST PRINT END */
    uint32_t current_time = timer_get_time_ms();

    switch (morse_state)
    {

    case MORSE_LINE_LOW:
        morse_line = get_light_sensors_level();
        if (morse_line != prev_morse_line)
        {
            if (first_rising_edge == 0)
            {
                if ((current_time - morse_line_changed_ms) < MORSE_PULSE_LOW_LIMIT_MS)
                {
                    morse_state = MORSE_ELEMENT_GAP;
                }
                else if ((current_time - morse_line_changed_ms) < MORSE_PULSE_HIGH_LIMIT_MS)
                {
                    morse_state = MORSE_LETTER_GAP;
                }
                else
                {
                    last_morse_pulse = 1;
                    morse_state = MORSE_LETTER_GAP;
                }
            }
            else
            {
                morse_state = MORSE_LINE_HIGH;
            }

            morse_line_changed_ms = current_time;
            prev_morse_line = morse_line;
        }
        else if (((current_time - morse_line_changed_ms) > MORSE_PULSE_HIGH_LIMIT_MS) && (first_rising_edge == 0))
        {
            last_morse_pulse = 1;
            morse_state = MORSE_LETTER_GAP;
        }
        break;

    case MORSE_LINE_HIGH:
        morse_line = get_light_sensors_level();
        if (morse_line != prev_morse_line)
        {
            if ((current_time - morse_line_changed_ms) < MORSE_PULSE_LOW_LIMIT_MS)
            {
                morse_state = MORSE_DOT;
            }
            else
            {
                morse_state = MORSE_DASH;
            }
            first_rising_edge = 0;

            morse_line_changed_ms = current_time;
            prev_morse_line = morse_line;
        }
        break;

    case MORSE_DOT:
        morse_data |= 1;
        /* TEST PRINT BEGIN */
        //printf("\r\n");
        /* TEST PRINT END */
        morse_state = MORSE_LINE_LOW;
        break;

    case MORSE_DASH:
        morse_data |= 2;
        /* TEST PRINT BEGIN */
        //printf("\r\n");
        /* TEST PRINT END */
        morse_state = MORSE_LINE_LOW;
        break;

    case MORSE_ELEMENT_GAP:
        morse_data <<= 2;
        //printf("\r\n");
        morse_state = MORSE_LINE_HIGH;
        break;

    case MORSE_LETTER_GAP:
        if (morse_data == MORSE_J_CHARACTER)
        {
            junction_types[junction_counter++] = TURN_RIGHT_CODE;
            /* TEST PRINT BEGIN */
            printf("J:\tTURN_RIGHT_CODE\r\n");
            /* TEST PRINT END */
        }
        else if (morse_data == MORSE_B_CHARACTER)
        {
            junction_types[junction_counter++] = TURN_LEFT_CODE;
            /* TEST PRINT BEGIN */
            printf("B:\tTURN_LEFT_CODE\r\n");
            /* TEST PRINT END */
        }
        else
        {
            /* TEST PRINT BEGIN */
            printf("Unknown Morse Code!\r\n");
            /* TEST PRINT END */
        }

        if (junction_counter > JUNCTION_BUFFER_SIZE)
        {
            junction_counter = 0;
            /* TEST PRINT BEGIN */
            // printf("\r\nJunction buffer OVF!!!\r\n");
            /* TEST PRINT END */
        }

        if (last_morse_pulse == 1)
        {
            morse_state = MORSE_FINISH;
        }
        else
        {
            morse_state = MORSE_LINE_HIGH;
        }
        morse_data = 0;
        break;

    case MORSE_FINISH:
        break;
    }
}
