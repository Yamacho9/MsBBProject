#ifndef UI_H
#define UI_H

#include "RunMethod/Vertical.h"
#include "Communication/RemoteStart.h"
#include "SubFunction/Calibration.h"
#include "SubFunction/Sound.h"
#include "TouchSensor.h"
#include "Clock.h"
#include "Motor.h"
#include "Nxt.h"
#include "Bluetooth.h"
#include "Lcd.h"

using namespace ecrobot;

class Calibration;

class UI
{
private:

	// Bluetooth通信用パスキー
	const char* PASS_KEY;

	//デバイスネーム
	const char* DEVICE_NAME;

	RemoteStart* m_remotestart;
	Calibration* m_pCalibration;
	Sound* m_pSound;
	TouchSensor* m_pTouchSensor;
	Clock* m_pClock;
	Motor* m_pTailMotor;
	Motor* m_pLeftMotor;
	Motor* m_pRightMotor;
	Nxt* m_pNxt;
	Bluetooth* m_pBluetooth;
	Lcd* m_pLcd;


	//属性として定義してあるけど別のクラスでは使いません。
	static int m_sound_flag;
	Posture* m_vertical;

public:

	void update();
	void initialize();
	void prepareRun();
	void setSound();
	void tailBalance();

	UI(Posture* v,
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
	   Lcd* pLcd
	  );

	virtual ~UI();

};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

