///////////////////////////////////////////////////////////
//  WheelControl.cpp
//  Implementation of the Class WheelControl
//  Created on:      2016/07/15 16:04:13
//  Original author: 039889
///////////////////////////////////////////////////////////

#include "WheelControl.h"
#include "balancer.h"
#include "Bluetooth.h"


WheelControl::WheelControl(Motor* wheelMotorL, Motor* wheelMotorR, Battery* battery, GyroSensor* gyroSensor){
	m_WheelMotorL = wheelMotorL;
	m_WheelMotorR = wheelMotorR;
	m_Battery = battery;
	m_GyroSensor = gyroSensor;
	m_RefSpeed = 0;
	m_RefTurn = 0;
	m_TwoWheelMode = false;
}


WheelControl::~WheelControl(){

}


void WheelControl::Init(){

	// モータエンコーダをリセットする
	m_WheelMotorL->ResetEnc();
	m_WheelMotorR->ResetEnc();

    // 倒立振子制御初期化
	balance_init();
}


// 4ms周期実行 要リアルタイム性
void WheelControl::Control(){

	signed char pwm_L;
	signed char pwm_R;

	if(m_TwoWheelMode == true) {	// 倒立走行
		// Duty比算出
		balance_control(
			(float)m_RefSpeed,
			(float)m_RefTurn,
			(float)m_GyroSensor->getSensorValue(),
			(float)m_GyroSensor->getGyroOffset(),
			(float)m_WheelMotorL->GetEnc(),
			(float)m_WheelMotorR->GetEnc(),
			(float)m_Battery->getValue(),
			&pwm_L,
			&pwm_R);

	}
	else {							// 尻尾走行

		// 範囲チェック＆丸め込み
		int pwm_L_temp = m_RefSpeed + m_RefTurn*1.0;
		int pwm_R_temp = m_RefSpeed - m_RefTurn*1.0;
//		int pwm_L_temp = 0;
//		int pwm_R_temp = 0;

		if(pwm_L_temp < -100) {
			pwm_L = -100;
		}
		else if(100 < pwm_L_temp){
			pwm_L = 100;
		}
		else {
			pwm_L = pwm_L_temp;
		}
		if(pwm_R_temp < -100) {
			pwm_R = -100;
		}
		else if(100 < pwm_R_temp) {
			pwm_R = 100;
		}
		else {
			pwm_R = pwm_R_temp;
		}

	}

	// Duty比設定
	//fprintf(btlog,"currentLinePosition = %10f\r\n",(int)pwm_L, (int)pwm_R);
	m_WheelMotorL->SetPWMValue((int)pwm_L);
	m_WheelMotorR->SetPWMValue((int)pwm_R);

}


//TODO 同時変更必要 現状では非スレッドセーフ
void WheelControl::SetRefValue(int speed, int turn){
	m_RefSpeed = speed;
	m_RefTurn = turn;
}


/**
 * 倒立走行と尻尾走行の切り替え
 */
void WheelControl::SetTwoWheelMode(bool onoff){

	if(m_TwoWheelMode == false && onoff == true) {
	    // 倒立振子制御初期化
	//	m_GyroSensor->reset();
	    //m_WheelMotorL->ResetEnc();  //TODO 不要の可能性がある 倒立移行時に失敗するならここを実行
		//m_WheelMotorR->ResetEnc();
		balance_init();
	}

	m_TwoWheelMode = onoff;
}
