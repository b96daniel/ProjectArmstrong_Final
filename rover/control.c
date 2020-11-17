#include "control.h"
#include "stdio.h"

uint8_t goto_barrier(void)
{
    uint32_t barr_distance=ultrasonic_sensor_get_distance_cm();
    
    if ((barr_distance-BARR_DIST)<BARR_THRESH)
    {
        motor_set_percent(motor_left, 0);
		motor_set_percent(motor_right, 0);
        return AT_BARRIER;
    }
    else
    {
        line_state_t line = get_line_state(line_sensor_is_line_present(line_sensor_left),
                                         line_sensor_is_line_present(line_sensor_right));
        if (line==BOTH)  
        {
            line=NEITHER;
        }                            
        
        drive(30, line);       
        return 0;   
    }
}

uint8_t barrier_lifted(void)
{
    if(ultrasonic_sensor_get_distance_cm() > (BARR_DIST + 10*BARR_THRESH) )
    {
        return 1;
    }
    else return 0;
}

uint8_t goto_lamp(void)
{   
    if ( (light_sensor_get_value(light_sensor_left) < ambient_light/3 ) && 
        (light_sensor_get_value(light_sensor_right) < ambient_light/3))      
    {
        motor_set_percent(motor_left, 0);
		motor_set_percent(motor_right, 0);
        return AT_LAMP;
    }
    else 
    {  
        line_state_t line = get_line_state(line_sensor_is_line_present(line_sensor_left),
                                         line_sensor_is_line_present(line_sensor_right));
        drive(SPEED, line);
        return 0;
    }
}

void drive(int16_t speed, line_state_t line)
{
    mean_speed=speed;
	line_following(line);

}
