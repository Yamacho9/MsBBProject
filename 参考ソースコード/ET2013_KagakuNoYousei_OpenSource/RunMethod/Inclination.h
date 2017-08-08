#ifndef INCLINATION_H
#define INCLINATION_H

#include "RunMethod/PID.h"
#include "RunMethod/Posture.h"
#include "Motor.h"

using namespace ecrobot;

class Inclination : public Posture {

private:
	//ŠÖ˜A
	Motor* m_pTailMotor;
	PID* m_pid;

	int m_target_angle;
	int m_PWM_ABS_MAX;
	virtual void changePosture();

public:
	void adjust(int speed,signed char circle,signed char& L,signed char& R);
	Inclination(PID* pid, int target,int MAX,Motor* pTailMotor);
	virtual ~Inclination();

};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

