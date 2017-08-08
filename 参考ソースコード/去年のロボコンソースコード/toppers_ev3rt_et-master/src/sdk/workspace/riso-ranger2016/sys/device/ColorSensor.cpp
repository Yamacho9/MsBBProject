///////////////////////////////////////////////////////////
//  ColorSensor.cpp
//  Implementation of the Class ColorSensor
//  Created on:      2016/07/26 21:04:52
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "ColorSensor.h"


ColorSensor::ColorSensor(sensor_port_t port){

	// メンバ初期化
	m_Port = port;

	// ポート設定
	ev3_sensor_config(m_Port, COLOR_SENSOR);
}


ColorSensor::~ColorSensor(){

}


int ColorSensor::getSensorValue(){
	return ev3_color_sensor_get_reflect(m_Port); //カラーセンサから明度を取得;
}
