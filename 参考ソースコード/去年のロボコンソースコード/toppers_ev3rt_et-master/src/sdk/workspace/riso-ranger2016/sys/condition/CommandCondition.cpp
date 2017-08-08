///////////////////////////////////////////////////////////
//  CommandCondition.cpp
//  Implementation of the Class CommandCondition
//  Created on:      2016/08/03 12:55:29
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "Bluetooth.h"
#include "CommandCondition.h"


CommandCondition::CommandCondition(){

}



CommandCondition::~CommandCondition(){

}



void CommandCondition::onStart(){

}


bool CommandCondition::check(){

	return isStartSignal();
}
