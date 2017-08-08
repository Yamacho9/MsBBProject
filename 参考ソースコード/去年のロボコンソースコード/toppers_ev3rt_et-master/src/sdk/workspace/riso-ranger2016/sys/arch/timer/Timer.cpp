///////////////////////////////////////////////////////////
//  Timer.cpp
//  Implementation of the Class Timer
//  Created on:      2016/04/28 14:48:15
//  Original author: 037789
///////////////////////////////////////////////////////////

#include "RTOS.h"
#include "Timer.h"
#include "TimerEvent.h"


Timer* Timer::instance = NULL;

Timer::Timer() : the_event(NULL), is_inuse(false) {
    // 配列メンバの初期化.
    for(uint8 i = 0; i < kMaxTimerId; i++) {
        time_count[i] = 0;
    }

    absoluteTime = 0;
}



Timer::~Timer(){

}





Timer* Timer::getInstance(){

    if(instance == NULL) {
        instance = new Timer();
    }
    else {
    }
    return  instance;
}


void Timer::tick(){

    for(uint8 i = 0; i < kMaxTimerId; i++) {

        if(time_count[i] > 0) {
            // デクリメント結果でカウント 0 となったらタイムアップ通知.
            if(--time_count[i] == 0) {
                the_event->ntfTimeOut(static_cast<EmTimerId>(i));
                stopTimer(static_cast<EmTimerId>(i));
            }
        }
        else {
            // Don't care.
        }
    }

    absoluteTime++;
}

void Timer::startTimer(uint32 rv_time, EmTimerId rv_id) {
    // カウントを設定してタイマの起動を行なう.
    time_count[rv_id] = rv_time;

    if(is_inuse == false) {
        // 1つ目のタイマ開始ならタイマソーソを起動.
        RTOS::stopCyclicHandler(ID_EV3CYC_1MS);
        is_inuse = true;
    }
}


void Timer::stopTimer(EmTimerId rv_id) {

   bool at_is_timer_inuse = false;
   // カウント 0 にしてタイムアウト通知をキャンセルする.
   time_count[rv_id] = 0;

   if(rv_id == kTempTimer) {
       time_count[kTempTimer] = 4;
   }

   for(uint8 i = 0; i < kMaxTimerId; i++ ) {
       if (time_count[i] != 0) {
           at_is_timer_inuse = true;
           break;
       }
   }

   if(at_is_timer_inuse == false) {
       // 全てのタイマが停止されたらしたらタイマソースを OFF する.
       RTOS::stopCyclicHandler(ID_EV3CYC_1MS);
       is_inuse = false;
   }
}

