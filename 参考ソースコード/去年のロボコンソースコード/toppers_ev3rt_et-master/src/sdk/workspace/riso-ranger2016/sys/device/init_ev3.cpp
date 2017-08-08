///////////////////////////////////////////////////////////
//  init_ev3.cpp
//  Implementation of the Class init_ev3
//  Created on:      2016/05/23 18:07:57
//  Original author: 037789
///////////////////////////////////////////////////////////

#include "ev3api.h"
#include "std.h"
#include "spec_ev3.h"
#include "init_ev3.h"


init_ev3::init_ev3(){

}



init_ev3::~init_ev3(){

}





void init_ev3::initialize(){
    // I/O 設定.

    // センサポート設定.
    for(uint8 i = 0; i < TNUM_SENSOR_PORT; i++) {
        ev3_sensor_config(st_sensor_config_tbl[i].port, st_sensor_config_tbl[i].type);
    }

    // モータポート設定.
    for(uint8 i = 0; i < TNUM_MOTOR_PORT; i++) {
        ev3_motor_config(st_motor_config_tbl[i].port, st_motor_config_tbl[i].type);
    }


}
