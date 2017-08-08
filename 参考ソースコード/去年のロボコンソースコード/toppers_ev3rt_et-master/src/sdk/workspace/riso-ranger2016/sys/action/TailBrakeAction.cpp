///////////////////////////////////////////////////////////
//  TailBrakeAction.cpp
//  Implementation of the Class TailBrakeAction
//  Created on:      2016/08/09 20:55:39
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "TailBrakeAction.h"


TailBrakeAction::TailBrakeAction(){

}



TailBrakeAction::~TailBrakeAction(){

}





void TailBrakeAction::onStart(){

	m_WheelControl->SetTwoWheelMode(false);
	m_TailControl->SetRefValue(80);
	m_WheelControl->SetRefValue(100, 0);

}


void TailBrakeAction::onCycle(){

	// 尻尾接地時はブレーキ 非接地時はフル前進で後方慣性力をかける
	if(m_TailControl->earthingDetection() == true) {
		m_WheelControl->SetRefValue(0, 0);
	}
	else{
		m_WheelControl->SetRefValue(100, 0);
//		m_WheelControl->SetRefValue(40, 0);
	}

}


void TailBrakeAction::onStop(){

	// 停止状態にする
	m_WheelControl->SetRefValue(0, 0);

}
