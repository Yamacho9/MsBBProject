///////////////////////////////////////////////////////////
//  Condition.cpp
//  Implementation of the Class Condition
//  Created on:      2016/08/03 12:28:24
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "Condition.h"


StateObserver* Condition::m_StateObserver;
UltrasonicControl* Condition::m_ultrasonicControl;

Condition::Condition(){

}



Condition::~Condition(){

}


void Condition::init(StateObserver *stateObserver, UltrasonicControl *ultrasonicControl){
	Condition::m_StateObserver = stateObserver;
	Condition::m_ultrasonicControl = ultrasonicControl;
}


