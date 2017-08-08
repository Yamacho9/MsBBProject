//******************************************************************************
//ファイル名 : LineTraceMethod.cpp
//
//概要 : 走行クラス（スーパークラス）のサブクラスです。処理内容はPID制御を用いたライントレース走行です。
//
//備考 : 注意！！尻尾走行の時はm_speedよりm_turn_MAXが大きくならないこと！！！！
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
#include "balancer.h" //倒立振子制御用ヘッダファイル
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


	signed char pwm_L, pwm_R; //左右モータPWM出力
	int sift;
	int light;

	//m_speedよりm_turn_MAXが大きくならないようにする操作。エラー回避
	if(m_speed < m_turn_MAX) {
		m_turn_MAX = m_speed;
	}


	light = m_cali->getBorder() - m_pLightSensor->get();
	sift = m_pid->calcControllValue(light);
	sift=(sift>m_turn_MAX)?m_turn_MAX:(sift<(m_turn_MAX*(-1)))?(m_turn_MAX*(-1)):sift;


	m_posture->adjust(m_speed, sift, pwm_L, pwm_R);

	m_pLeftMotor->setPWM(pwm_L); //左モータPWM出力セット(-100～100)
	m_pRightMotor->setPWM(pwm_R); //右モータPWM出力セット(-100～100)


}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

