/*
 * GyroSensor.cpp
 *
 *  Created on: 2016/08/05
 *      Author: ryuji
 */

#include "GyroSensor.h"

GyroSensor::GyroSensor(sensor_port_t portNum) {

	// メンバ初期化
	m_Port = portNum;

	// ポート設定
	ev3_sensor_config(m_Port, GYRO_SENSOR);
}

GyroSensor::~GyroSensor() {

}


int GyroSensor::getSensorValue(){
	return ev3_gyro_sensor_get_rate(m_Port);
}

int GyroSensor::getGyroOffset(){
	return c_GyroOffset;
}


void GyroSensor::reset(){
	ev3_gyro_sensor_reset(m_Port);
}
