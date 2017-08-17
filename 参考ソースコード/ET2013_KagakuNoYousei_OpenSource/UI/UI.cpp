#include "UI/UI.h"
#include "SubFunction/Calibration.h"
#include "Communication/RemoteStart.h"
#include "SubFunction/Sound.h"
#include "TouchSensor.h"
#include "Clock.h"
#include "Motor.h"
#include "Lcd.h"
#include "Nxt.h"
#include "Bluetooth.h"

using namespace ecrobot;


extern "C"
{
#include "balancer.h"	//�|���U�q����p�w�b�_�t�@�C��
}

int UI::m_sound_flag = 0;

UI::UI(Posture* v,
	   RemoteStart* rs,
	   Calibration* pCalibration,
	   Sound* pSound,
	   TouchSensor* pTouchSensor,
	   Clock* pClock,
	   Motor* pTailMotor,
	   Motor* pLeftMotor,
	   Motor* pRightMotor,
	   Nxt* pNxt,
	   Bluetooth* pBluetooth,
	   Lcd* pLcd)
	: PASS_KEY("1234"),	//�p�X�L�[�̐ݒ�@�K�؂ɕύX���Ă�������
	  DEVICE_NAME("ET00"),	//�f�o�C�X�l�[���̐ݒ�@�K�؂ɕύX���Ă�������
	  m_pTouchSensor(pTouchSensor),
	  m_pClock(pClock),
	  m_pTailMotor(pTailMotor),
	  m_pLeftMotor(pLeftMotor),
	  m_pRightMotor(pRightMotor),
	  m_pNxt(pNxt),
	  m_pBluetooth(pBluetooth),
	  m_pLcd(pLcd)

{
	m_vertical = v;
	m_remotestart = rs;
	m_pCalibration = pCalibration;
	m_pSound = pSound;
}

UI::~UI() {
}

void UI::update() {

	if(m_sound_flag == 1) {
		m_pSound->instructBeat();
		m_sound_flag = 0;
	}
}


void UI::initialize() {

	//�f�o�C�X�l�[��
	m_pBluetooth->setFriendlyName(DEVICE_NAME);

	m_pLcd->clear();// LCD�\������
	m_pLcd->cursor(0, 0);
	m_pLcd->putf("snsnsn", "Connect by BT", "or", "Press RUN button");// "Connect by BT" "or" "Press RUN button"
	m_pLcd->disp();
	// Bluetooth�ʐM�ڑ� or RUN�{�^������ �܂Ń��[�v����
	while(true) {
		if(m_pBluetooth->waitForConnection(PASS_KEY, 10) == true) {
			break;						// Bluetooth�ʐM�ڑ��ς� �� ���[�v�𔲂���
		}
		if(m_pNxt->getButtons() & Nxt::RUN_ON == true) {
			break;						// RUN�{�^�������ς� �� ���[�v�𔲂���
		}

		m_pClock->wait(10);				// 10msec�҂�
	}

	m_pCalibration->calibrate();
}


void UI::prepareRun() {

	signed char turn = 0;     //���񖽗�
	signed char pwm_L, pwm_R; //���E���[�^PWM�o��
	int m_speed = 0;

	while(1) {
		balance_init();
		if(m_pTouchSensor->isPressed() == true) {

			break;
		}
		if(m_remotestart->remote_start() == true) {
			break;
		}
		m_vertical->adjust(m_speed,turn,pwm_L,pwm_R);
	}
}

void UI::setSound()
{

	m_sound_flag = 1;

}


void UI::tailBalance() {


	signed char turn = 0;  //���񖽗�
	signed char pwm_L = 0;
	signed char pwm_R = 0; //���E���[�^PWM�o��
	int m_speed = 0;


	while(m_pTouchSensor->isPressed()==true) { //�������܂ő҂�
	}

	m_pLeftMotor->setCount(0); 	//�����[�^�G���R�[�_���Z�b�g
	m_pRightMotor->setCount(0);	//�E���[�^�G���R�[�_���Z�b�g
	m_pTailMotor->setCount(0); 	//�K�����[�^�G���R�[�_���Z�b�g

	m_pTailMotor->setPWM(-10);//�K����0�̂Ƃ���܂œ������B
	m_pClock->wait(3000);//3�b�ԓ������B
	m_pTailMotor->setCount(0);//�K�����[�^�G���R�[�_���Z�b�g�i������Ԑݒ�j

	while(1) {

		m_vertical->adjust(m_speed,turn,pwm_L,pwm_R);

		if(m_pTouchSensor->isPressed()==1) {
			m_pSound->instructBeat();
			m_pClock->wait(500);
			break;
		}

		m_pClock->wait(4);
	}

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

