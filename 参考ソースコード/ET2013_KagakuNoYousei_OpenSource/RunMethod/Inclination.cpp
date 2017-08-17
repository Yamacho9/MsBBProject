#include "RunMethod/Inclination.h"
#include "RunMethod/RunMethod.h"
#include "Motor.h"

using namespace ecrobot;

Inclination::Inclination(PID* pid,int target,int MAX,Motor* pTailMotor):m_pTailMotor(pTailMotor) {

	m_pid = pid;
	m_target_angle = target;
	m_PWM_ABS_MAX = MAX;
}

Inclination::~Inclination() {
}

//�|�����炵���ۑ��s�ɕς���Ƃ��ɂ�����ʂȓ�����L�q
void Inclination::changePosture() {
}

void Inclination::adjust(int speed,signed char circle,signed char& L,signed char& R) {

	if(m_current_posture == Posture::INC) {	//�O�̓��삪�|�����������ǂ����𔻒f
		changePosture();
		m_current_posture = Posture::VER;
	}

	int pwmt = 0;
	int angle = 0;

	//���� L��R�͎Q�Ɠn���ł��B
	if (circle >= 0) {	//���i�ƉE���񖽗߂̂Ƃ�
		L = speed;
		R = speed - circle;
	}
	else {			//�����񖽗߂̂Ƃ�
		L = speed + circle;
		R = speed;
	}

	angle = m_target_angle - m_pTailMotor->getCount();
	pwmt = m_pid->calcControllValue(angle);

	//PWM�o�͖O�a����
	if (pwmt > m_PWM_ABS_MAX)
	{
		pwmt = m_PWM_ABS_MAX;
	}
	else if (pwmt < -m_PWM_ABS_MAX)
	{
		pwmt = -m_PWM_ABS_MAX;
	}

	m_pTailMotor->setPWM((signed char)pwmt);


}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

