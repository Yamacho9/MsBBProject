//���s���@�p�b�P�[�W
#include "RunMethod/Posture.h"
#include "RunMethod/Vertical.h"
#include "RunMethod/Inclination.h"
#include "RunMethod/LineTraceMethod.h"
#include "RunMethod/RunMethod.h"
#include "RunMethod/PID.h"

//�R�[�X�p�b�P�[�W
#include "Course/RunCourse.h"
#include "Course/RunBlock.h"
#include "Course/Track.h"

//�����p�b�P�[�W
#include "Condition/AllDistanceCondition.h"
#include "Condition/Condition.h"

//�⏕�@�\�p�b�P�[�W
#include "SubFunction/Calibration.h"
#include "SubFunction/Stop.h"
#include "SubFunction/Sound.h"

//UI�p�b�P�[�W
#include "UI/UI.h"

//�ʐM�p�b�P�[�W
#include "Communication/GetLogData.h"
#include "Communication/RemoteStart.h"

//API�֘A
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
//�|�[�g�ݒ�

const ePortS GYRO_SENSOR_PORT =			PORT_1;
const ePortS ULTRASONIC_SENSOR_PORT =	PORT_2;
const ePortS LIGHT_SENSOR_PORT =		PORT_3;
const ePortS TOUCH_SENSOR_PORT =		PORT_4;
const ePortM TAIL_MOTOR_PORT =			PORT_A;
const ePortM RIGHT_MOTOR_PORT =			PORT_B;
const ePortM LEFT_MOTOR_PORT =			PORT_C;



extern "C"
{

//�J�E���^��`
	DeclareCounter(SysTimerCnt);

//�^�X�N��`
	DeclareTask(RunTask);
	DeclareTask(UiTask);
	DeclareTask(EmergencyStopTask);

//�A���[����`
	DeclareAlarm(RunAlarm);
	DeclareAlarm(UiAlarm);
	DeclareAlarm(EmergencyStopAlarm);

//�C�x���g��`
	DeclareEvent(RunEvent);
	DeclareEvent(UiEvent);
	DeclareEvent(EmergencyStopEvent);

//*****************************************************************************
// �֐��� 	: user_1ms_isr_type2
// ���� 	: �Ȃ�
// �߂�l	: �Ȃ�
// �T�v 	: 1msec�������荞�݃t�b�N�֐�(OSEK ISR type2�J�e�S��)
//*****************************************************************************

	void user_1ms_isr_type2(void)
	{
		SignalCounter(SysTimerCnt);		//�J�E���^�X�V
		SleeperMonitor();				// needed for I2C devices and Clock class �iClock�N���X�Ŏg�p�j
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

//�R�[�X
	RunCourse* in_basic;

//PID
	PID* pid_light;
	PID* pid_tail;

//�K��
	Posture* inc;

//�|��
	Posture* ver;

//�T�E���h
	Sound* sound;

//���m����
	Condition* in01;
	Condition* in02;
	Condition* in03;

//���s���@
	RunMethod* run_straight;
	RunMethod* run_slope_up;
	RunMethod* run_slope_down;

//���s���
	RunBlock* start_straight;
	RunBlock* slope_up;
	RunBlock* slope_down;

//���H
	Track* inbasic;

//���̑�
	RemoteStart* remotestart;
	UI* ui;
	Stop* st;
	Calibration* calibration;
	GetLogData* getlogdata;



//���s�^�X�N�i4�~���b�����j
	TASK(RunTask)
	{

		//�C���X�^���X�\�z

		//PID
		pid_light = new PID(5.0,0.0,0.0);//P,I,D�̏��ԂōœK�l����͂��Ă��������B
		pid_tail = new PID(4.5,0.0,0.0);

		//�K��
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

		//IN�R�[�X�̃x�[�V�b�N�̌��m������ݒ�
		in01 = new AllDistanceCondition(100,&leftMotor,&rightMotor);	//�����s�����F100cm
		in02 = new AllDistanceCondition(200,&leftMotor,&rightMotor);	//�����s�����F200cm
		in03 = new AllDistanceCondition(300,&leftMotor,&rightMotor);	//�����s�����F300cm

		//IN�R�[�X�̃x�[�V�b�N�̑��s���@��ݒ�
		run_straight	= new LineTraceMethod(calibration,ver,pid_light,20,50,&lightSensor,&leftMotor,&rightMotor);	//���C���g���[�X�̃X�s�[�h�F20
		run_slope_up	= new LineTraceMethod(calibration,ver,pid_light,30,50,&lightSensor,&leftMotor,&rightMotor);	//���C���g���[�X�̃X�s�[�h�F30
		run_slope_down	= new LineTraceMethod(calibration,ver,pid_light,40,50,&lightSensor,&leftMotor,&rightMotor);	//���C���g���[�X�̃X�s�[�h�F40

		//IN�R�[�X�̃x�[�V�b�N�̑��s����ݒ�
		start_straight = new RunBlock(in01,run_straight,ui);
		slope_up  = new RunBlock(in02,run_slope_up,ui);
		slope_down  = new RunBlock(in03,run_slope_down,ui);

		//�R�[�X�ɑ��H��ǉ�
		in_basic->addTrack(inbasic);

		//in�x�[�V�b�N�̑��s���̒ǉ�
		inbasic->addBlock(start_straight);
		inbasic->addBlock(slope_up);
		inbasic->addBlock(slope_down);

		//���[�^�̃J�E���^�N���A
		leftMotor.setCount(0);
		rightMotor.setCount(0);
		tailMotor.setCount(0);

		//UI�^�X�N�N��
		ActivateTask(UiTask);

		//�ً}��~�^�X�N�N��
		ActivateTask(EmergencyStopTask);

		//���s����
		in_basic->prepareRun();

		while(1)
		{
			WaitEvent(RunEvent);		//���s�C�x���g�҂�
			ClearEvent(RunEvent);		//���s�C�x���g�N���A

			if(in_basic->startRun() == true)
			{
				tailMotor.setPWM(0);		// A���[�^�I���t�b�N�֐��F�K��
				leftMotor.setPWM(0);		// B���[�^�I���t�b�N�֐��F�E���[�^
				rightMotor.setPWM(0);		// C���[�^�I���t�b�N�֐��F�����[�^

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

		TerminateTask();				//���^�X�N�I��
	}

	//UI�^�X�N�i1000�~���b�����j
	TASK(UiTask)
	{
		while(1)
		{
			WaitEvent(UiEvent);			//UI�C�x���g�҂�
			ClearEvent(UiEvent);		//UI�C�x���g�N���A
			ui->update();
			getlogdata->getLogData();

		}

		TerminateTask();

	}

	//�ً}��~�^�X�N�i1500�~���b�����j
	TASK(EmergencyStopTask)
	{

		while(1)
		{
			WaitEvent(EmergencyStopEvent);	//�ً}��~�C�x���g�҂�
			ClearEvent(EmergencyStopEvent);	//�ً}��~�C�x���g�N���A

			st->stop();

		}

		TerminateTask();				//���^�X�N�I��
	}

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

