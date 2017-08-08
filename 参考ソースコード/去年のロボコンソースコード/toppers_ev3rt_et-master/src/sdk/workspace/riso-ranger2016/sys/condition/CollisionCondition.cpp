///////////////////////////////////////////////////////////
//  CollisionCondition.cpp
//  Implementation of the Class CollisionCondition
//  Created on:      2016/08/03 12:28:36
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "CollisionCondition.h"
#include "Bluetooth.h"


CollisionCondition::CollisionCondition(){
	m_Velocity = 0;
	m_VelocityPrev = 0;
}



CollisionCondition::~CollisionCondition(){

}


void CollisionCondition::onStart(){

}


bool CollisionCondition::check(){

	bool ret = false;

	// 条件判定
	if(m_StateObserver->getVelocity() < c_dif) {
		ret = true;
	}

	return ret;
}
