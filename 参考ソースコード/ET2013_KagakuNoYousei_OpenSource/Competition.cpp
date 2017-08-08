//走行方法パッケージ
#include "RunMethod/Posture.h"
#include "RunMethod/Vertical.h"
#include "RunMethod/Inclination.h"
#include "RunMethod/LineTraceMethod.h"
#include "RunMethod/RunMethod.h"
#include "RunMethod/PID.h"

//コースパッケージ
#include "Course/RunCourse.h"
#include "Course/RunBlock.h"
#include "Course/Track.h"

//条件パッケージ
#include "Condition/AllDistanceCondition.h"
#include "Condition/Condition.h"

//補助機能パッケージ
#include "SubFunction/Calibration.h"
#include "SubFunction/Stop.h"
#include "SubFunction/Sound.h"

//UIパッケージ
#include "UI/UI.h"

//通信パッケージ
#include "Communication/GetLogData.h"
#include "Communication/RemoteStart.h"

//API関連
#include "Motor.h"
#include "TouchSensor.h"
#include "LightSensor.h"
#include "SonarSensor.h"
#include "GyroSensor.h"
#include "Clock.h"
#include "Bluetooth.h"
#include "Lcd.h"
#include "Daq.h"
#include "Speaker.h"
#include "Nxt.h"

using namespace ecrobot;


//------------------------------
//ポート設定

const ePortS GYRO_SENSOR_PORT =			PORT_1;
const ePortS ULTRASONIC_SENSOR_PORT =	PORT_2;
const ePortS LIGHT_SENSOR_PORT =		PORT_3;
const ePortS TOUCH_SENSOR_PORT =		PORT_4;
const ePortM TAIL_MOTOR_PORT =			PORT_A;
const ePortM RIGHT_MOTOR_PORT =			PORT_B;
const ePortM LEFT_MOTOR_PORT =			PORT_C;



