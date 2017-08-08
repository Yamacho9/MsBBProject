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

//倒立からしっぽ走行に変えるときにする特別な動作を記述
void Inclination::changePosture() {
}

void Inclination::adjust(int speed,signed char circle,signed char& L,signed char& R) {

	if(m_current_posture == Posture::INC) {	//前の動作が倒立だったかどうかを判断
		changePosture();
		m_current_posture = Posture::VER;
	}

	int pwmt = 0;
	int angle = 0;

	//注意 LとRは参照渡しです。
	if (circle >= 0) {	//直進と右旋回命令のとき
		L = speed;
		R = speed - circle;
	}
	else {			//左旋回命令のとき
		L = speed + circle;
		R = speed;
	}

	angle = m_target_angle - m_pTailMotor->getCount();
	pwmt = m_pid->calcControllValue(angle);

	//PWM出力飽和処理
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

