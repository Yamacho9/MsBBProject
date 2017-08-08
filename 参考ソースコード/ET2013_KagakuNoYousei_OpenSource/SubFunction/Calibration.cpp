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
	//黒色の輝度を取得・画面表示
	m_pLcd->clear();						//画面の初期化
	m_pLcd->cursor(0,0);					//出力位置の指定
	m_pLcd->putf("s","Get color BLACK");	//出力文字列
	m_pLcd->disp();							//画面に出力
	m_black = calBrightness();				//黒色の輝度を取得
	m_pLcd->cursor(2,1);
	m_pLcd->putf("d",m_black,0);			//出力値　黒色輝度

	// ↓ジャイロオフセット値確認用　機体を静止させた状態で取得できます。これはキャリブレーションではないので注意
	m_pLcd->cursor(2,7);
	int gyro;
	gyro =m_pGyroSensor->get();
	m_pLcd->putf("d",gyro,0);
	// ↑ここまで ******************************

	m_pLcd->disp();


	//白色の輝度を取得・画面表示
	m_pLcd->cursor(0,2);
	m_pLcd->putf("s","Get color WHITE");
	m_pLcd->disp();
	m_white = calBrightness();				//白色の輝度を取得
	m_pLcd->cursor(2,3);
	m_pLcd->putf("d",m_white,0);
	m_pLcd->disp();


	//灰色の輝度を取得・画面表示
	m_pLcd->cursor(0,4);
	m_pLcd->putf("s","Get color GRAY");
	m_pLcd->disp();
	m_gray = calBrightness(); 				//灰色の輝度を取得
	m_pLcd->cursor(2,5);
	m_pLcd->putf("d",m_gray,0);
	m_pLcd->disp();


	//境界線輝度を算出
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

