///////////////////////////////////////////////////////////
//  TailControl.cpp
//  Implementation of the Class TailControl
//  Created on:      2016/07/21 20:55:36
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "TailControl.h"


TailControl::TailControl(Motor* tailMotor, PIDControl* pidControl){

	m_TailMotor = tailMotor;
        m_PIDControl = pidControl;
	m_RefAngle = 0;

}



TailControl::~TailControl(){

}



void TailControl::init() {
	// エンコーダ初期化
	m_TailMotor->ResetEnc();
}

void TailControl::Control(){

	int pwm;

	// Duty比算出
	pwm = (m_RefAngle - m_TailMotor->GetEnc())*1.5; // 比例制御
	// Duty比設定
	m_TailMotor->SetPWMValue(pwm);

}


bool TailControl::earthingDetection(){

	// 比例制御で一定以上の誤差があるとき接地していると判断
	if(m_RefAngle - m_TailMotor->GetEnc() < TailControl::c_EarthingThreshold) {
		return false;
	}
	else {
		return true;
	}

}


void TailControl::SetRefValue(int angle){
	m_RefAngle = angle;
}
