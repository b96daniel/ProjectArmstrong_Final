#ifndef _COMPASS
#define _COMPASS

#include <stdint.h>
#include "microbit/i2c.h"

#define MAGN_ADDRESS    0x1E
#define CFG_REG_A_M     0x60 
#define CFG_REG_B_M     0x61 
#define CFG_REG_C_M     0x62 
#define CTRL_REG1_A     0x20
#define STAT_REG_M      0x67


#define OUTX_L_REG_M    0x68
#define OUTX_H_REG_M    0x69 
#define OUTY_L_REG_M    0x6A
#define OUTY_H_REG_M    0x6B 
#define OUTZ_L_REG_M    0x6C
#define OUTZ_H_REG_M    0x6D

void compass_init(void);
int16_t read_magnetometer_y(void);
int16_t get_orientation(void);

#endif