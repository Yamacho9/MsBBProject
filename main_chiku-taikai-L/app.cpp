/**
 ******************************************************************************
 ** �t�@�C���� : app.cpp
 **
 ** �T�v : Bluetooth�X�^�[�g �{ ���C���g���[�X�iPID����j+�L�����u���[�V����
 **
 ** ���L : �������
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
#include "CalcDistanceAndDirection.h"
#include "StepStage.h"
#include "GarageIn.h"
#include "lookup.h"

using namespace ev3api;

//#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif


/*�O���[�o���ϐ�*/
/*���{�b�g�ɕ�����\�����邽�߂̃O���[�o���ϐ�*/
int count;
static char message[MESSAGE_LEN + 1] = {0};
/*�X�s�[�h���グ�邽�߂̃O���[�o���ϐ�*/
int speed_count=0;
int speed=0;
/* D����p */
int lastErr=0; //�O��΍�
/* Bluetooth */
static int32_t   bt_cmd = 0;      /* Bluetooth�R�}���h 1:�����[�g�X�^�[�g */
static FILE     *bt = NULL;      /* Bluetooth�t�@�C���n���h�� */

/*�֐��̃v���g�^�C�v�錾*/
//���b�Z�[�W�������֐�
static void Message(const char* str);
//�e�Z���T�̏������A�p�����[�^�t�@�C���̓ǂݍ��݂�����֐�
static void Init();
//�����ۃR���g���[��
static bool tail_control(int32_t angle, tailSpeed sp);
// �^�b�`�Z���T�������m�֐�
int bPressTouchSensor(void);

//�^�b�`�Z���T�t���O
int bTouchSensor = 0;	// 0:OFF�A1:ON

/* �I�u�W�F�N�g�ւ̃|�C���^��` */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;

