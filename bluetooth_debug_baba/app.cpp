/**
 ******************************************************************************
 ** ファイル名 : app.cpp
 **
 ** 概要 : カラーセンサの値をbluetoothで拾ってくるもの作りたい
 **2017/5/17ばば
 ** 注記 : 
 ******************************************************************************
 **/

#include "ev3api.h"
#include "app.h"
#include "balancer.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"

using namespace ev3api;

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/* Bluetooth */
static int32_t   bt_cmd = 0;      /* Bluetoothコマンド 1:リモートスタート */
static FILE     *bt = NULL;      /* Bluetoothファイルハンドル */

//#define DEVICE_NAME     "RisoRanger2016-mk2"  /* Bluetooth名 hrp2/target/ev3.h BLUETOOTH_LOCAL_NAMEで設定 */
//#define PASS_KEY        "1111" /* パスキー    hrp2/target/ev3.h BLUETOOTH_PIN_CODEで設定 */
#define CMD_START         '1'    /* リモートスタートコマンド */

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)
#define MESSAGE_LEN 8

/* 関数プロトタイプ宣言 */
//メッセージを書く関数
static void Message(const char* str);
//状態を表示する関数
void display();
//各センサの初期化をする関数
static void Init();


/* オブジェクトへのポインタ定義 */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;

/*表示するためのグローバル変数*/
int count;
static char message[MESSAGE_LEN + 1] = {0};

int colorSensor_data = 0;

/* メインタスク */
void main_task(intptr_t unused)
{
	/*グローバル変数の初期化*/
	count = 1;
	
	/*各センサのポート設定*/
	Init();
	
    /* Open Bluetooth file */
	/* iniファイルからbluetooth情報を取得する */
	
	/*接続状態を確認*/
	Message("waitting bluetooth connect");
	while(!ev3_bluetooth_is_connected()){
		tslp_tsk(100);
	}
	
	/*シリアルポートを開く*/
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);
	Message("bluetooth serial port open");
    
    /* Bluetooth通信タスクの起動 */
	act_tsk(BT_TASK);
	Message("Bluetooth task Start");
	
    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
	Message("Init finished.");
	
	//bluetooth start
	while(1){
		Message("bluetooth start waiting...");
		 if (bt_cmd == 1){//bluetooth start
		 	fprintf(bt,"bluetooth start start");
    		break;
    	}
	}
    /**
    * メインループ
    */
    while(1)
    {
    	if (ev3_button_is_pressed(BACK_BUTTON)){
    		//backbuttonが押されると終了
    		Message("finished...");
    		break;
    	}
    	
    	/*カラーセンサから値をとってくる*/
    	colorSensor_data = colorSensor->getBrightness();
    	fprintf(bt,"color:%d",colorSensor_data);
    	
    	
        clock->sleep(10); /* 10msec周期起動 */
    }
	

	/*終了処理*/
    ter_tsk(BT_TASK);
    fclose(bt);

    ext_tsk();
}

//*****************************************************************************
// 関数名 : bt_task
// 引数 : unused
// 返り値 : なし
// 概要 : 
//*****************************************************************************
void bt_task(intptr_t unused)
{
	//colorSensor_dataを送信したい
	/*通信処理*/
	while(1){
		/*
		char str[4];
		char* color = "c";
		char* nn = "|";
		
		sprintf(str, "%d", colorSensor_data);

		//現在の情報を送信
		fwrite(color,1,3,bt);
		fwrite(str,1,5,bt);
		fwrite(nn,1,3,bt);
		*/
		
		//受信
		char c = fgetc(bt);
		switch(c){
		case '1':
			bt_cmd = 1;
			break;
		default:
			break;
		
		}
		
		clock->sleep(5);
		
	}
	
}

//*******************************************************************
// 関数名 : display
// 引数 : なし
// 返り値 : なし
// 概要 : 状態を表示する
//*******************************************************************
void display()
{
  ev3_lcd_set_font(EV3_FONT_SMALL);
  ev3_lcd_draw_string("Program is running", 10, 30);
  ev3_lcd_set_font(EV3_FONT_MEDIUM);
  ev3_lcd_draw_string(message, 10, 40);
}

//*******************************************************************
// 関数名 : Message
// 引数 : str(表示したい文字列)
// 返り値 : なし
// 概要 : 
//*******************************************************************
void Message(const char* str){
	ev3_lcd_draw_string(str, 0, CALIB_FONT_HEIGHT*count);
	count++;
}


//*******************************************************************
// 関数名 : Init
// 引数 : なし
// 返り値 : なし
// 概要 : 
//*******************************************************************
void Init(){

	/* 各オブジェクトを生成・初期化する */
    touchSensor = new TouchSensor(PORT_1);
    colorSensor = new ColorSensor(PORT_2);
    sonarSensor = new SonarSensor(PORT_3);
    gyroSensor  = new GyroSensor(PORT_4);
    leftMotor   = new Motor(PORT_C);
    rightMotor  = new Motor(PORT_B);
    tailMotor   = new Motor(PORT_A);
    clock       = new Clock();
}


