#include "RunMethod/Inclination.h"
#include "RunMethod/RunMethod.h"
#include "RunMethod/Vertical.h"
#include "Motor.h"
#include "GyroSensor.h"
#include "Nxt.h"

using namespace ecrobot;

extern "C" {
#include "balancer.h" //倒立振子制御用ヘッダファイル
}

Vertical::Vertical(Motor* pTailMotor,Motor* pLeftMotor,Motor* pRightMotor,GyroSensor* pGyroSensor,Nxt* pNxt)
	:	m_pTailMotor(pTailMotor),m_pLeftMotor(pLeftMotor),m_pRightMotor(pRightMotor),m_pGyroSensor(pGyroSensor),m_pNxt(pNxt) {
}

Vertical::~Vertical() {
}

//尻尾走行から倒立に変更した時に行う動作
void Vertical::changePosture() {

	balance_init();
	m_pLeftMotor->setCount(0);	//左モータエンコーダリセット
	m_pRightMotor->setCount(0);	//右モータエンコーダリセット
}

void Vertical::adjust(int speed,signed char circle,signed char& L,signed char& R) {

	if(m_current_posture == Posture::VER) {
		changePosture();
		m_current_posture = Posture::INC;
	}

	float GYRO_OFFSET=611;	  //ジャイロオフセット値 機体に合わせて変更
	signed char forward;      //前後進命令
	signed char turn;         //旋回命令
	signed char pwm_L, pwm_R; //左右モータPWM出力
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

