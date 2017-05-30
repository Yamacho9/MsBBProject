/**
 ******************************************************************************
 ** ファイル名 : app.cpp
 **
 ** 概要 : 2輪倒立振子ライントレースロボットのTOPPERS/HRP2用C++サンプルプログラム
 **
 ** 注記 : sample_cpp (ライントレース/尻尾モータ/超音波センサ/リモートスタート)
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

/* 下記のマクロは個体/環境に合わせて変更する必要があります */
#define GYRO_OFFSET           0  /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
//#define LIGHT_WHITE          40  /* 白色の光センサ値 */
//#define LIGHT_BLACK           0  /* 黒色の光センサ値 */
#define SONAR_ALERT_DISTANCE 30  /* 超音波センサによる障害物検知距離[cm] */
#define TAIL_ANGLE_STAND_UP  93  /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3  /* バランス走行時の角度[度] */
#define TAIL_ANGLE_INIT       0  /* 0度 */
#define P_GAIN             2.5F  /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60  /* 完全停止用モータ制御PWM絶対最大値 */

#define DEVICE_NAME     "RisoRanger2016-mk2"  /* Bluetooth名 hrp2/target/ev3.h BLUETOOTH_LOCAL_NAMEで設定 */
#define PASS_KEY        "1111" /* パスキー    hrp2/target/ev3.h BLUETOOTH_PIN_CODEで設定 */
#define CMD_START         '1'    /* リモートスタートコマンド */

#define KP					0.74/*0.74*/ //ライントレース制御 比例係数
#define KI					0.01/*0.01*/ //ライントレース制御 積分係数
#define KD					0.03/*0.20*/ //ライントレース制御 微分係数
#define TARGET				35	 //ライントレース制御 光量ターゲット値
#define DELTA_T				0.004 //処理周期（s）
#define INT_NUM				250	//積分する偏差数(1s分)

#define MESSAGE_LEN			20

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

/* 関数プロトタイプ宣言 */
static int32_t sonar_alert(void);
static void tail_control(int32_t angle);
//メッセージを書く関数
static void Message(const char* str);


/* オブジェクトへのポインタ定義 */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;

/*グローバル変数：Massage関数で使用*/
int count;
//static char message[MESSAGE_LEN + 1] = {0};
int colorSensor_data = 0;//カラーセンサの値を代入するもの


