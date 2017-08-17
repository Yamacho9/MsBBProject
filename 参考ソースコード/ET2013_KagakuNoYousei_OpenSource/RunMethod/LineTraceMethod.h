//******************************************************************************
//�t�@�C���� : LineTraceMethod.h
//
//�T�v : ���s�N���X�i�X�[�p�[�N���X�j�̃T�u�N���X�ł��B�������e��PID�����p�������C���g���[�X���s�ł��B
//
//���l :
//******************************************************************************

#ifndef LINETRACE_METHOD_H
#define LINETRACE_METHOD_H

#include "RunMethod/RunMethod.h"
#include "SubFunction/Calibration.h"
#include "RunMethod/PID.h"
#include "LightSensor.h"
#include "Motor.h"

using namespace ecrobot;

class LineTraceMethod:public RunMethod {

private:
	int m_speed;
	int m_turn_MAX;
	int m_border_change;

	//�֘A
	Calibration* m_cali;
	PID* m_pid;
	LightSensor* m_pLightSensor;
	Motor* m_pLeftMotor;
	Motor* m_pRightMotor;



public:
	void run();
	LineTraceMethod(Calibration* cali,Posture* posture,PID* p_i_d,int speed,int max,LightSensor* pLightSensor,Motor* pLeftMotor,Motor* pRightMotor);
	virtual ~LineTraceMethod();


};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