extern "C"
{

//カウンタ定義
	DeclareCounter(SysTimerCnt);

//タスク定義
	DeclareTask(RunTask);
	DeclareTask(UiTask);
	DeclareTask(EmergencyStopTask);

//アラーム定義
	DeclareAlarm(RunAlarm);
	DeclareAlarm(UiAlarm);
	DeclareAlarm(EmergencyStopAlarm);

//イベント定義
	DeclareEvent(RunEvent);
	DeclareEvent(UiEvent);
	DeclareEvent(EmergencyStopEvent);

//*****************************************************************************
// 関数名 	: user_1ms_isr_type2
// 引数 	: なし
// 戻り値	: なし
// 概要 	: 1msec周期割り込みフック関数(OSEK ISR type2カテゴリ)
//*****************************************************************************

	void user_1ms_isr_type2(void)
	{
		SignalCounter(SysTimerCnt);		//カウンタ更新
		SleeperMonitor();				// needed for I2C devices and Clock class （Clockクラスで使用）
	}


//API
	Motor tailMotor(TAIL_MOTOR_PORT);
	Motor leftMotor(LEFT_MOTOR_PORT);
	Motor rightMotor(RIGHT_MOTOR_PORT);
	TouchSensor touchSensor(TOUCH_SENSOR_PORT);
	LightSensor lightSensor(LIGHT_SENSOR_PORT);
	SonarSensor sonarSensor(ULTRASONIC_SENSOR_PORT);
	Clock clock;
	GyroSensor gyroSensor(GYRO_SENSOR_PORT);
	Bluetooth bluetooth;
	Lcd lcd;
	Daq daq(bluetooth);
	Speaker speaker;
	Nxt nxt;

//コース
	RunCourse* in_basic;

//PID
	PID* pid_light;
	PID* pid_tail;

//尻尾
	Posture* inc;

//倒立
	Posture* ver;

//サウンド
	Sound* sound;

//検知条件
	Condition* in01;
	Condition* in02;
	Condition* in03;

//走行方法
	RunMethod* run_straight;
	RunMethod* run_slope_up;
	RunMethod* run_slope_down;

//走行区画
	RunBlock* start_straight;
	RunBlock* slope_up;
	RunBlock* slope_down;

//走路
	Track* inbasic;

//その他
	RemoteStart* remotestart;
	UI* ui;
	Stop* st;
	Calibration* calibration;
	GetLogData* getlogdata;



//走行タスク（4ミリ秒周期）
	TASK(RunTask)
	{

		//インスタンス構築

		//PID
		pid_light = new PID(5.0,0.0,0.0);//P,I,Dの順番で最適値を入力してください。
		pid_tail = new PID(4.5,0.0,0.0);

		//尻尾
		inc = new Inclination(pid_tail,110,50,&tailMotor);

		getlogdata = new GetLogData(&daq);
		remotestart = new RemoteStart(&bluetooth);
		calibration = new Calibration(sound,&touchSensor,&lightSensor,&clock,&lcd,&gyroSensor);
		sound = new Sound(&speaker);
		ui = new UI(inc,remotestart,calibration,sound,&touchSensor,&clock,&tailMotor,&leftMotor,&rightMotor,&nxt,&bluetooth,&lcd);
		in_basic = new RunCourse(ui);
		inbasic = new Track();
		st = new Stop(&tailMotor,&leftMotor,&rightMotor,&gyroSensor,&speaker);
		ver = new Vertical(&tailMotor,&leftMotor,&rightMotor,&gyroSensor,&nxt);

		//INコースのベーシックの検知条件を設定
		in01 = new AllDistanceCondition(100,&leftMotor,&rightMotor);	//総走行距離：100cm
		in02 = new AllDistanceCondition(200,&leftMotor,&rightMotor);	//総走行距離：200cm
		in03 = new AllDistanceCondition(300,&leftMotor,&rightMotor);	//総走行距離：300cm

		//INコースのベーシックの走行方法を設定
		run_straight	= new LineTraceMethod(calibration,ver,pid_light,20,50,&lightSensor,&leftMotor,&rightMotor);	//ライントレースのスピード：20
		run_slope_up	= new LineTraceMethod(calibration,ver,pid_light,30,50,&lightSensor,&leftMotor,&rightMotor);	//ライントレースのスピード：30
		run_slope_down	= new LineTraceMethod(calibration,ver,pid_light,40,50,&lightSensor,&leftMotor,&rightMotor);	//ライントレースのスピード：40

		//INコースのベーシックの走行区画を設定
		start_straight = new RunBlock(in01,run_straight,ui);
		slope_up  = new RunBlock(in02,run_slope_up,ui);
		slope_down  = new RunBlock(in03,run_slope_down,ui);

		//コースに走路を追加
		in_basic->addTrack(inbasic);

		//inベーシックの走行区画の追加
		inbasic->addBlock(start_straight);
		inbasic->addBlock(slope_up);
		inbasic->addBlock(slope_down);

		//モータのカウンタクリア
		leftMotor.setCount(0);
		rightMotor.setCount(0);
		tailMotor.setCount(0);

		//UIタスク起動
		ActivateTask(UiTask);

		//緊急停止タスク起動
		ActivateTask(EmergencyStopTask);

		//走行準備
		in_basic->prepareRun();

		while(1)
		{
			WaitEvent(RunEvent);		//走行イベント待ち
			ClearEvent(RunEvent);		//走行イベントクリア

			if(in_basic->startRun() == true)
			{
				tailMotor.setPWM(0);		// Aモータ終了フック関数：尻尾
				leftMotor.setPWM(0);		// Bモータ終了フック関数：右モータ
				rightMotor.setPWM(0);		// Cモータ終了フック関数：左モータ

				break;
			}
		}


		delete pid_light;
		delete pid_tail;
		delete ver;
		delete remotestart;
		delete ui;
		delete in_basic;
		delete inbasic;
		delete st;
		delete calibration;
		delete sound;
		delete inc;

		TerminateTask();				//自タスク終了
	}

	//UIタスク（1000ミリ秒周期）
	TASK(UiTask)
	{
		while(1)
		{
			WaitEvent(UiEvent);			//UIイベント待ち
			ClearEvent(UiEvent);		//UIイベントクリア
			ui->update();
			getlogdata->getLogData();

		}

		TerminateTask();

	}

	//緊急停止タスク（1500ミリ秒周期）
	TASK(EmergencyStopTask)
	{

		while(1)
		{
			WaitEvent(EmergencyStopEvent);	//緊急停止イベント待ち
			ClearEvent(EmergencyStopEvent);	//緊急停止イベントクリア

			st->stop();

		}

		TerminateTask();				//自タスク終了
	}

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

