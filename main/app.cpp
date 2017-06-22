/**
 ******************************************************************************
 ** ファイル名 : app.cpp
 **
 ** 概要 : Bluetoothスタート ＋ ライントレース（PID制御）+キャリブレーション
 **
 ** 注記 : 結合作業
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
#include "LineTrace.h"

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

/* 下記のマクロは個体/環境に合わせて変更する必要があります */
#define GYRO_OFFSET           0  /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
#define SONAR_ALERT_DISTANCE 30  /* 超音波センサによる障害物検知距離[cm] */
#define TAIL_ANGLE_STAND_UP  93  /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3  /* バランス走行時の角度[度] */
#define TAIL_ANGLE_INIT       0  /* 0度 */
#define P_GAIN             2.5F  /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60  /* 完全停止用モータ制御PWM絶対最大値 */
//#define DEVICE_NAME     "ET0"  /* Bluetooth名 hrp2/target/ev3.h BLUETOOTH_LOCAL_NAMEで設定 */
//#define PASS_KEY        "1234" /* パスキー    hrp2/target/ev3.h BLUETOOTH_PIN_CODEで設定 */
#define CMD_START         '1'    /* リモートスタートコマンド */
#define TARGET				35	 //ライントレース制御 光量ターゲット値
#define DELTA_T				0.004 //処理周期（s）
#define INT_NUM				250	//積分する偏差数(1s分)

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)
#define MESSAGE_LEN 8

/* 関数プロトタイプ宣言 */
static int32_t sonar_alert(void);
static void tail_control(int32_t angle);
//メッセージを書く関数
static void Message(const char* str);
//状態を表示する関数
void display();
//各センサの初期化をする関数
static void Init();
//キャリブレーション
void Calibration(int* min, int* max, Motor* left, Motor* right, GyroSensor* gyroSen, TouchSensor* touch);

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

/* メインタスク */
void main_task(intptr_t unused)
{
    int8_t forward;      /* 前後進命令 */
	float turn;         /* 旋回命令 */
	int8_t pwm_L, pwm_R; /* 左右モータPWM出力 */
	int8_t cur_brightness;	/* 検出した光センサ値 */
	int errorList[INT_NUM];	//偏差履歴テーブル
	int i;
	for (i = 0; i < INT_NUM; i++) { //テーブル初期化
		errorList[i] = 0;
	}
	int nextErrorIndex = 0;	//次の変更履歴のインデックス
	int max=-255;//キャリブレーションの最大値
	int min=255;//キャリブレーションの最小値

	/*グローバル変数の初期化*/
	count = 1;
	
	/*各センサのポート設定*/
	Init();
	
	/* 尻尾モーターのリセット */
    tailMotor->reset();
	tail_control(TAIL_ANGLE_INIT); /* 0度に制御 */

	
    /* Open Bluetooth file */
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
	Message("bluetooth start waiting...");
	while(1){
		if (bt_cmd == 1){//bluetooth start
			fprintf(bt,"bluetooth start");
    		break;
    	}
		if (touchSensor->isPressed())
        {
		 	Message("touch sensor start");
            break; /* タッチセンサが押された */
        }
		clock->sleep(10);
	}
	
	//キャリブレーション
	//min,maxにキャリブレーションの結果が出力される
	//Calibration(&min, &max);
	Calibration(&min, &max, leftMotor, rightMotor, gyroSensor, touchSensor);

    ev3_led_set_color(LED_GREEN); /* スタート通知 */
	
	/* 走行モーターエンコーダーリセット */
    leftMotor->reset();
    rightMotor->reset();
    
    /* ジャイロセンサーリセット */
    gyroSensor->reset();
    balance_init(); /* 倒立振子API初期化 */
	
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
    	
        tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */

        if (sonar_alert() == 1) /* 障害物検知 */
        {
			forward = turn = 0; /* 障害物を検知したら停止 */
		}
        else
        {
            forward = 45; /* 前進命令 */
			cur_brightness = colorSensor->getBrightness();
			turn = LineTrace(TARGET, cur_brightness, DELTA_T, errorList, nextErrorIndex);
			fprintf(bt, "cur_brightness = %d, turn = %f\n", cur_brightness, turn);
		}

        /* 倒立振子制御API に渡すパラメータを取得する */
        motor_ang_l = leftMotor->getCount();
        motor_ang_r = rightMotor->getCount();
        gyro = gyroSensor->getAnglerVelocity();
        volt = ev3_battery_voltage_mV();

        /* 倒立振子制御APIを呼び出し、倒立走行するための */
        /* 左右モータ出力値を得る */
        balance_control(
            (float)forward,
            (float)turn,
            (float)gyro,
            (float)GYRO_OFFSET,
            (float)motor_ang_l,
            (float)motor_ang_r,
            (float)volt,
            (int8_t *)&pwm_L,
            (int8_t *)&pwm_R);

        leftMotor->setPWM(pwm_L);
        rightMotor->setPWM(pwm_R);

        clock->sleep(4); /* 4msec周期起動 */
    }
    leftMotor->reset();
    rightMotor->reset();

	/*終了処理*/
    ter_tsk(BT_TASK);
    fclose(bt);

    ext_tsk();
}

