///////////////////////////////////////////////////////////
//  LineTraceAction.cpp
//  Implementation of the Class LineTraceAction
//  Created on:      2016/07/27 11:47:53
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "LineTraceAction.h"
#include "Bluetooth.h"


LineTraceAction::LineTraceAction(int speed, int tailAngle, float referenceLinePosition, PIDControl* pidControl){

	// メンバ初期化
	m_Speed = speed;									// 走行速度設定
	m_TailAngle = tailAngle;							// 尻尾角度設定
	m_PidControl = pidControl;							// PIDパタメータ設定
	m_ReferenceLinePosition = referenceLinePosition;	// トレース位置設定
}


LineTraceAction::~LineTraceAction(){

	// PIDインスタンス削除
	delete m_PidControl;

}



void LineTraceAction::onStart(){

	// 倒立モードに切り替え
	m_WheelControl->SetTwoWheelMode(true);

}


void LineTraceAction::onCycle(){

	int turn = 0;
	float currentLinePosition = 0;

	// 現在のライン位置を取得
	currentLinePosition = m_StateObserver->GetLinePosition();

	// 旋回量を算出
	turn = m_PidControl->PIDCalculation(m_ReferenceLinePosition, currentLinePosition);
//	turn =0 ;


	// 前進量,旋回量を設定
	m_WheelControl->SetRefValue(m_Speed, turn);

	// 尻尾角度を設定
	m_TailControl->SetRefValue(m_TailAngle);

}


void LineTraceAction::onStop(){

}
