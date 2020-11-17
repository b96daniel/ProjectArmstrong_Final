#include "compass.h"
#include <stdio.h>

void compass_init()
{
    uint8_t write_data = 0;
    i2c_write(MAGN_ADDRESS, CFG_REG_A_M, &write_data, 1);
}

uint8_t m_count = 0;

int16_t read_magnetometer_y(void)
{
    uint8_t read_data[2];
    int16_t y = 0, x = 0, z = 0;
    uint8_t status = 0;

    while (1)
    {
        i2c_read(MAGN_ADDRESS, STAT_REG_M, &status, 1) ;
        if((status & 0x0F) == 0x0F){
            i2c_read(MAGN_ADDRESS, OUTX_L_REG_M, read_data, 2);
            x = (read_data[1] << 8) | (read_data[0]);   

            i2c_read(MAGN_ADDRESS, OUTY_L_REG_M, read_data, 2);
            y = (read_data[1] << 8) | (read_data[0]);

            i2c_read(MAGN_ADDRESS, OUTZ_L_REG_M, read_data, 2);
            z = (read_data[1] << 8) | (read_data[0]);
            m_count++;
        }
        if (m_count >= 5) break;
    }
    
    return y;
}

int16_t get_orientation()
{
    if (read_magnetometer_y() < 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}
