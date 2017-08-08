///////////////////////////////////////////////////////////
//  DistanceCondition.cpp
//  Implementation of the Class DistanceCondition
//  Created on:      2016/08/03 12:28:33
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "DistanceCondition.h"


/**
 * 位置変分指定
 */
DistanceCondition::DistanceCondition(int distanceDifference){
	m_DistanceDifference = distanceDifference;
}



DistanceCondition::~DistanceCondition(){

}



void DistanceCondition::onStart(){

	// 基準距離取得
	m_StartDistance = m_StateObserver->GetRunningDistance();
}


bool DistanceCondition::check(){

	bool ret = false;

	// 条件判定
	if(m_DistanceDifference < 0 &&  m_StateObserver->GetRunningDistance() <= m_StartDistance + m_DistanceDifference) {			// 減少方向
		ret = true;
	}
	else if(0 <= m_DistanceDifference &&  m_StartDistance + m_DistanceDifference <= m_StateObserver->GetRunningDistance()) {	// 増加方向
		ret = true;
	}

	return ret;
}
