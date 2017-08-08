///////////////////////////////////////////////////////////
//  Motor.cpp
//  Implementation of the Class Motor
//  Created on:      2016/07/26 18:58:12
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "Motor.h"
#include "Bluetooth.h"

Motor::Motor(motor_port_t PortNum){

	// メンバ初期化
	m_AngularVelocity = 0;
	m_MotorEncBuf = new int[26];
	for(int i=0; i<26; i++) this->m_MotorEncBuf[i] = 0;
	m_MotorEncBufNextIndex = 0;
	m_Port = PortNum;

	// モータタイプ設定
	ev3_motor_config(m_Port, LARGE_MOTOR);

	// ブレーキモード解除
	ev3_motor_stop(m_Port, false);

}


Motor::~Motor(){

}


/**
 * 角速度[deg/sec]計算
 * 4ms周期で呼び出しが必要
 */
void Motor::UpdateAngularVelocity(){

	// 角速度[deg/sec]を更新（0.1sec間の角度degの差から角速度[deg/sec]を計算）
	m_MotorEncBuf[m_MotorEncBufNextIndex] = GetEnc();
	m_AngularVelocity = (m_MotorEncBuf[m_MotorEncBufNextIndex] - m_MotorEncBuf[(m_MotorEncBufNextIndex+1)%26]) * 10;	//TODO 呼び出し周期により計算調整が必要
	m_MotorEncBufNextIndex++;
	if(26 <= m_MotorEncBufNextIndex) {
		m_MotorEncBufNextIndex = 0;
	}

}


/**
 * エンコーダリセット
 * TODO 実行後0.１秒間、角速度が０になる
 */
void Motor::ResetEnc(){

	// エンコーダリセット
    ev3_motor_reset_counts(m_Port);

	// 角速度関連メンバを初期化
	m_AngularVelocity = 0;
	m_MotorEncBuf = new int[26];
	for(int i=0; i<26; i++) this->m_MotorEncBuf[i] = 0;
	m_MotorEncBufNextIndex = 0;

}


/**
 * エンコーダ値を取得
 */
int Motor::GetEnc(){
	return ev3_motor_get_counts(m_Port);
}


/**
 * 角速度[deg/sec]を取得
 */
int Motor::GetAngularVelocity(){
	return m_AngularVelocity;
}


void Motor::SetPWMValue(int PWMValue){

	// 範囲チェック
	if(PWMValue < -100) {
		PWMValue = -100;
	}
	else if(100 < PWMValue) {
		PWMValue = 100;
	}

	// PWM設定
	ev3_motor_set_power(m_Port, (int)PWMValue);

}
