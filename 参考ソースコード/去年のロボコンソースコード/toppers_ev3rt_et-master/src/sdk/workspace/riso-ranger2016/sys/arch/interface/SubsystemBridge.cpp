///////////////////////////////////////////////////////////
//  SubsystemBridge.cpp
//  Implementation of the Class SubsystemBridge
//  Created on:      2016/04/19 20:14:30
//  Original author: 037789
///////////////////////////////////////////////////////////

#include <string.h>
#include "RTOS.h"
#include "SubsystemBridge.h"




SubsystemBridge::SubsystemBridge(uint16 rv_subsys_id, sint32 rv_mbox_id)
    : subsys_id(rv_subsys_id),
      mbox_id(rv_mbox_id)
{

}


SubsystemBridge::~SubsystemBridge(){

}


void SubsystemBridge::doInitialize(){

}


void SubsystemBridge::receiveMessage(uint16* rv_message){
    uint16 at_msg_code = *rv_message; // 先頭はメッセージコード.

    actReceiveMessage(at_msg_code, rv_message + 1);
}


void SubsystemBridge::sendMessage(uint16 rv_msg_code, void* rv_message, uint16 rv_size){
    // メッセージ領域の確保.
    uint16* at_msg = new uint16[(rv_size / 2) + 4];
    uint32 i = 0;

    at_msg[i++] = subsys_id;
    at_msg[i++] = rv_msg_code;

    if(rv_size > 0) {
        memcpy(static_cast<void *>(at_msg + i), rv_message, rv_size);
    }

    RTOS::sndMessage(mbox_id, at_msg);
}


void SubsystemBridge::actInitialize(){

}


void SubsystemBridge::actReceiveMessage(uint16 rv_msg_code, uint16* rv_message){

}
