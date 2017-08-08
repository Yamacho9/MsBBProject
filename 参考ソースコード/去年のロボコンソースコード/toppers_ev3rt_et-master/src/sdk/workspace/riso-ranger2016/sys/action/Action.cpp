///////////////////////////////////////////////////////////
//  Action.cpp
//  Implementation of the Class Action
//  Created on:      2016/07/27 11:47:44
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "Action.h"


StateObserver* Action::m_StateObserver = NULL;
TailControl* Action::m_TailControl = NULL;
WheelControl* Action::m_WheelControl = NULL;

Action::Action(){

}


Action::~Action(){

}


void Action::init(StateObserver* stateObserver, TailControl* tailControl, WheelControl* wheelControl){
	m_StateObserver = stateObserver;
	m_TailControl = tailControl;
	m_WheelControl = wheelControl;

}

void Action::EmergencyStop(){

	// 尻尾、左右車輪を停止
	m_TailControl->SetRefValue(90);
	m_WheelControl->SetTwoWheelMode(false);
	m_WheelControl->SetRefValue(0, 0);

}
