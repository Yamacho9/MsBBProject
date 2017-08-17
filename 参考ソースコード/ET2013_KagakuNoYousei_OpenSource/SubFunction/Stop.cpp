#include "SubFunction/Stop.h"
#include "Motor.h"
#include "GyroSensor.h"
#include "Speaker.h"

using namespace ecrobot;

DeclareResource(resource1);		//�r�������Ŏg�p���܂��B

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

	if(300>m_pGyroSensor->get() || 850<m_pGyroSensor->get()) { //�]�|���i�W���C���Z���T�l��300��菬�����@���́@850���傫�����j

		GetResource(resource1);						//�r�������@ReleaseResource(resource1)�܂ő��̃^�X�N���~�����܂��B

		m_pTailMotor->setPWM(0); //A���[�^��~
		m_pLeftMotor->setPWM(0); //B���[�^��~
		m_pRightMotor->setPWM(0); //C���[�^��~
		m_pSpeaker->playTone(600,100,60);
		systick_wait_ms(20000);	//20sec�E�F�C�g
		ReleaseResource(resource1);

	}
}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

