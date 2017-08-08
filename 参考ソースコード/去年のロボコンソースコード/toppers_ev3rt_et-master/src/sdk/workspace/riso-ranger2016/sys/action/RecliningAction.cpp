///////////////////////////////////////////////////////////
//  RecliningAction.cpp
//  Implementation of the Class RecliningAction
//  Created on:      2016/07/27 11:48:03
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "RecliningAction.h"

RecliningAction::RecliningAction(int angularVelocity){

	// メンバ初期化
	m_AngularVelocity = angularVelocity;
	m_CurrentReferenceAngle = 0;

}



RecliningAction::~RecliningAction(){

}



void RecliningAction::onStart(){

	// 現在の尻尾角度を目標値として初期化
	m_CurrentReferenceAngle = m_StateObserver->getTailAngle();

	// 倒立制御をオフ
	m_WheelControl->SetTwoWheelMode(false);
	
	// 走行速度を零にする
	m_WheelControl->SetRefValue(0, 0);

}


void RecliningAction::onCycle(){

	// 指定加速度で目標値を変化させる
	m_CurrentReferenceAngle += (float)m_AngularVelocity/(float)250;	//TODO 呼び出し周期をどこから取得して計算

	// 尻尾角度目標値を設定
	m_TailControl->SetRefValue(m_CurrentReferenceAngle);

}


void RecliningAction::onStop(){

}
