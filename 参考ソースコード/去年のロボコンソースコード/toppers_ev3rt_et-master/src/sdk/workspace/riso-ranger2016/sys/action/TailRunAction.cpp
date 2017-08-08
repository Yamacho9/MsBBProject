/*
 * TailRunAction.cpp
 *
 *  Created on: 2016/08/11
 *      Author: ryuji
 */

#include "TailRunAction.h"

TailRunAction::TailRunAction(int speed, int turn) {

	// メンバ初期化
	m_Speed = speed;
	m_Turn = turn;

}

TailRunAction::~TailRunAction() {

}



void TailRunAction::onStart(){

	// 走行速度と旋回速度を設定
	m_WheelControl->SetRefValue(m_Speed, m_Turn);

	// 尻尾角度は現状維持

}


void TailRunAction::onCycle(){

}


void TailRunAction::onStop(){

}
