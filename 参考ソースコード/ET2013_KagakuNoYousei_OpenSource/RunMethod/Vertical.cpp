#include "RunMethod/Inclination.h"
#include "RunMethod/RunMethod.h"
#include "RunMethod/Vertical.h"
#include "Motor.h"
#include "GyroSensor.h"
#include "Nxt.h"

using namespace ecrobot;

extern "C" {
#include "balancer.h" //�|���U�q����p�w�b�_�t�@�C��
}

Vertical::Vertical(Motor* pTailMotor,Motor* pLeftMotor,Motor* pRightMotor,GyroSensor* pGyroSensor,Nxt* pNxt)
	:	m_pTailMotor(pTailMotor),m_pLeftMotor(pLeftMotor),m_pRightMotor(pRightMotor),m_pGyroSensor(pGyroSensor),m_pNxt(pNxt) {
}

Vertical::~Vertical() {
}

//�K�����s����|���ɕύX�������ɍs������
void Vertical::changePosture() {

	balance_init();
	m_pLeftMotor->setCount(0);	//�����[�^�G���R�[�_���Z�b�g
	m_pRightMotor->setCount(0);	//�E���[�^�G���R�[�_���Z�b�g
}

void Vertical::adjust(int speed,signed char circle,signed char& L,signed char& R) {

	if(m_current_posture == Posture::VER) {
		changePosture();
		m_current_posture = Posture::INC;
	}

	float GYRO_OFFSET=611;	  //�W���C���I�t�Z�b�g�l �@�̂ɍ��킹�ĕύX
	signed char forward;      //�O��i����
	signed char turn;         //���񖽗�
	signed char pwm_L, pwm_R; //���E���[�^PWM�o��
	signed char pwmt;
	int angle;

	forward = speed;
	turn = circle;

	balance_control(
		(float)forward,
		(float)turn,
		(float)m_pGyroSensor->get(),
		(float)GYRO_OFFSET,
		(float)m_pLeftMotor->getCount(),
		(float)m_pRightMotor->getCount(),
		(float)m_pNxt->getBattMv(),
		&pwm_L,
		&pwm_R);

	L = pwm_L;
	R = pwm_R;

	angle = m_pTailMotor->getCount();

	if(angle>10) {
		pwmt = -20;
	}
	else {
		pwmt = 0;
	}

	m_pTailMotor->setPWM((signed char)pwmt);

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

