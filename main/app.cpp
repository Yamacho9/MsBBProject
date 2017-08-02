/**
 ******************************************************************************
 ** ファイル名 : app.cpp
 **
 ** 概要 : Bluetoothスタート ＋ ライントレース（PID制御）+キャリブレーション
 **
 ** 注記 : 結合作業
 ******************************************************************************
 **/

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
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
#include "Calibration.h"
#include "judgeSection.h"
#include "CalcDistanceAndDirection.h"

using namespace ev3api;

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif


/*グローバル変数*/
/*ロボットに文字を表示するためのグローバル変数*/
int count;
static char message[MESSAGE_LEN + 1] = {0};
/*スピードを上げるためのグローバル変数*/
int speed_count=0;
int speed=0;
/* D制御用 */
int lastErr=0; //前回偏差
/* Bluetooth */
static int32_t   bt_cmd = 0;      /* Bluetoothコマンド 1:リモートスタート */
static FILE     *bt = NULL;      /* Bluetoothファイルハンドル */

/* パラメータファイル情報 extern */
int buf[BUF_LINE_SIZE][BUF_COLUMN_SIZE];
int param[BUF_COLUMN_SIZE];
int linenum;
int *arr0, *arr1;



/*関数のプロトタイプ宣言*/
//メッセージを書く関数
static void Message(const char* str);
//各センサの初期化、パラメータファイルの読み込みをする関数
static void Init();
//終了処理
void Finalize();
//超音波センサ
static int32_t sonar_alert(void);
//しっぽコントロール
static bool tail_control(int32_t angle, tailSpeed sp);
//マップ情報読み込み関数
void readMapdata();
//PIDパラメータ読み込み関数
void readPIDdata();
//
void split(char* s, const std::string& delim,int i);

/* オブジェクトへのポインタ定義 */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;


