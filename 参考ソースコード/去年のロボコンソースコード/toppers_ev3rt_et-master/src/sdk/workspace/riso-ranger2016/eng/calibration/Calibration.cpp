///////////////////////////////////////////////////////////
//  Calibration.cpp
//  Implementation of the Class Calibration
//  Created on:      2016/07/05 11:43:06
//  Original author: 039918
///////////////////////////////////////////////////////////

#include "Calibration.h"


Calibration::Calibration(ColorSensor* colorSensor){
	// メンバ初期化
	m_ColorSensor = colorSensor;
	m_LightValueMax = 55;
	m_LightValueMin = 2;
	m_CalibrationIndex = 0;
}


Calibration::~Calibration(){

}


void Calibration::onPressed(){

	if(m_CalibrationIndex == 0) {	// 白
		this->ConfigureWhiteValue();
		m_CalibrationIndex++;
	}
	else {							// 黒
		this->ConfigureLineValue();
		m_CalibrationIndex = 0;
	}

}


void Calibration::onReleased(){

}


float Calibration::GetNormalizedSensorValue(){

	//bool_t isPressed = ev3_touch_sensor_is_pressed(EV3_PORT_1); //タッチセンサの状態検出

	// 光センサ値を正規化
	float normalizedSensorValue = ((float)m_ColorSensor->getSensorValue() - (float)m_LightValueMin) /( (float)m_LightValueMax- (float)m_LightValueMin);
//	printf("normalizedSensor=%f colorsensor= %d \n",normalizedSensorValue,m_ColorSensor->getSensorValue());
	// 範囲チェック＆丸め込み
	if(normalizedSensorValue < 0.0) {
		normalizedSensorValue = 0;
	}
	else if(1.0 < normalizedSensorValue) {
		normalizedSensorValue = 1.0;
	}


	return normalizedSensorValue;

}


// private method

void Calibration::ConfigureWhiteValue(){
	m_LightValueMin = m_ColorSensor->getSensorValue();
}


void Calibration::ConfigureLineValue(){
	m_LightValueMax = m_ColorSensor->getSensorValue();
}
