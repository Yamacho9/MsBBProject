///////////////////////////////////////////////////////////
//  HighPriority.cpp
//  Implementation of the Class HighPriority
//  Created on:      2016/05/24 11:33:05
//  Original author: 037789
///////////////////////////////////////////////////////////

#include "Task.h"
#include "HighPriority.h"


HighPriority::HighPriority(sint32 rv_tsk_id, sint32 rv_mbox_id)
    : Task(rv_tsk_id, rv_mbox_id)
{

}



HighPriority::~HighPriority(){

}





void HighPriority::start(){

}


void HighPriority::loop(){

}


bool HighPriority::isOwnMessage(){

	return false;
}
