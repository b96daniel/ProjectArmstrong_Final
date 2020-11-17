#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include "microbit/uart.h"
#include "microbit/timer.h"
#include "microbit/led_matrix.h"
#include "bitbot/line_sensor.h"
#include "bitbot/light_sensor.h"
#include "bitbot/motor.h"
#include "compass.h"
#include "control.h"
#include "morse.h"
#include "map.h"

typedef enum state_type
{
    BARRIER = 0,
    START,
    LAMP,
    MORSE,
    TEMP,
    ROUTING,
    COMPASS,
    REVERSE,
    HOME,
    LED_MATRIX
} state_type_t;

extern state_type_t state;

#endif
