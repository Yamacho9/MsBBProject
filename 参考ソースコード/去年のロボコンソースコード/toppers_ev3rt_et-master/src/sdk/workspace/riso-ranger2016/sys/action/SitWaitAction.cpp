///////////////////////////////////////////////////////////
//  SitWaitAction.cpp
//  Implementation of the Class SitWaitAction
//  Created on:      2016/08/01 16:33:15
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "SitWaitAction.h"


SitWaitAction::SitWaitAction(int tailAngle){

	// メンバ初期化
	m_TailAngle = tailAngle;	// 尻尾角度[deg]

}



SitWaitAction::~SitWaitAction(){

}




void SitWaitAction::onStart(){

	// 倒立制御オフ
	m_WheelControl->SetTwoWheelMode(false);

	// 前進量,旋回量を設定
	m_WheelControl->SetRefValue(0, 0);

	// 尻尾角度を設定
	m_TailControl->SetRefValue(m_TailAngle);

}


void SitWaitAction::onCycle(){

}


void SitWaitAction::onStop(){

}
