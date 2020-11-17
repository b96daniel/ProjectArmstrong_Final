#ifndef MAP_H_
#define MAP_H_

#include <stdio.h>
#include "bitbot/line_sensor.h"
#include "main.h"
#include "motor_control.h"
#include "morse.h"
#include "control.h"

#define JTIME_THRESH 500

typedef enum mode_type
{
	MODE_SINGLE,
	MODE_JUNCTION,
	MODE_END
} mode_type_t;

void routing_state_machine(void);

void navigate_home(int16_t orient);
void init_edges(void);

#endif
