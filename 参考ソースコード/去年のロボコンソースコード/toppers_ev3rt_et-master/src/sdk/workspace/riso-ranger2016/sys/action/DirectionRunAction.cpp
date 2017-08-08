///////////////////////////////////////////////////////////
//  DirectionRunAction.cpp
//  Implementation of the Class DirectionRunAction
//  Created on:      2016/07/27 11:47:53
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "DirectionRunAction.h"
#include "Bluetooth.h"


DirectionRunAction::DirectionRunAction(int speed, int tailAngle, int direction){

	// メンバ初期化
	m_Speed = speed;									// 走行速度設定
	m_TailAngle = tailAngle;							// 尻尾角度設定
	m_ReferenceDirection = direction;					// 方位設定
	m_PidControl = new PIDControl(2, 0, 0);
}


DirectionRunAction::~DirectionRunAction(){

	// PIDインスタンス削除
	delete m_PidControl;

}



void DirectionRunAction::onStart(){

	// 起動時方位角に指定角度を足した値を目標方位角に設定
	m_ReferenceDirection += m_StateObserver->GetDirection();

}


void DirectionRunAction::onCycle(){

	int turn = 0;
	int currentDirection = 0;

	// 現在のライン位置を取得
	currentDirection = m_StateObserver->GetDirection();

	// 旋回量を算出
	turn = m_PidControl->PIDCalculation(m_ReferenceDirection, currentDirection);

	// 前進量,旋回量を設定
	m_WheelControl->SetRefValue(m_Speed, turn);

	// 尻尾角度を設定
	m_TailControl->SetRefValue(m_TailAngle);

}


void DirectionRunAction::onStop(){

}
