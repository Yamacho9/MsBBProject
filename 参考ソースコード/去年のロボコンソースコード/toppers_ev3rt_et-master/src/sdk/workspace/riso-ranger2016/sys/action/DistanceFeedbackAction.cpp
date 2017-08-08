///////////////////////////////////////////////////////////
//  DistanceFeedbackAction.cpp
//  Implementation of the Class DistanceFeedbackAction
//  Created on:      2016/07/27 11:47:53
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "DistanceFeedbackAction.h"
#include "Bluetooth.h"


DistanceFeedbackAction::DistanceFeedbackAction(int referenceDistance){

	// メンバ初期化
	m_ReferenceDistance = referenceDistance;
	m_PIDContol = new PIDControl(0.4, 0, 0);
}


DistanceFeedbackAction::~DistanceFeedbackAction(){

	delete m_PIDContol;

}



void DistanceFeedbackAction::onStart(){

	// 起動時方位角に指定角度を足した値を目標方位角に設定
	m_ReferenceDistance += m_StateObserver->GetRunningDistance();

}


void DistanceFeedbackAction::onCycle(){

	int speed;

	// 走行速度を算出
	speed = m_PIDContol->PIDCalculation(m_ReferenceDistance, m_StateObserver->GetRunningDistance());

	// 前進量,旋回量を設定
	m_WheelControl->SetRefValue(speed, 0);

}


void DistanceFeedbackAction::onStop(){

}