/* メインタスク */
void main_task(intptr_t unused)
{
	//グローバル変数の初期化
	count = 1;
	
    int8_t forward;      /* 前後進命令 */
	float turn;         /* 旋回命令 */
	int8_t pwm_L, pwm_R; /* 左右モータPWM出力 */
	//int8_t white;		/* 白色の光センサ値 */
	//int8_t black;		/* 黒色の光センサ値 */
	int8_t cur_brightness;	/* 検出した光センサ値 */
	
	
	float error=0, lasterror=0, integral=0;
	float errorList[INT_NUM];	// 偏差履歴
	int i = 0, j = 0;
	for (i = 0; i < INT_NUM; i++) {
		errorList[i] = 0;
	}
	
    /* 各オブジェクトを生成・初期化する */
    touchSensor = new TouchSensor(PORT_1);
    colorSensor = new ColorSensor(PORT_3);
    sonarSensor = new SonarSensor(PORT_2);
    gyroSensor  = new GyroSensor(PORT_4);
    leftMotor   = new Motor(PORT_C);
    rightMotor  = new Motor(PORT_B);
    tailMotor   = new Motor(PORT_A);
    clock       = new Clock();


    /* 尻尾モーターのリセット */
    tailMotor->reset();
	tail_control(TAIL_ANGLE_INIT); /* 0度に制御 */

    /* Open Bluetooth file */
	/* 接続状態を確認、接続されると抜ける*/
	Message("waitting bluetooth connect");
	while(!ev3_bluetooth_is_connected()){
		tslp_tsk(100);
	}
	/* シリアルポートを開く */
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);
	Message("bluetooth serial port open");

    /* Bluetooth通信タスクの起動 */
    act_tsk(BT_TASK);
	Message("Bluetooth task Start");

    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
	Message("Init finished.");

   /* カラーセンサーキャリブレーションの部分 */
   ////後で追加////////////////////////////////////////
	/* スタート待機 */
	/* スタートするまでの動作 */
	
    while(1)
    {
        //tail_control(TAIL_ANGLE_STAND_UP); // 完全停止用角度に制御

        if (bt_cmd == 1)
        {
            break; // bluetoothスタート
        }

        if (touchSensor->isPressed())
        {
            break; //タッチセンサが押された
        }

        clock->sleep(10);
    }

    /* 走行モーターエンコーダーリセット */
    leftMotor->reset();
    rightMotor->reset();
    
    /* ジャイロセンサーリセット */
    gyroSensor->reset();
    balance_init(); /* 倒立振子API初期化 */

	//count = 0; /* 連続旋回回数初期化*/
	//history = 0; //直近の旋回初期化
	
    ev3_led_set_color(LED_GREEN); /* スタート通知 */
	Message("main loop start");

    /**
    * Main loop for the self-balance control algorithm
    */
    while(1)
    {
    	int32_t motor_ang_l, motor_ang_r;
        int32_t gyro, volt;

        if (ev3_button_is_pressed(BACK_BUTTON)){
    		//backbuttonが押されると終了
    		Message("finished...");
    		break;
    	}
    	
    	/*teratermで表示するため、カラーセンサから値をとってくる*/
    	colorSensor_data = colorSensor->getBrightness();

        tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */

        if (sonar_alert() == 1) /* 障害物検知 */
        {
			//fprintf(bt, "Detect things\n");
			forward = turn = 0; /* 障害物を検知したら停止 */
		}
        else
        {
            forward = 45; /* 前進命令 */
			cur_brightness = colorSensor->getBrightness();
			//monochrome = (cur_brightness - white) / (black - white) * 100;
			//color = colorSensor->getColorNumber();

			// P制御
			//error = cur_brightness - (white + black) / 2;
			//error = TARGET - cur_brightness;	// 黒線の右側をトレース
			error = cur_brightness - TARGET;	// 黒線の左側をトレース

			// I制御
			errorList[j] = error;
			integral = 0;
			for (i = 0; i < INT_NUM; i++) {
				integral += errorList[i] * DELTA_T;
			}
			j + 1 < INT_NUM ? j++ : j = 0;
        	//integral += (lasterror + error) / 2 * DELTA_T;

			// D制御
			turn = KP * error + KI * integral + KD * (error - lasterror) / DELTA_T;
			lasterror = error;
			//turn = KP * error;
			if (turn > 100) {
				turn = 100;
			}
			else if (turn < -100) {
				turn = -100;
			}

			fprintf(bt, "cur_brightness = %d, turn = %f\n", cur_brightness, turn);
			//fprintf(bt, "color_id = %d\n", color);
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
		//fprintf(bt, "left = %d, right = %d\n", pwm_L, pwm_R);

        clock->sleep(4); /* 4msec周期起動 */
    }
    leftMotor->reset();
    rightMotor->reset();

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
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//       ASCIIコードで1を送信すると、リモートスタートする。
//*****************************************************************************
void bt_task(intptr_t unused)
{
    while(1)
    {
        /*uint8_t c = fgetc(bt); // 受信
        switch(c)
        {
        case '1':
            bt_cmd = 1;
            break;
        default:
            break;
        }
        fputc(c, bt); //エコーバック
    	*/
    	
    	
    	//colorSensor_dataを送信したい
		/*通信処理*/
		
		char str[4];
		char* color = "c";
		char* nn = "|";
		
		sprintf(str, "%d", colorSensor_data);

		fwrite(color,1,3,bt);
		fwrite(str,1,5,bt);
		fwrite(nn,1,3,bt);
		
		clock->sleep(500);

    }
}

