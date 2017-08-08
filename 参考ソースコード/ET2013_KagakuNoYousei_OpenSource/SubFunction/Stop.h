#ifndef STOP_H
#define STOP_H

#include "Motor.h"
#include "GyroSensor.h"
#include "Speaker.h"

using namespace ecrobot;

class Stop {

private:

	//ŠÖ˜A
	Motor* m_pTailMotor;
	Motor* m_pLeftMotor;
	Motor* m_pRightMotor;
	GyroSensor*	m_pGyroSensor;
	Speaker* m_pSpeaker;

public:
	Stop(Motor* pTailMotor,
		 Motor* pLeftMotor,
		 Motor* pRightMotor,
		 GyroSensor* pGyroSensor,
		 Speaker* pSpeaker);

	virtual ~Stop();

	void stop();

};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

