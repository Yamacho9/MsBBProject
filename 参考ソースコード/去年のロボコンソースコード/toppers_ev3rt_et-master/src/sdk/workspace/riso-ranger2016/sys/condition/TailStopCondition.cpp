///////////////////////////////////////////////////////////
//  TailStopCondition.cpp
//  Implementation of the Class TailStopCondition
//  Created on:      2016/09/06 21:01:25
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "TailStopCondition.h"


TailStopCondition::TailStopCondition(int targetStableTime){

	// メンバ初期化
	m_stableTime = 0;
	m_targetStableTime = targetStableTime;
}



TailStopCondition::~TailStopCondition(){

}


void TailStopCondition::onStart(){

}


bool TailStopCondition::check(){

	bool ret = false;

	// 尻尾停止時間計測
	if(m_StateObserver->GetTailAngularVelocity() == 0) {
		m_stableTime += 4;	//TODO 4ms周期 4の取得場所
	}
	else {
		m_stableTime = 0;
	}

	// 目標停止時間判定
	if(m_targetStableTime < m_stableTime) {
		ret = true;
	}

	return ret;
}
