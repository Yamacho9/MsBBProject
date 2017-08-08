#include "SubFunction/Stop.h"
#include "Motor.h"
#include "GyroSensor.h"
#include "Speaker.h"

using namespace ecrobot;

DeclareResource(resource1);		//排他処理で使用します。

Stop::Stop(Motor* pTailMotor,
		   Motor* pLeftMotor,
		   Motor* pRightMotor,
		   GyroSensor* pGyroSensor,
		   Speaker* pSpeaker)
	:	m_pTailMotor(pTailMotor),
		m_pLeftMotor(pLeftMotor),
		m_pRightMotor(pRightMotor),
		m_pGyroSensor(pGyroSensor),
		m_pSpeaker(pSpeaker)
{
}

Stop::~Stop() {
}

void Stop::stop() {

	if(300>m_pGyroSensor->get() || 850<m_pGyroSensor->get()) { //転倒時（ジャイロセンサ値が300より小さい　又は　850より大きい時）

		GetResource(resource1);						//排他処理　ReleaseResource(resource1)まで他のタスクを停止させます。

		m_pTailMotor->setPWM(0); //Aモータ停止
		m_pLeftMotor->setPWM(0); //Bモータ停止
		m_pRightMotor->setPWM(0); //Cモータ停止
		m_pSpeaker->playTone(600,100,60);
		systick_wait_ms(20000);	//20secウェイト
		ReleaseResource(resource1);

	}
}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

