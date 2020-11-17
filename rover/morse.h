#ifndef MORSE_H_
#define MORSE_H_

#include "bitbot/light_sensor.h"
#include "microbit/timer.h"
#include "microbit/uart.h"
#include <stdio.h>

#define LIGHT_SENSOR_TRIGGER_LEVEL 75

#define MORSE_PULSE_MIN_LIMIT_MS 50
#define MORSE_PULSE_LOW_LIMIT_MS 200
#define MORSE_PULSE_HIGH_LIMIT_MS 500

#define LIGHT_ON 1
#define LIGHT_OFF 0
#define JUNCTION_BUFFER_SIZE 10
#define MORSE_J_CHARACTER 0b01101010
#define MORSE_B_CHARACTER 0b10010101

typedef enum morse_state
{
    MORSE_LINE_LOW,
    MORSE_LINE_HIGH,
    MORSE_DOT,
    MORSE_DASH,
    MORSE_ELEMENT_GAP,
    MORSE_LETTER_GAP,
    MORSE_FINISH
} morse_state_t;

typedef enum junction_type
{
    TURN_RIGHT_CODE = -1,
    UNKNOWN=0,
    TURN_LEFT_CODE = 1
} junction_type_t;

extern junction_type_t junction_types[JUNCTION_BUFFER_SIZE];
extern uint16_t junction_counter;
extern morse_state_t morse_state;

void morse_communication( void );

#endif
