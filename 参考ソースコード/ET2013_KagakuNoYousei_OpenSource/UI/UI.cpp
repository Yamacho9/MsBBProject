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
#include "balancer.h"	//倒立振子制御用ヘッダファイル
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
	: PASS_KEY("1234"),	//パスキーの設定　適切に変更してください
	  DEVICE_NAME("ET00"),	//デバイスネームの設定　適切に変更してください
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

	//デバイスネーム
	m_pBluetooth->setFriendlyName(DEVICE_NAME);

	m_pLcd->clear();// LCD表示する
	m_pLcd->cursor(0, 0);
	m_pLcd->putf("snsnsn", "Connect by BT", "or", "Press RUN button");// "Connect by BT" "or" "Press RUN button"
	m_pLcd->disp();
	// Bluetooth通信接続 or RUNボタン押下 までループする
	while(true) {
		if(m_pBluetooth->waitForConnection(PASS_KEY, 10) == true) {
			break;						// Bluetooth通信接続済み ⇒ ループを抜ける
		}
		if(m_pNxt->getButtons() & Nxt::RUN_ON == true) {
			break;						// RUNボタン押下済み ⇒ ループを抜ける
		}

		m_pClock->wait(10);				// 10msec待つ
	}

	m_pCalibration->calibrate();
}


void UI::prepareRun() {

	signed char turn = 0;     //旋回命令
	signed char pwm_L, pwm_R; //左右モータPWM出力
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


	signed char turn = 0;  //旋回命令
	signed char pwm_L = 0;
	signed char pwm_R = 0; //左右モータPWM出力
	int m_speed = 0;


	while(m_pTouchSensor->isPressed()==true) { //押されるまで待つ
	}

	m_pLeftMotor->setCount(0); 	//左モータエンコーダリセット
	m_pRightMotor->setCount(0);	//右モータエンコーダリセット
	m_pTailMotor->setCount(0); 	//尻尾モータエンコーダリセット

	m_pTailMotor->setPWM(-10);//尻尾を0のところまで動かす。
	m_pClock->wait(3000);//3秒間動かす。
	m_pTailMotor->setCount(0);//尻尾モータエンコーダリセット（初期状態設定）

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

