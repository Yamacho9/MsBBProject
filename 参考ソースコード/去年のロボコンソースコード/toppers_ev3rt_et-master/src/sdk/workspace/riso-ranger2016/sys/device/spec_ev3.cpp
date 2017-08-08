///////////////////////////////////////////////////////////
//  spec_ev3.cpp
//  Implementation of the Class spec_ev3
//  Created on:      2016/05/23 18:20:06
//  Original author: 037789
///////////////////////////////////////////////////////////


#include "spec_ev3.h"


const T_SensorConfig st_sensor_config_tbl[] = {
    {EV3_PORT_1, TOUCH_SENSOR},
    {EV3_PORT_2, ULTRASONIC_SENSOR},
    {EV3_PORT_3, COLOR_SENSOR},
    {EV3_PORT_4, GYRO_SENSOR},
};

const T_MotorConfig st_motor_config_tbl[] = {
    {EV3_PORT_A, LARGE_MOTOR},
    {EV3_PORT_B, LARGE_MOTOR},
    {EV3_PORT_C, LARGE_MOTOR},
    {EV3_PORT_D, NONE_MOTOR},
};

spec_ev3::spec_ev3(){

}



spec_ev3::~spec_ev3(){

}
