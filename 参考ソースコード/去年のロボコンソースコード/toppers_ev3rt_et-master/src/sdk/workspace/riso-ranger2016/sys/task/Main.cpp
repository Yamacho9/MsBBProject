///////////////////////////////////////////////////////////
//  Main.cpp
//  Implementation of the Class Main
//  Created on:      2016/04/28 11:49:13
//  Original author: 037789
///////////////////////////////////////////////////////////

#include "Timer.h"
#include "TimerEvent.h"
#include "RTOS.h"
#include "init_ev3.h"
#include "Task.h"
#include "DebugConsole.h"
#include "Engine.h"
#include "HighPriority.h"
#include "Main.h"

#include "Temp.h" //TODO 試走会暫定

Main* Main::instance = NULL;

Main::Main(sint32 rv_tsk_id, sint32 rv_mbox_id)
    : Task(rv_tsk_id, rv_mbox_id)
{
    the_task[kTaskMain] = this;
}



Main::~Main(){

}

Main* Main::getInstance() {
    if(instance == NULL) {
        instance = new Main(ID_TASK_MAIN, ID_MSG_BOX_MAIN);
    }
    else {
    }
    return  instance;
}

void Main::start(){

    // システムアーキ関連の初期化処理
    //   - タイマ生成/稼動開始
    Timer* at_the_timer = Timer::getInstance();
    TimerEvent* at_the_timer_event = new TimerEvent();

    // 関連張り.
    at_the_timer->set_the_event(at_the_timer_event);

    // ASIC初期化(ポート設定).
    init_ev3::initialize();

    /* Open Bluetooth file */
//    bt = ev3_serial_open_file(EV3_SERIAL_BT);
//  assert(bt != NULL);

    // デバッグコンソールを真っ先に立ち上げる.
    the_task[kTaskDebugConsole] = new DebugConsole(ID_TASK_DEBUG_CONSOLE, ID_MSG_BOX_DEBUG_CONSOLE);
    RTOS::startTask(ID_TASK_DEBUG_CONSOLE);

    /* LCD画面表示 */
//  ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
//  ev3_lcd_draw_string("EV3way-ET RisoRanger 2016 ver000.000.001", 0, CALIB_FONT_HEIGHT*1);


    // カーネルオブジェクト生成.
    the_task[kTaskHighPriority] = new HighPriority(ID_TASK_MAIN, ID_MSG_BOX_HIGH_PRIORITY);
    the_task[kTaskEngine] = new Engine(ID_TASK_ENGINE, ID_MSG_BOX_ENGINE);


    RTOS::startTask(ID_TASK_HIGH_PRIORITY);
    RTOS::startTask(ID_TASK_ENGINE);

    // 生成したカーネルオブジェクトに稼動と初期化の指示.

    // システムスタート指示.

    //TODO 試走会暫定
    Temp::init();
    Timer::getInstance()->startTimer(4, kTempTimer);


    // Main タスク 定常処理開始
    loop();
}


void Main::loop(){
    // 何やる？
    // 以下案
    // - HEARTBEAT 点滅
    // - メモリリソースの監視
    for(;;){
        RTOS::waiTask(1000);         // 超暫定、こんなのやっちゃダメ!ってか、ってかOSの単位時間がわからない.
    }
}
