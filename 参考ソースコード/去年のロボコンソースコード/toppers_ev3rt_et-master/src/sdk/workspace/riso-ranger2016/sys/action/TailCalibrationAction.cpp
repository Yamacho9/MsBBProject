///////////////////////////////////////////////////////////
//  TailCalibrationAction.cpp
//  Implementation of the Class TailCalibrationAction
//  Created on:      2016/09/12 16:01:03
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "TailCalibrationAction.h"


TailCalibrationAction::TailCalibrationAction(){

	// メンバ初期化
	m_TimeCount = 0;
	m_RefTailAngle = 90;

}



TailCalibrationAction::~TailCalibrationAction(){

}





void TailCalibrationAction::onStart(){

	// まず尻尾を適当に中間あたりの角度まで移動させる

}


void TailCalibrationAction::onCycle(){

	// 一定時間後から一定速度で0[deg]方向に動かしていく
	if(250 < m_TimeCount) {				// todo マジックナンバー 周期と連動するように
		m_RefTailAngle -= 0.16;			// todo マジックナンバー 周期と連動するように
	}
	// 尻尾に目標値設定
	m_TailControl->SetRefValue(m_RefTailAngle);

	// 時間を進める
	m_TimeCount++;

}


void TailCalibrationAction::onStop(){

	// 現在位置を0[deg]の位置とする
	m_TailControl->init();
}
