//******************************************************************************
//�t�@�C���� : LineTraceMethod.cpp
//
//�T�v : ���s�N���X�i�X�[�p�[�N���X�j�̃T�u�N���X�ł��B�������e��PID�����p�������C���g���[�X���s�ł��B
//
//���l : ���ӁI�I�K�����s�̎���m_speed���m_turn_MAX���傫���Ȃ�Ȃ����ƁI�I�I�I
//
//******************************************************************************

#include "RunMethod/RunMethod.h"
#include "RunMethod/LineTraceMethod.h"
#include "SubFunction/Calibration.h"
#include "LightSensor.h"
#include "Motor.h"

using namespace ecrobot;

extern "C"
{
#include "balancer.h" //�|���U�q����p�w�b�_�t�@�C��
}

LineTraceMethod::LineTraceMethod(Calibration* cali,Posture* posture,PID* p_i_d,int speed,int max,LightSensor* pLightSensor,Motor* pLeftMotor,Motor* pRightMotor)
	:m_pLightSensor(pLightSensor),m_pLeftMotor(pLeftMotor),m_pRightMotor(pRightMotor) {

	m_cali = cali;
	m_posture = posture;
	m_pid = p_i_d;
	m_speed = speed;
	m_turn_MAX = max;
}

LineTraceMethod::~LineTraceMethod() {
}

void LineTraceMethod::run() {


	signed char pwm_L, pwm_R; //���E���[�^PWM�o��
	int sift;
	int light;

	//m_speed���m_turn_MAX���傫���Ȃ�Ȃ��悤�ɂ��鑀��B�G���[���
	if(m_speed < m_turn_MAX) {
		m_turn_MAX = m_speed;
	}


	light = m_cali->getBorder() - m_pLightSensor->get();
	sift = m_pid->calcControllValue(light);
	sift=(sift>m_turn_MAX)?m_turn_MAX:(sift<(m_turn_MAX*(-1)))?(m_turn_MAX*(-1)):sift;


	m_posture->adjust(m_speed, sift, pwm_L, pwm_R);

	m_pLeftMotor->setPWM(pwm_L); //�����[�^PWM�o�̓Z�b�g(-100�`100)
	m_pRightMotor->setPWM(pwm_R); //�E���[�^PWM�o�̓Z�b�g(-100�`100)


}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

