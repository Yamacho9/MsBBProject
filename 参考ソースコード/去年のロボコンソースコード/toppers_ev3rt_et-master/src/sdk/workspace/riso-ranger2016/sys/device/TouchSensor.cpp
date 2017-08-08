/*
 * TouchSensor.cpp
 *
 *  Created on: 2016/07/30
 *      Author: Reina
 */

#include "TouchSensor.h"

TouchSensor::TouchSensor(sensor_port_t port) {

	// メンバ初期化
	m_Port = port;

	//タッチセンサを初期化
	ev3_sensor_config(m_Port, TOUCH_SENSOR);
}

TouchSensor::~TouchSensor() {
}


bool TouchSensor::getSensorValue(){
	return ev3_touch_sensor_is_pressed(m_Port);
}
