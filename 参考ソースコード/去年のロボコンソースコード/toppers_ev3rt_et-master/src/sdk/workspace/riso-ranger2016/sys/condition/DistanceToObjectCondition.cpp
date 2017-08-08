/*
 * DistanceToObjectCondition.cpp
 *
 *  Created on: 2016/08/12
 *      Author: Reina
 */

#include "DistanceToObjectCondition.h"


/**
 * 障害物までの距離[cm]を指定
 */
DistanceToObjectCondition::DistanceToObjectCondition(int distance){
	m_Distance = distance;
}



DistanceToObjectCondition::~DistanceToObjectCondition(){

}



void DistanceToObjectCondition::onStart(){

}


bool DistanceToObjectCondition::check(){

	bool ret = false;

	// 条件判定
	if(m_ultrasonicControl->getUltrasonicValue() <= m_Distance){
		ret = true;
		
		printf("distance = %d\n\r",m_ultrasonicControl->getUltrasonicValue());
	
	}
	
/*	
	if(m_DistanceDifference < 0 &&  m_ultrasonicControl->getUltrasonicValue() <= m_StartDistance + m_DistanceDifference) {			// 減少方向
		ret = true;
		printf("test %d\n\n\n\n\n\n",m_ultrasonicControl->getUltrasonicValue());

	}
	else if(0 <= m_DistanceDifference &&  m_StartDistance + m_DistanceDifference <= m_ultrasonicControl->getUltrasonicValue()) {	// 増加方向

		
		ret = true;
		printf("test %d\n\n\n\n\n\n",m_ultrasonicControl->getUltrasonicValue());
	

	}
*/
	
	return ret;
}
