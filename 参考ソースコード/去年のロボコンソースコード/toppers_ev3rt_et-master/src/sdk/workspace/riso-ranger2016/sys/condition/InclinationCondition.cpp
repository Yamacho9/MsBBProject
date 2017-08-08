///////////////////////////////////////////////////////////
//  InclinationCondition.cpp
//  Implementation of the Class InclinationCondition
//  Created on:      2016/08/03 12:28:34
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "InclinationCondition.h"

/**
 * –Ú•WŒXÎw’è
 */
InclinationCondition::InclinationCondition(int inclinationTarget){
	m_InclinationTarget = inclinationTarget;
}



InclinationCondition::~InclinationCondition(){

}



void InclinationCondition::onStart(){


}


bool InclinationCondition::check(){

	bool ret = false;

	// ğŒ”»’è StateObserver::getTailAngle()
	
	if(m_InclinationTarget - c_error < m_StateObserver->getTailAngle() && m_StateObserver->getTailAngle() < m_InclinationTarget + c_error){
		ret =true;
		printf("\r\nAngle = %d\r\n\r\n", m_StateObserver->getTailAngle());
	}

	return ret;

}
