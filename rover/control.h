#ifndef _CONTROL
#define _CONTROL

#include "bitbot/ultrasonic_sensor.h"
#include "bitbot/motor.h"
#include "bitbot/light_sensor.h"
#include "motor_control.h"
#include <stdint.h>

#define BARR_DIST       5U
#define BARR_THRESH     1U
#define AT_BARRIER      1U
#define BARR_KP         10
#define SPEED           50U
#define AT_LAMP         1U



uint8_t goto_barrier(void);
uint8_t barrier_lifted(void);
uint8_t goto_lamp(void);
void drive(int16_t speed, line_state_t line);

#endif  