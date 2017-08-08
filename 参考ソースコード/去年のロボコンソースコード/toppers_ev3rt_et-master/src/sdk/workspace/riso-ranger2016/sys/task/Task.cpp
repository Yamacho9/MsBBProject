///////////////////////////////////////////////////////////
//  Task.cpp
//  Implementation of the Class Task
//  Created on:      2016/04/28 11:49:10
//  Original author: 037789
///////////////////////////////////////////////////////////

#include "RTOS.h"
#include "SubsystemBridge.h"
#include "Task.h"


Task::Task(sint32 rv_tsk_id, sint32 rv_mbox_id)
    : task_id(rv_tsk_id),
      mbox_id(rv_mbox_id)
{

}



Task::~Task(){

}




// 受信したメッセージをサブシステムに分配.
void Task::loop(){
    uint16* at_msg = NULL;

    for(;;) {
        RTOS::rcvMessage(reinterpret_cast<void **>(&at_msg), mbox_id);

        EmSubsystemId at_subsys_id = static_cast<EmSubsystemId>(*at_msg); // 先頭 2Byte は宛先 SubSystem.
        uint16 * at_msg_data = at_msg + 1; // データ

        // SubSystem が配下に存在すればメッセージを投げる.
        if(isOwnMessage(at_subsys_id) == true) {
            the_subsystem_bridge[at_subsys_id]->receiveMessage(at_msg_data);
        }

        // メッセージ領域の開放.
        delete[] at_msg;
    }
}


void Task::start(){

}



bool Task::isOwnMessage(EmSubsystemId rv_subsys_id){

	return false;
}
