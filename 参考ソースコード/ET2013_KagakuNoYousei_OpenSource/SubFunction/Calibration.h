#ifndef CAlIBRATION_H
#define CAlIBRATION_H

#include "SubFunction/Sound.h"
#include "TouchSensor.h"
#include "LightSensor.h"
#include "Clock.h"
#include "Lcd.h"
#include "GyroSensor.h"

using namespace ecrobot;

class Calibration {
private:

	int m_border;
	int m_black;
	int m_white;
	int m_gray;
	//ŠÖ˜A
	Sound* m_pSound;
	TouchSensor* m_pTouchSensor;
	LightSensor* m_pLightSensor;
	Clock* m_pClock;
	Lcd* m_pLcd;
	GyroSensor* m_pGyroSensor;

public:
	Calibration(Sound* pSound,TouchSensor* pTouchSensor,LightSensor* pLightSensor,Clock* pClock,Lcd* pLcd,GyroSensor* pGyroSensor);
	int calBrightness();
	void calibrate();
	int getBlack();
	int getBorder();
	virtual ~Calibration();

};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

