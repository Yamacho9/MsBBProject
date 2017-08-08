///////////////////////////////////////////////////////////
//  LUltrasonicControl.cpp
//  Implementation of the Class LUltrasonicControl
//  Created on:      2016/07/15 16:04:13
//  Original author: 039889
///////////////////////////////////////////////////////////

#include "UltrasonicControl.h"


UltrasonicControl::UltrasonicControl(sensor_port_t PortNum){

	// メンバ初期化
	m_Port = PortNum;

	// ポート設定
	ev3_sensor_config(m_Port, ULTRASONIC_SENSOR);
}


UltrasonicControl::~UltrasonicControl(){

}
int16_t UltrasonicControl::getUltrasonicValue(){
	return ev3_ultrasonic_sensor_get_distance(m_Port);
}

