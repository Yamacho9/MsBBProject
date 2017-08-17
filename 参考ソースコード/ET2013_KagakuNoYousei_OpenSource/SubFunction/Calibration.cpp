#include "Calibration.h"
#include "SubFunction/Sound.h"
#include "TouchSensor.h"
#include "LightSensor.h"
#include "Clock.h"
#include "Lcd.h"
#include "GyroSensor.h"

using namespace ecrobot;

Calibration::Calibration(Sound* pSound,
						 TouchSensor* pTouchSensor,
						 LightSensor* pLightSensor,
						 Clock* pClock,
						 Lcd* pLcd,
						 GyroSensor* pGyroSensor)
	:m_pSound(pSound),m_pTouchSensor(pTouchSensor),m_pLightSensor(pLightSensor),m_pClock(pClock),m_pLcd(pLcd),m_pGyroSensor(pGyroSensor) {

}

Calibration::~Calibration() {
}

int Calibration::calBrightness() {

	int point = 0;

	while(!m_pTouchSensor->isPressed());

	for(int i = 0; i <= 99; i++) {
		point += m_pLightSensor->get();
		m_pClock->wait(1);
	}

	point = point/100;
	m_pSound->instructBeat();

	m_pClock->wait(500);

	return point;

}



void Calibration::calibrate() {
	//���F�̋P�x���擾�E��ʕ\��
	m_pLcd->clear();						//��ʂ̏�����
	m_pLcd->cursor(0,0);					//�o�͈ʒu�̎w��
	m_pLcd->putf("s","Get color BLACK");	//�o�͕�����
	m_pLcd->disp();							//��ʂɏo��
	m_black = calBrightness();				//���F�̋P�x���擾
	m_pLcd->cursor(2,1);
	m_pLcd->putf("d",m_black,0);			//�o�͒l�@���F�P�x

	// ���W���C���I�t�Z�b�g�l�m�F�p�@�@�̂�Î~��������ԂŎ擾�ł��܂��B����̓L�����u���[�V�����ł͂Ȃ��̂Œ���
	m_pLcd->cursor(2,7);
	int gyro;
	gyro =m_pGyroSensor->get();
	m_pLcd->putf("d",gyro,0);
	// �������܂� ******************************

	m_pLcd->disp();


	//���F�̋P�x���擾�E��ʕ\��
	m_pLcd->cursor(0,2);
	m_pLcd->putf("s","Get color WHITE");
	m_pLcd->disp();
	m_white = calBrightness();				//���F�̋P�x���擾
	m_pLcd->cursor(2,3);
	m_pLcd->putf("d",m_white,0);
	m_pLcd->disp();


	//�D�F�̋P�x���擾�E��ʕ\��
	m_pLcd->cursor(0,4);
	m_pLcd->putf("s","Get color GRAY");
	m_pLcd->disp();
	m_gray = calBrightness(); 				//�D�F�̋P�x���擾
	m_pLcd->cursor(2,5);
	m_pLcd->putf("d",m_gray,0);
	m_pLcd->disp();


	//���E���P�x���Z�o
	m_border =static_cast<int>((m_black + m_white + m_gray)/3 );

	m_pLcd->cursor(0,6);
	m_pLcd->putf("s","border");
	m_pLcd->disp();
	m_pLcd->cursor(2,7);
	m_pLcd->putf("d",m_border,0);
	m_pLcd->disp();


}


int Calibration::getBlack() {

	return m_black;
}


int Calibration::getBorder() {

	return m_border;
}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