//*****************************************************************************
// 関数名 : sonar_alert
// 引数 : 無し
// 返り値 : 1(障害物あり)/0(障害物無し)
// 概要 : 超音波センサによる障害物検知
//*****************************************************************************
static int32_t sonar_alert(void)
{
    static uint32_t counter = 0;
    static int32_t alert = 0;

    int32_t distance;

    if (++counter == 40/4) /* 約40msec周期毎に障害物検知  */
    {
        /*
         * 超音波センサによる距離測定周期は、超音波の減衰特性に依存します。
         * NXTの場合は、40msec周期程度が経験上の最短測定周期です。
         * EV3の場合は、要確認
         */
        distance = sonarSensor->getDistance();
        if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
        {
            alert = 1; /* 障害物を検知 */
        }
        else
        {
            alert = 0; /* 障害物無し */
        }
        counter = 0;
    }

    return alert;
}

//*****************************************************************************
// 関数名 : tail_control
// 引数 : angle (モータ目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モータの角度制御
//*****************************************************************************
static void tail_control(int32_t angle)
{
    float pwm = (float)(angle - tailMotor->getCount()) * P_GAIN; /* 比例制御 */
    /* PWM出力飽和処理 */
    if (pwm > PWM_ABS_MAX)
    {
        pwm = PWM_ABS_MAX;
    }
    else if (pwm < -PWM_ABS_MAX)
    {
        pwm = -PWM_ABS_MAX;
    }

    tailMotor->setPWM(pwm);
}

//*****************************************************************************
// 関数名 : bt_task
// 引数 : unused
// 返り値 : なし
// 概要 : 
//*****************************************************************************
void bt_task(intptr_t unused)
{
	/*通信処理*/
	while(1){
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
    colorSensor = new ColorSensor(PORT_3);
    sonarSensor = new SonarSensor(PORT_2);
    gyroSensor  = new GyroSensor(PORT_4);
    leftMotor   = new Motor(PORT_C);
    rightMotor  = new Motor(PORT_B);
    tailMotor   = new Motor(PORT_A);
    clock       = new Clock();
}

//******
// Calibration
// 入力：なし
// 出力：int min,int max
//******
void Calibration(int* min, int* max, ev3api::Motor* left, ev3api::Motor* right, ev3api::GyroSensor* gyroSen, ev3api::TouchSensor* touch){
	int8_t cur_brightness;	/* 検出した光センサ値 */
	int8_t pwm_L, pwm_R; /* 左右モータPWM出力 */
	
		/* キャリブレーション待機 */
    while(1)
    {
        tail_control(TAIL_ANGLE_STAND_UP); /* 完全停止用角度に制御 */
        if (touchSensor->isPressed())
        {
            break; /* タッチセンサが押された */
        }
        clock->sleep(10);
        
    }
    
    /* 走行モーターエンコーダーリセット */
    left->reset();
    right->reset();
    
    /* ジャイロセンサーリセット */
    gyroSen->reset();
    balance_init(); /* 倒立振子API初期化 */
	
    ev3_led_set_color(LED_GREEN); /* スタート通知 */

    /**
    * Main loop for the self-balance control algorithm
    */
	//clock_t start = clock();    // スタート時間
	int forward = 23; /* 前進命令 */
	int turn = 0;
	int count=0, count2=0;
	//*min=255;
	//*max=-255;
    while(1)
    {

        if (ev3_button_is_pressed(BACK_BUTTON)) break;

        tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */

        if (sonar_alert() == 1) /* 障害物検知 */
        {
			forward = turn = 0; /* 障害物を検知したら停止 */
		}
        else
        {
			cur_brightness = colorSensor->getBrightness();
			
			if(cur_brightness>=*max){
				*max = cur_brightness;
			}
			if(cur_brightness<=*min){
				*min = cur_brightness;
			}
			fprintf(bt, "max = %d, min = %d\n", *max, *min);
		}

        /* 倒立振子制御API に渡すパラメータを取得する */
        motor_ang_l = left->getCount();
        motor_ang_r = right->getCount();
        gyro = gyroSen->getAnglerVelocity();
        volt = ev3_battery_voltage_mV();


        /* 倒立振子制御APIを呼び出し、倒立走行するための */
        /* 左右モータ出力値を得る */
        balance_control(
            (float)forward,
            (float)turn,
            (float)gyro,
            (float)GYRO_OFFSET,
            (float)motor_ang_l,
            (float)motor_ang_r,
            (float)volt,
            (int8_t *)&pwm_L,
            (int8_t *)&pwm_R);

        left->setPWM(pwm_L);
        right->setPWM(pwm_R);
    	
        clock->sleep(4); /* 4msec周期起動 */
        if(count>=250){
			forward=forward*(-1);
			count2++;
			if(count2>=4){
					break;
			}
			count=0;
		}
        count++;
    }
    left->reset();
    right->reset();
}
