///////////////////////////////////////////////////////////
//  RotateAction.cpp
//  Implementation of the Class RotateAction
//  Created on:      2016/07/27 11:47:53
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "RotateAction.h"
#include "Bluetooth.h"


RotateAction::RotateAction(int turn){
	m_Turn = turn;
}


RotateAction::~RotateAction(){

}



void RotateAction::onStart(){

	// 前進量,旋回量を設定
	m_WheelControl->SetRefValue(0, m_Turn);

}


void RotateAction::onCycle(){

}


void RotateAction::onStop(){

}
