///////////////////////////////////////////////////////////
//  RTOS.cpp
//  Implementation of the Class RTOS
//  Created on:      2016/04/13 12:52:15
//  Original author: 037789
///////////////////////////////////////////////////////////
#include <kernel.h>

#include "ev3api.h"
#include "RTOS.h"


// 静的メンバ属性の初期化(起動時に初期化される).
uint16 RTOS::lockcpu_count = 0;

RTOS::RTOS(){
    // インスタンスとしては使用しない想定なので空のままにしておく.
}



RTOS::~RTOS(){

}




void RTOS::startTask(uint16 rv_task_id, uint16 rm_prm){
    // タスクを起動する.
    // 引数 rv_prm は TOPPERS 新世代カーネルでは使用しない.
    act_tsk(rv_task_id);
}


void RTOS::waiTask(uint16 rv_timeout_cnt){
    tslp_tsk(rv_timeout_cnt);
}


void RTOS::sndMessage(uint16 rv_mbx_id, void* rv_message){
    // 非同期メッセージ送信に使用する.
    //   OS のデータキューを利用.

    ER at_err = psnd_dtq(rv_mbx_id, reinterpret_cast<intptr_t>(rv_message));

    if(at_err != E_OK) {
        // 送信エラー発生時
        // T.B.D.
    }
    else {
        // Don't care...
    }
}


void RTOS::intrSndMessage(uint16 rv_mbx_id, void* rv_message){
    // 非同期メッセージ送信に使用する.
    //   OS のデータキューを利用.

    ER at_err = ipsnd_dtq(rv_mbx_id, reinterpret_cast<intptr_t>(rv_message));

    if(at_err != E_OK) {
        // 送信エラー発生時
        // T.B.D.
    }
    else {
        // Don't care...
    }
}


void RTOS::rcvMessage(void** rv_message, uint16 rv_mbx_id){
    // 非同期メッセージ受信.
    //   OS のデータキューを利用.

    ER at_err = rcv_dtq(rv_mbx_id, reinterpret_cast<intptr_t *>(rv_message));

    if(at_err != E_OK) {
        // 受信エラー発生時.
    }
    else {
        // Don't care...
    }
}


void RTOS::waiSemaphore(uint16 rv_sem_id){
    wai_sem(rv_sem_id);
}


void RTOS::sigSemaphore(uint16 rv_sem_id){
    sig_sem(rv_sem_id);
}


void RTOS::intrSetFlag(uint16 rv_flg_id, uint32 rv_flg_pattern){
    iset_flg(rv_flg_id, rv_flg_pattern);
}


uint32 RTOS::waiFlag(uint16 rv_flg_id){

    FLGPTN at_flg_pattern = 0;

    wai_flg(rv_flg_id, 0xFFFFFFFF, TWF_ORW, &at_flg_pattern);

    return  static_cast<uint32>(at_flg_pattern);
}


void RTOS::lockCpu(){
    loc_cpu();
    ++lockcpu_count;

}


void RTOS::unlockCpu(){
    --lockcpu_count;

    if(lockcpu_count <= 0) {
        lockcpu_count = 0;
        unl_cpu();
    }
}


void RTOS::startCyclicHandler(uint16 rv_cyc_id){

    ER at_err = ev3_sta_cyc(rv_cyc_id);

    if(at_err != E_OK) {

    }
    else {
        // Don't care...
    }
}



void RTOS::stopCyclicHandler(uint16 rv_cyc_id){
    ER at_err = ev3_stp_cyc(rv_cyc_id);

    if(at_err != E_OK) {
    }
    else {
        // Don't care...
    }
}
