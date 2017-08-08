///////////////////////////////////////////////////////////
//  system_task.cpp
//  Implementation of the Class system_task
//  Created on:      2016/04/28 11:21:26
//  Original author: 037789
///////////////////////////////////////////////////////////

#include "ev3api.h"
#include "Main.h"
#include "HighPriority.h"
#include "Engine.h"
#include "Timer.h"
#include "app.h"

/* LCDフォントサイズ コンパイル通し用暫定*/
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

void TaskMain(intptr_t unused){
	ev3_led_set_color(LED_ORANGE);
    Main::getInstance()->start();
}

void TaskHighPriority(intptr_t unused){
//  HighPriority *at_high_priority_task = new HighPriority(ID_TASK_HIGH_PRIORITY, ID_MSG_BOX_HIGH_PRIORITY);
    HighPriority* at_task = static_cast<HighPriority *>(Main::getInstance()->get_the_task(kTaskHighPriority));

    if(at_task != NULL) {
        at_task->start();
    }
    else {
        ev3_lcd_draw_string("SYS::ERROR:: HIGH PRIORITY TASK OBJECT IS NOT EXSIST.", 0, CALIB_FONT_HEIGHT*1);
    }
}


void TaskEngine(intptr_t unused){
    Engine *at_task = static_cast<Engine *>(Main::getInstance()->get_the_task(kTaskEngine));

    if(at_task != NULL) {
        at_task->start();
    }
    else {
        ev3_lcd_draw_string("SYS::ERROR:: ENGINE TASK OBJECT IS NOT EXSIST.", 0, CALIB_FONT_HEIGHT*1);
    }
}

void TaskDebugConsole(intptr_t unused){
    Engine *at_task = static_cast<Engine *>(Main::getInstance()->get_the_task(kTaskDebugConsole));

    if(at_task != NULL) {
        at_task->start();
    }
    else {
        ev3_lcd_draw_string("SYS::ERROR:: DEBUG CONSOLE OBJECT IS NOT EXSIST.", 0, CALIB_FONT_HEIGHT*1);
    }
}

void Cyc1msecInterval(intptr_t exinf) {
    // ID_EV3CYC_1MS サイクリックハンドラ.
      // タイマ周期は OS コンフィグによる.
    Timer* at_obj = Timer::getInstance();

    at_obj->tick();
}


system_task::system_task(){

}



system_task::~system_task(){

}
