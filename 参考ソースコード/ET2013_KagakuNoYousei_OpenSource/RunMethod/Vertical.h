#ifndef VERTICAL_H
#define VERTICAL_H

#include "RunMethod/Posture.h"
#include "Motor.h"
#include "GyroSensor.h"
#include "Nxt.h"

using namespace ecrobot;


class Vertical : public Posture {
	
private:
	virtual void changePosture();

	//ŠÖ˜A
	Motor* m_pTailMotor;
	Motor* m_pLeftMotor;
	Motor* m_pRightMotor;
	GyroSensor*	m_pGyroSensor;
	Nxt* m_pNxt;

public:
	void adjust(int speed,signed char circle,signed char& L,signed char& R);
	Vertical(Motor* pTailMotor,Motor* pLeftMotor,Motor* pRightMotor,GyroSensor* pGyroSensor,Nxt* pNxt);
	virtual ~Vertical();

};


#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