/* メインタスク */
void main_task(intptr_t unused)
{
    int8_t forward;      /* 前後進命令 */
	float turn;         /* 旋回命令 */
	int8_t pwm_L, pwm_R; /* 左右モータPWM出力 */
	int8_t cur_brightness=0;	/* 検出した光センサ値 */

	int max=-255;//キャリブレーションの最大値
	int min=255;//キャリブレーションの最小値
	bool ret = false;
	int section=1; //現在の区間

	/*グローバル変数の初期化*/
	count = 1;
	
	/*各センサのポート設定*/
	Init();
	
	/* 尻尾モーターのリセット */
    tailMotor->reset();

	
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
	
	//キャリブレーション
	//min,maxにキャリブレーションの結果が出力される
	Message("Calibration waiting..");
	Message("push touch sensor : do calibration");
	Message("push back button : don't calibration");
	Calibration(&min, &max, colorSensor, leftMotor, rightMotor, gyroSensor, tailMotor, touchSensor, clock);
	fprintf(bt,"Calibration result\nmax:%d min:%d",max,min);
	
    ev3_led_set_color(LED_GREEN); /* スタート通知 */
	

	//bluetooth start
	Message("bluetooth start waiting...");
	while(1){
		if(!ret){
			ret = tail_control(TAIL_ANGLE_STAND_UP, eSlow);
		}
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
	
	/* 走行モーターエンコーダーリセット */
    leftMotor->reset();
    rightMotor->reset();
	
	/* ジャイロセンサーリセット */
    gyroSensor->reset();
    balance_init(); /* 倒立振子API初期化 */

	/* 走行体の状態を起こす */
	while(1)
	{
		float pwm = (float)(TAIL_ANGLE_START - tailMotor->getCount()); // 比例制御
		if (pwm > 0)
		{
			tailMotor->setPWM(20);
		}
		else if (pwm < 0)
		{
			break;
		}
		clock->sleep(4);
	}
	ret = false;
	
    /**
    * メインループ
    */
    while(1)
    {
    	int32_t motor_ang_l=0, motor_ang_r=0;
		int32_t gyro, volt=0;
    	int target=0;
    	int distance, direction; //走行距離、向き
    	
    	if (ev3_button_is_pressed(BACK_BUTTON)){
    		//backbuttonが押されると終了
    		Message("finished...");
    		break;
    	}
		if (touchSensor->isPressed())
		{ 
			// タッチセンサが押されると終了
			Message("finished...");
			break;
		}

    	if (gyroSensor->getAnglerVelocity() > FALL_DOWN || -(gyroSensor->getAnglerVelocity()) > FALL_DOWN)
    	{
			// 転倒を検知すると終了
	    	fprintf(bt, "getAnglerVelocity = %d\n", gyroSensor->getAnglerVelocity());
    		fprintf(bt, "Emergency Stop.\n");
			Message("finished...");
    		break;
    	}
    	
        if(!ret){
        	/* バランス走行用角度に制御 */
			ret = tail_control(TAIL_ANGLE_DRIVE, eFast);
		}

        if (sonar_alert() == 1) /* 障害物検知 */
        {
			forward = turn = 0; /* 障害物を検知したら停止 */
		}
        else
        {
        	/*
        	//3s後に速度が45に到達するように少しずつ加速させる
            forward = 10 + speed;
        	speed_count = speed_count + 464;
        	if(speed_count > 10000){
        		if(speed < 35){
        			speed++;
        		}
        		speed_count = speed_count - 10000;
        	}
        	*/
        	//forward = 10; /* 前進命令 */
			cur_brightness = colorSensor->getBrightness();
        	target = (max + min)/2;

        	//turn値とforwardが返り値
			turn = LineTrace(section, target, cur_brightness, DELTA_T, &lastErr, &forward);
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
        	(float)GYRO_OFFSET_PID,
            (float)motor_ang_l,
            (float)motor_ang_r,
            (float)volt,
            (int8_t *)&pwm_L,
            (int8_t *)&pwm_R);

        leftMotor->setPWM(pwm_L);
        rightMotor->setPWM(pwm_R);

    	/* 距離・角度計測 */
    	distance = 0;
    	direction = 0;
		CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);

    	//現在の区間を取得する
    	section = judgeSection(distance,direction);
    	//fprintf(bt, "distance = %d, direction = %d\n", distance, direction);
    	//fprintf(bt, "cur_brightness = %d, turn = %f, forward = %d\n", cur_brightness, turn, forward);
    	//現在の走行状況を記録
    	float p,i,d;	//PID制御係数
    	GetPID(&p,&i,&d);	//取得
    	int err;	//偏差
    	float diff;	//偏差微分
    	GetVar(&err,&diff);	//取得
    	fprintf(bt,"p:%f i:%f d:%f\n",p,i,d);
    	fprintf(bt,"err:%d diff:%f\n",err,diff);
    	fprintf(bt, "distance = %d | direction = %d | section%d \nbrightness = %d | turn = %f | forward = %d\n",distance,direction,section,cur_brightness,turn,forward);
    	
        clock->sleep(4); /* 4msec周期起動 */
    }
    leftMotor->reset();
    rightMotor->reset();
	tailMotor->reset();

	/*終了処理*/
	Finalize();
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
static bool tail_control(int32_t angle, tailSpeed sp)
{
	float pwm_max;
	float pwm = (float)(angle - tailMotor->getCount()) * P_GAIN; // 比例制御
	if (pwm<0.1 && pwm >-0.1){
		tailMotor->setBrake(true);
		tailMotor->setPWM(0);
		return true;
	}
	else{
		tailMotor->setBrake(false);
		if (sp == eFast){
			pwm_max = PWM_ABS_MAX_FAST;
		}else if (sp == eSlow){
			pwm_max = PWM_ABS_MAX_SLOW;
		}else{
			pwm_max = 45;
		}
		
		// PWM出力飽和処理
		if (pwm > pwm_max)
		{
			pwm = pwm_max;
		}
		else if (pwm < -pwm_max)
		{
			pwm = -pwm_max;
		}
		tailMotor->setPWM(pwm);
		return false;
	}

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
		if(c != EOF){
			fprintf(bt,"%c",c);
		}
		
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
// 関数名 : readMapdata()
// 引数 : 
// 返り値 : 
// 概要 : 
//*******************************************************************
void readMapdata() {
	
	int i;
	
	std::cerr << "reading" << std::endl;

	std::ifstream ifs;  // ファイル読み取り用ストリーム
	ifs.open("/ev3rt/apps/sumL.txt");	// ファイルオープン

	if (ifs.fail()) {	// ファイルオープンに失敗したらそこで終了
		std::cerr << "ファイルを開けません\n";
		return;
	}

	char buf[256];	// データ一時保管用配列

	while (ifs.getline(buf, sizeof(buf))) {	// ファイルから1行ずつ読み込む
		linenum++;	// 行数をカウントしている
	}

	std::cerr << "読み込んだ行数 = " << linenum << "\n";

	ifs.clear(); // ファイル末尾に到達というフラグをクリア
	ifs.seekg(0, std::ios::beg);	// ファイル先頭に戻る

	arr0 = (int *)malloc(linenum * sizeof(int));
	arr1 = (int *)malloc(linenum * sizeof(int));

	for (i = 0; i<linenum; i++) {
		ifs.getline(buf, sizeof(buf));	// 一行読込
		split(buf, ",",i);
		std::cout << i << " = " << arr0[i] << ", " << arr1[i] << std::endl;
	}
	
	ifs.close();

	return;
}

//*******************************************************************
// 関数名 : readMapdata()
// 引数 : 
// 返り値 : なし
// 概要 : 
//*******************************************************************

void readPIDdata(){
	/* 配列の要素数計算 */
	
	/* パラメータファイル情報 */
	FILE *fp;
	int buf_size_c = sizeof(buf[0])/sizeof(buf[0][0]);/*4*/
	int buf_size_l = sizeof(buf)/sizeof(buf[0]);/*9*/
	
	/* パラメータファイルを読み込み用として開く */
	int i=0,j=0;
	fp = fopen("/ev3rt/apps/param.txt","r");
	if(fp == NULL){
		printf("%sが開けませんでした\n","/ev3rt/apps/param.txt");
		/* default値を代入して戻る */
		for (j=0; j<buf_size_c; j++){
			for(i=0; i<buf_size_l; i++){
				buf[0][j] = 0;
				buf[1][j] = 50;
				buf[2][j] = 74;
				buf[3][j] = 1;
				buf[4][j] = 3;
			}
		}
		return;
	}
	
	/* ファイルを読み込んで配列bufに格納 */
	for (j=0; j<buf_size_c; j++){
		for(i=0; i<buf_size_l; i++){
			if(fscanf(fp,"%d,",&buf[i][j])!='\0'){
				;
			}
		}
	}
	
	fclose(fp);
	
}
//*******************************************************************
// 関数名 : split
// 引数 : 
// 返り値 : 
// 概要 : 
//*******************************************************************
void split(char* s, const std::string& delim,int i)
{
	using namespace std;
	int count = 0;
	char *tok;

	//std::cerr << "split" << std::endl;

	tok = strtok(s, delim.c_str());
	while (tok != NULL) {
		if ((count % 2) == 0) {
			arr0[i] = atoi(tok);
		}
		else {
			arr1[i] = atoi(tok);
		}
		tok = strtok(NULL, delim.c_str());  /* 2回目以降 */
		count++;
	}
	//printf("i=%d, count=%d, arr0=%d, arr1=%d\n", i, count, arr0[i], arr1[i]);
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
	
	//マップデータ読み込み
	readMapdata();
	//PIDパラメータデータ読み込み
	readPIDdata();
	
}

//*******************************************************************
// 関数名 : Finalize()
// 引数 : なし
// 返り値 : なし
// 概要 : 終了処理たち
//*******************************************************************
void Finalize(){

	free(arr0);
	free(arr1);
}



/*getset関数たち*/
int getBufLineSize(){
	return BUF_LINE_SIZE;
}
int getBufColumnSize(){
	return BUF_COLUMN_SIZE;
}
int getlinenum(){
	return linenum;
}
void setlinenum(int num){
	linenum = num;
}