/* ���C���^�X�N */
void main_task(intptr_t unused)
{
    int8_t forward;      /* �O��i���� */
	float turn;         /* ���񖽗� */
	int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int8_t cur_brightness=0;	/* ���o�������Z���T�l */

	int max=-255;//�L�����u���[�V�����̍ő�l
	int min=255;//�L�����u���[�V�����̍ŏ��l
	bool ret = false;
	int section=1; //���݂̋��
#ifndef DEBUG
	Mode mode = eLineTrace;	// ���s���[�h�F���C���g���[�X�i�����l�j
#else
	Mode mode = eLineTrace;	// ���s���[�h�F�����l
#endif
	int32_t motor_ang_l, motor_ang_r;	// ���E�ԗւ̉�]�ʁideg.�j
	int32_t gyro, volt;	// �U�q�|������Ɏg�p
   	int target;	// ���C���g���[�X�̌��Z���T�ڕW�l
   	int distance;	// ���s����
   	int direction; //	����p�x
   	int err;	// ���C���g���[�X�pPD����̕΍�
   	float diff;	// ���C���g���[�X�pPD����̔���
	
	/*�O���[�o���ϐ��̏�����*/
	count = 1;
	
	/*�e�Z���T�̃|�[�g�ݒ�*/
	Init();
	
	/* �K�����[�^�[�̃��Z�b�g */
    tailMotor->reset();

	
    /* Open Bluetooth file */
	/*�V���A���|�[�g���J��*/
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);
	Message("bluetooth serial port open");
    
    /* Bluetooth�ʐM�^�X�N�̋N�� */
	act_tsk(BT_TASK);
	Message("Bluetooth task Start");
	
    ev3_led_set_color(LED_ORANGE); /* �����������ʒm */
	Message("Init finished.");
	
	//�L�����u���[�V����
	//min,max�ɃL�����u���[�V�����̌��ʂ��o�͂����
	Message("Calibration waiting..");
	Message("push touch sensor : do calibration");
	Message("push back button : don't calibration");
	Calibration(&min, &max, colorSensor, leftMotor, rightMotor, gyroSensor, tailMotor, touchSensor, clock);
	target = (max + min)/2;		// �ڕW�l����
	fprintf(bt,"Calibration result\nmax:%d min:%d\n",max,min);
	
    ev3_led_set_color(LED_GREEN); /* �X�^�[�g�ʒm */
	

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
		if (bPressTouchSensor())
        {
		 	Message("touch sensor start");
            break; /* �^�b�`�Z���T�������ꂽ */
        }
		clock->sleep(10);
	}
	
	/* ���s���[�^�[�G���R�[�_�[���Z�b�g */
    leftMotor->reset();
    rightMotor->reset();
	
	/* �W���C���Z���T�[���Z�b�g */
    gyroSensor->reset();
    balance_init(); /* �|���U�qAPI������ */

	/* ���s�̂̏�Ԃ��N���� */
	while(1)
	{
		float pwm = (float)(TAIL_ANGLE_START - tailMotor->getCount()); // ��ᐧ��
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
    * ���C�����[�v
    */
    while(1)
    {
    	// �������i���ԗւ̉�]�ʁA�{�̃o�b�e���[�̓d���A�{�̂̉����x�A���s�����A����p�x�j
    	motor_ang_l=0, motor_ang_r=0;
		volt=0, gyro =0;
    	distance=0;
    	direction=0;
    	
    	if (ev3_button_is_pressed(BACK_BUTTON)){
    		//backbutton���������ƏI��
    		Message("back button is pressed");
    		Message("finished...");
    		break;
    	}
		if (bPressTouchSensor())
		{ 
			// �^�b�`�Z���T���������ƏI��
    		Message("touch sensor is pressed");
			Message("finished...");
			break;
		}

    	if (gyroSensor->getAnglerVelocity() > FALL_DOWN || -(gyroSensor->getAnglerVelocity()) > FALL_DOWN)
    	{
			// �]�|�����m����ƏI��
	    	fprintf(bt, "getAnglerVelocity = %d\n", gyroSensor->getAnglerVelocity());
    		Message("Emergency Stop");
			Message("finished...");
    		break;
    	}
    	
        if(!ret){
        	/* �o�����X���s�p�p�x�ɐ��� */
			ret = tail_control(TAIL_ANGLE_DRIVE, eFast);
		}

    	switch (mode) {
    	case eLineTrace:
			// ���݂̌��Z���T�l�擾
    		cur_brightness = colorSensor->getBrightness();
    		
    		//turn�l��forward���Ԃ�l
			turn = LineTrace(section, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
    		
        	/* �|���U�q����API �ɓn���p�����[�^���擾���� */
        	motor_ang_l = leftMotor->getCount();
        	motor_ang_r = rightMotor->getCount();
        	gyro = gyroSensor->getAnglerVelocity();
        	volt = ev3_battery_voltage_mV();

        	/* �|���U�q����API���Ăяo���A�|�����s���邽�߂� */
        	/* ���E���[�^�o�͒l�𓾂� */
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
    		
	    	/* ���s�����E����p�x�v�� */
			CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
#ifndef DEBUG
    		if(distance >= GOAL_DISTANCE_L + Distance2GoalMargine) mode = eStepStage;
#else
    		distance = distance + START_DEBUG;
    		if(distance >= GOAL_DEBUG) mode = eStepStage;
#endif
	    	//���݂̑��s�󋵂��L�^
	    	fprintf(bt, "distance = %d | direction = %d | section%d \nbrightness = %d | turn = %f | forward = %d | err = %d | diff = %f\n",distance,direction,section,cur_brightness,turn,forward,err,diff);

    		break;
    	case eStepStage:
    		fprintf(bt, "case eStepStage:\n");
    		mode = StepStage(min, max, colorSensor, leftMotor, rightMotor, gyroSensor, tailMotor, touchSensor, clock);
    		break;
    	case eLookUpGate:
			fprintf(bt, "case eLookUpGate:\n");
			mode = lookup(target, gyroSensor, colorSensor, leftMotor, rightMotor, tailMotor, clock, touchSensor, sonarSensor);
    		break;
		case eGarageIn:
			fprintf(bt, "case eGarageIn:\n");
			mode = GarageIn(min, max, colorSensor, leftMotor, rightMotor, gyroSensor, tailMotor, touchSensor, clock);
			break;
		case eEnd:
			fprintf(bt, "case eEnd:\n");
			break;
		}

    	if (mode == eEnd) {
    		Message("mode = eEnd");
			Message("finished...");
    		break;
    	}
    	
        clock->sleep(4); /* 4msec�����N�� */
    }
    leftMotor->reset();
    rightMotor->reset();
	tailMotor->reset();

	/*�I������*/
    ter_tsk(BT_TASK);
    fclose(bt);

    ext_tsk();
}

//*****************************************************************************
// �֐��� : tail_control
// ���� : angle (���[�^�ڕW�p�x[�x])
// �Ԃ�l : ����
// �T�v : ���s�̊��S��~�p���[�^�̊p�x����
//*****************************************************************************
static bool tail_control(int32_t angle, tailSpeed sp)
{
	float pwm_max;
	float pwm = (float)(angle - tailMotor->getCount()) * P_GAIN; // ��ᐧ��
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
		
		// PWM�o�͖O�a����
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
// �֐��� : bt_task
// ���� : unused
// �Ԃ�l : �Ȃ�
// �T�v : 
//*****************************************************************************
void bt_task(intptr_t unused)
{
	/*�ʐM����*/
	while(1){
		//��M
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
// �֐��� : display
// ���� : �Ȃ�
// �Ԃ�l : �Ȃ�
// �T�v : ��Ԃ�\������
//*******************************************************************
void display()
{
  ev3_lcd_set_font(EV3_FONT_SMALL);
  ev3_lcd_draw_string("Program is running", 10, 30);
  ev3_lcd_set_font(EV3_FONT_MEDIUM);
  ev3_lcd_draw_string(message, 10, 40);
}

//*******************************************************************
// �֐��� : Message
// ���� : str(�\��������������)
// �Ԃ�l : �Ȃ�
// �T�v : 
//*******************************************************************
void Message(const char* str){
	ev3_lcd_draw_string(str, 0, CALIB_FONT_HEIGHT*count);
	count++;
}

//*****************************************************************************
// �֐��� : bPressTouchSensor()
// ���� : ����
// �Ԃ�l : 1(�������m)/0(���������m)
// �T�v : �^�b�`�Z���T�������m�i�`���^�����O�h�~�����܂ށj
//*****************************************************************************
int bPressTouchSensor(void)
{
	// ������
    int now = 0;
    int before = 0;
	int bPress = 0;
	
	now = touchSensor->isPressed();	// ���݂̃^�b�`�Z���T���
	before = bTouchSensor;	//�O��̃^�b�`�Z���T���
	
	if (now == 1 && before == 0){	// �^�b�`�Z���T�������m
		bPress = 1;
	}
	bTouchSensor = now;	// ���݂̃^�b�`�Z���T��Ԃ�ۑ�

    return bPress;
}

//*******************************************************************
// �֐��� : Init
// ���� : �Ȃ�
// �Ԃ�l : �Ȃ�
// �T�v : 
//*******************************************************************
void Init(){

	/* �e�I�u�W�F�N�g�𐶐��E���������� */
    touchSensor = new TouchSensor(PORT_1);
    colorSensor = new ColorSensor(PORT_3);
    //sonarSensor = new SonarSensor(PORT_2);
    gyroSensor  = new GyroSensor(PORT_4);
    leftMotor   = new Motor(PORT_C);
    rightMotor  = new Motor(PORT_B);
    tailMotor   = new Motor(PORT_A);
    clock       = new Clock();
}
