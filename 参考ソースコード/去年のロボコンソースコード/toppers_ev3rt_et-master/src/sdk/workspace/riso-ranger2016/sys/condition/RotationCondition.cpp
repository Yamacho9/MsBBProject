///////////////////////////////////////////////////////////
//  RotationCondition.cpp
//  Implementation of the Class RotationCondition
//  Created on:      2016/08/03 12:28:35
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "RotationCondition.h"


RotationCondition::RotationCondition(int rotationDifference){
	m_RotationDifference = rotationDifference;
}



RotationCondition::~RotationCondition(){

}



void RotationCondition::onStart(){

	
	m_StartRotation = m_StateObserver->GetDirection();
}


bool RotationCondition::check(){

	bool ret = false;
	
	if(m_RotationDifference < 0 &&  m_StateObserver->GetDirection() <= m_StartRotation + m_RotationDifference){
		ret = true;	
	}
	else if(0 <= m_RotationDifference &&  m_StartRotation + m_RotationDifference <= m_StateObserver->GetDirection() ){
		ret = true;	
	}
	
	return ret;
}
