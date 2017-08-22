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
#include "judgeSection.h"
#include "CalcDistanceAndDirection.h"
#include "StepStage.hpp"

using namespace ev3api;

#define DEBUG

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
static int32_t   bt_cmd = 0;	  /* Bluetooth�R�}���h 1:�����[�g�X�^�[�g */
static FILE	 *bt = NULL;	  /* Bluetooth�t�@�C���n���h�� */

/* �p�����[�^�t�@�C����� extern */
int buf[BUF_LINE_SIZE][BUF_COLUMN_SIZE];
int param[BUF_COLUMN_SIZE];
int linenum;
int *arr0, *arr1;



/*�֐��̃v���g�^�C�v�錾*/
//���b�Z�[�W�������֐�
static void Message(const char* str);
//�e�Z���T�̏������A�p�����[�^�t�@�C���̓ǂݍ��݂�����֐�
static void Init();
//�I������
void Finalize();
//�����g�Z���T
static int32_t sonar_alert(void);
//�����ۃR���g���[��
static bool tail_control(int32_t angle, tailSpeed sp);
//�}�b�v���ǂݍ��݊֐�
void readMapdata();
//PID�p�����[�^�ǂݍ��݊֐�
void readPIDdata();
//
void split(char* s, const std::string& delim,int i);

/* �I�u�W�F�N�g�ւ̃|�C���^��` */
TouchSensor*	touchSensor;
SonarSensor*	sonarSensor;
ColorSensor*	colorSensor;
GyroSensor*	 gyroSensor;
Motor*		  leftMotor;
Motor*		  rightMotor;
Motor*		  tailMotor;
Clock*		  clock;


/* ���C���^�X�N */
void main_task(intptr_t unused)
{
	int8_t forward;	  /* �O��i���� */
	float turn;		 /* ���񖽗� */
	int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int8_t cur_brightness=0;	/* ���o�������Z���T�l */

	int max=-255;//�L�����u���[�V�����̍ő�l
	int min=255;//�L�����u���[�V�����̍ŏ��l
	bool ret = false;
	int section=1; //���݂̋��
	
	// ��������K�i�ʉߗp
	runmode mode = eModeLineTrace;
	//runmode mode = eModeStep;
	int step_mode = 0;
	bool tail_step = false;
	int32_t motor_ang_l_bak = 0, motor_ang_r_bak = 0;
	int distance_org = 0, direction_org = 0; //���s�����A����
	int count_stable = 0;
	int max_s = 25;
	int32_t gyro_ang = 0;
	bool spin_frag = false;
	bool step_frag = false;
	//�����܂ŊK�i�ʉߗp

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

	while(1){	
	//�L�����u���[�V����
	//min,max�ɃL�����u���[�V�����̌��ʂ��o�͂����
	Message("Calibration waiting..");
	Message("push touch sensor : do calibration");
	Message("push back button : don't calibration");
	//Calibration(&min, &max, colorSensor, leftMotor, rightMotor, gyroSensor, tailMotor, touchSensor, clock);
	max = 80;
	min = 0;
	fprintf(bt,"Calibration result\nmax:%d min:%d\n",max,min);
	ev3_led_set_color(LED_GREEN); /* �X�^�[�g�ʒm */
	

	//bluetooth start
	Message("bluetooth start waiting...");
	while(1){
		if(!ret){
			ret = tail_control(TAIL_ANGLE_STAND_UP, eSlow);
		}
		if (bt_cmd == 1){//bluetooth start
			fprintf(bt,"bluetooth start\n");
			bt_cmd = 0;
			break;
		}
		if (touchSensor->isPressed())
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
		int32_t motor_ang_l=0, motor_ang_r=0;
		int32_t gyro, volt=0;
		int target=0;
		int distance, direction; //���s�����A����
		
		if (ev3_button_is_pressed(BACK_BUTTON)){
			//backbutton���������ƏI��
			Message("finished...");
			break;
		}
		if (touchSensor->isPressed())
		{ 
			// �^�b�`�Z���T���������ƏI��
			Message("finished...");
			break;
		}

		if (gyroSensor->getAnglerVelocity() > FALL_DOWN || -(gyroSensor->getAnglerVelocity()) > FALL_DOWN)
		{
			// �]�|�����m����ƏI��
			fprintf(bt, "getAnglerVelocity = %d\n", gyroSensor->getAnglerVelocity());
			fprintf(bt, "Emergency Stop.\n");
			Message("finished...");
			break;
		}
		
		if(!ret){
			/* �o�����X���s�p�p�x�ɐ��� */
			ret = tail_control(TAIL_ANGLE_DRIVE, eFast);
		}

	   
		cur_brightness = colorSensor->getBrightness();
		target = (max + min)/2;
		
		/* �|���U�q����API �ɓn���p�����[�^���擾���� */
		motor_ang_l = leftMotor->getCount();
		motor_ang_r = rightMotor->getCount();
		gyro = gyroSensor->getAnglerVelocity();
		volt = ev3_battery_voltage_mV();
		//gyro_ang = gyroSensor->getAngle();
		
		switch(mode){
		case (eModeLineTrace):
			//turn�l��forward���Ԃ�l
			turn = LineTrace(section, target, cur_brightness, DELTA_T, &lastErr, &forward);			

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

			/* �����E�p�x�v�� */
			distance = 0;
			direction = 0;
			CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
			if(distance > 100){
				mode=eModeStep;
				fprintf(bt, "Step Stage Start....\n");
			}

			//���݂̋�Ԃ��擾����
			section = judgeSection(distance,direction);
			//fprintf(bt, "distance = %d, direction = %d\n", distance, direction);
			//fprintf(bt, "cur_brightness = %d, turn = %f, forward = %d\n", cur_brightness, turn, forward);
			//���݂̑��s�󋵂��L�^
			float p,i,d;	//PID����W��
			GetPID(&p,&i,&d);	//�擾
			int err;	//�΍�
			float diff;	//�΍�����
			GetVar(&err,&diff);	//�擾
			//fprintf(bt,"p:%f i:%f d:%f\n",p,i,d);
			//fprintf(bt,"err:%d diff:%f\n",err,diff);
			//fprintf(bt, "distance = %d | direction = %d | section%d \nbrightness = %d | turn = %f | forward = %d\n",distance,direction,section,cur_brightness,turn,forward);
		
			clock->sleep(4); /* 4msec�����N�� */
			break;
		case (eModeStep):
			if (step_mode == 0){	//�X�e�b�v0�F�i�����m�܂�
				if(findStep(motor_ang_l,motor_ang_r,motor_ang_l_bak,motor_ang_r_bak,gyro)==true){
					step_mode = 1;
					forward = 0;
					turn = 0;
					fprintf(bt, "find step....\n");
					motor_ang_l_bak = 0;
					motor_ang_r_bak = 0;
					//fprintf(bt, "%d\t%d\t%d\t%d\t%d\n",step_mode,motor_ang_l,motor_ang_r,gyro,gyro_ang);
					CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
					distance_org = distance;
					direction_org = direction;
					fprintf(bt, "%d\t%d\t%d\n",direction_org,direction,spin_frag);
				}else{
					//fprintf(bt, "%d\t%d\t%d\t%d\t%d\n",step_mode,motor_ang_l,motor_ang_r,gyro,gyro_ang);
					//turn�l��forward���Ԃ�l
					turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward);
					forward = 35;
					motor_ang_l_bak = motor_ang_l;
					motor_ang_r_bak = motor_ang_r;
				}
			}
			else if(step_mode == 1){	//�X�e�b�v�P�F�i�����オ��܂�
				if(!tail_step){
					//fprintf(bt, "%d\t%d\t%d\t%d\t%d\n",step_mode,motor_ang_l,motor_ang_r,gyro,gyro_ang);
					tail_step = tail_control(TAIL_ANGLE_STAND_UP, eSlow);
					forward = 0;
					turn = 0;
				}
				else{
					forward = 30;
					turn = 0;
					if(clearStep(motor_ang_l,motor_ang_r,motor_ang_l_bak,motor_ang_r_bak)==true){
						step_mode = 2;
						forward = 0;
						turn = 0;
						tail_step = false;
						fprintf(bt, "clear step....\n");
						motor_ang_l_bak = 0;
						motor_ang_r_bak = 0;
						//fprintf(bt, "%d\t%d\t%d\t%d\n",motor_ang_l,motor_ang_r,gyro,gyro_ang);
					}
					else{
						motor_ang_l_bak = motor_ang_l;
						motor_ang_r_bak = motor_ang_r;
					}
					//fprintf(bt, "%d\t%d\t%d\t%d\t%d\n",step_mode,motor_ang_l,motor_ang_r,gyro,gyro_ang);
				}
			}
			else if(step_mode == 2){	//�X�e�b�v�Q�F�X�s���ʒu�ɂ��܂�
				turn = 0;
				//fprintf(bt, "%d\t%d\t%d\n",motor_ang_l,motor_ang_r,gyro);
				CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
				if((distance - distance_org) >= 150 && (distance - distance_org) < 170){
					if(gyro < 0 &&(pwm_L<0 && pwm_R<0)){
						leftMotor->reset();
						rightMotor->reset();
						step_mode = 3;
						fprintf(bt, "stop motor step....\n");
						clock->sleep(250); /* 0.2sec�҂� */
						
						break;
					}else{
						forward = -10;
					}
				}if(distance-distance_org < 150){
					forward = 10;
				}else{
					forward = -10;
				}
			}
			else if(step_mode == 3){	//�X�e�b�v�R�F�X�s���I���܂�
				leftMotor->setPWM(10);
				rightMotor->setPWM(-10);
				CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
				if(direction > 358){
					leftMotor->reset();
					rightMotor->reset();
					gyroSensor->reset();
					step_mode = 4;
					//fprintf(bt, "%d\t%d\t%d\n",motor_ang_l,motor_ang_r,direction);
					//fprintf(bt, "%d\t%d\t%d\t%d\n",motor_ang_l,motor_ang_r,gyro,tailMotor->getCount());
					fprintf(bt, "rotation OK step....\n");
				}
				//fprintf(bt, "%d\t%d\t%d\n",motor_ang_l,motor_ang_r,direction);
				//fprintf(bt, "%d\t%d\t%d\t%d\n",motor_ang_l,motor_ang_r,gyro,tailMotor->getCount());
				break;
			}
			else if(step_mode == 4){	//�X�e�b�v�S�F�K�����s�J�n
				if(cur_brightness > max_s ){
					max_s = cur_brightness;
				}
				target = (max_s + min)/2;
				turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward);
				//fprintf(bt, "target:%d\t cur_bri:%d\t max_s:%d\t min:%d\n",target,cur_brightness,max_s,min);
				pwm_L = turn + 12;
				pwm_R = -turn + 12;
				leftMotor->setPWM(pwm_L);
				rightMotor->setPWM(pwm_R);
				//fprintf(bt, "%d\n",gyro);
				//fprintf(bt, "%d\t%d\n",motor_ang_l-motor_ang_l_bak,motor_ang_r-motor_ang_r_bak);
				if(findStep2(gyro)==true){
					leftMotor->reset();
					rightMotor->reset();
					step_mode = 5;
					fprintf(bt, "tail Start OK step4....\n");
					clock->sleep(250);
					break;
				}
				clock->sleep(4); /* 4msec�����N�� */
				break;
			}
			else if(step_mode == 5){	//�X�e�b�v�T�F2�i�ڂ�o��̊J�n(���ԗ�)
				rightMotor->setPWM(0);
				leftMotor->setPWM(30);
				float pwm = (float)(85 - tailMotor->getCount()); // ��ᐧ��
				if (pwm > 0)
				{
					tailMotor->setPWM(30);
				}
				else if (pwm < 0)
				{
					tailMotor->setPWM(0);
				}
				count_stable++;
				if(count_stable > 100){
					tailMotor->setPWM(0);
					count_stable = 0;
					step_mode = 6;
					fprintf(bt, "Up2Step1 step5....\n");
				}
				clock->sleep(4); /* 4msec�����N�� */
				//fprintf(bt, "%d\t%d\t%d\t%d\n",gyro,motor_ang_l,motor_ang_r,tailMotor->getCount());
				break;
			}
				else if(step_mode == 6){	//�X�e�b�v�U�F2�i�ڂ�o��̊J�n(�E�ԗ�)
				rightMotor->setPWM(30);
				leftMotor->setPWM(0);
				float pwm = (float)(90 - tailMotor->getCount()); // ��ᐧ��
				if (pwm > 0)
				{
					tailMotor->setPWM(30);
				}
				else if (pwm < 0)
				{
					tailMotor->setPWM(0);
				}
				count_stable++;
				if(count_stable > 100){
					count_stable = 0;
					step_mode = 7;
					fprintf(bt, "Up2Step2 step6....\n");
				}
				clock->sleep(4); /* 4msec�����N�� */
				fprintf(bt, "%d\t%d\t%d\t%d\n",gyro,motor_ang_l,motor_ang_r,tailMotor->getCount());
				break;
			}
			else if(step_mode == 7){	//�X�e�b�v�V�F2�i�ڂ�o��̊J�n(�K��)
				if(step_frag == false && findStep2(gyro)==true){
					step_frag = true;
					fprintf(bt, "####Up2Step3 step_frag = true step....\n");
				}
				if(step_frag){	
					float pwm = (float)(TAIL_ANGLE_STAND_UP - tailMotor->getCount()); // ��ᐧ��
					if (pwm < 0)
					{
						tailMotor->setPWM(-20);
					}
					else if (pwm > 0)
					{
						tailMotor->setPWM(20);
						step_mode = 8;
						step_frag = false;
						fprintf(bt, "Up2Step3 step7....\n");
					}
				}
				else{
					float pwm = (float)(90 - tailMotor->getCount()); // ��ᐧ��
					if (pwm > 0)
					{
						tailMotor->setPWM(10);
					}
					else if (pwm < 0)
					{
						tailMotor->setPWM(0);
					}
				}
				
				leftMotor->setPWM(20);
				rightMotor->setPWM(20);
				//fprintf(bt, "%d\t%d\t%d\t%d\n",gyro,motor_ang_l,motor_ang_r,tailMotor->getCount());
				clock->sleep(4); /* 4msec�����N�� */
				break;
			}
			else if(step_mode == 8){	//�X�e�b�v�W�F2�i�ڏ����i��
				if(!tail_step){
					//fprintf(bt, "%d\t%d\t%d\t%d\t%d\n",step_mode,motor_ang_l,motor_ang_r,gyro,gyro_ang);
					tail_step = tail_control(TAIL_ANGLE_STAND_UP, eSlow);
				}
				
				count_stable++;
				leftMotor->setPWM(10);
				rightMotor->setPWM(10);
				if(tail_step && count_stable > 150){
					count_stable = 0;
					step_mode = 9;
					leftMotor->reset();
					rightMotor->reset();
					tail_step = false;
					fprintf(bt, "forword OK step8....\n");
				}
				clock->sleep(4); /* 4msec�����N�� */
				break;
			}
			else if(step_mode == 9){	//�X�e�b�v�X�F�X�s���I���܂�(2�i��)
				leftMotor->setPWM(10);
				rightMotor->setPWM(-10);
				CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
				if(spin_frag == false){
					if(direction > 358){
						leftMotor->reset();
						rightMotor->reset();
						spin_frag = true;
					}
				}
				else{
					if(direction > 283){
						leftMotor->reset();
						rightMotor->reset();
						spin_frag = false;
						gyroSensor->reset();
						balance_init(); /* �|���U�qAPI������ */
						lastErr = 0;
						step_mode = 10;
						//fprintf(bt, "%d\t%d\t%d\n",motor_ang_l,motor_ang_r,direction);
						//fprintf(bt, "%d\t%d\t%d\t%d\n",motor_ang_l,motor_ang_r,gyro,tailMotor->getCount());
						fprintf(bt, "rotation OK step9....\n");
					}
				}
				//fprintf(bt, "%d\t%d\t%d\n",motor_ang_l,motor_ang_r,direction);
				//fprintf(bt, "%d\t%d\t%d\t%d\n",motor_ang_l,motor_ang_r,gyro,tailMotor->getCount());
				break;
			}
			else if(step_mode == 10){	//�X�e�b�v�P�O�F��֓|���ĊJ
				float pwm = (float)(TAIL_ANGLE_START - tailMotor->getCount()); // ��ᐧ��
				if (pwm > 0)
				{
					tailMotor->setPWM(20);
					break;
				}
				else if (pwm < 0)
				{
					tailMotor->setPWM(-20);
					step_mode = 11;
					lastErr = 0;
					forward = 0;
					turn = 0;
					ret = false;
					//fprintf(bt, "%d\t%f\n",cur_brightness,turn);
					fprintf(bt, "tailUP OK step10....\n");
				}

			}
			else if(step_mode == 11){	//�X�e�b�v�P�P�F2�b�ԁA�O�ɐi�݂܂�
				turn = 0;
				forward = 20;
				fprintf(bt, "%d\t%d\t%d\t%d\n",gyro,motor_ang_l,motor_ang_r,tailMotor->getCount());
				count_stable++;
				if(count_stable > 500){
					count_stable = 0;
					step_mode = 0;
					mode = eModeGarage;
					fprintf(bt, "step_mode AllOK step11 to Garage....\n");
				}
			}

				
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
			clock->sleep(4); /* 4msec�����N�� */
			break;
		case (eModeLookUp):
			//turn�l��forward���Ԃ�l
			turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward);
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
			
			clock->sleep(4); /* 4msec�����N�� */
			break;
		case (eModeGarage):
			//turn�l��forward���Ԃ�l
			turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward);
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
			
			clock->sleep(4); /* 4msec�����N�� */
			break;
		}

		
	}
	leftMotor->reset();
	rightMotor->reset();
	//��������f�o�b�O�p(�������[�v)		
	step_mode = 0;
	tail_step = false;
	step_frag = false;
	motor_ang_l_bak = 0;
	motor_ang_r_bak = 0;
	distance_org = 0;
	direction_org = 0; //���s�����A����
	count_stable = 0;
	max_s = 25;
	gyro_ang = 0;
	spin_frag = false;
	mode = eModeLineTrace;
	ret = false;
	clock->sleep(5000);
	}
	//�����܂Ńf�o�b�O�p
	tailMotor->reset();

	/*�I������*/
	Finalize();
	ter_tsk(BT_TASK);
	fclose(bt);

	ext_tsk();
}

//*****************************************************************************
// �֐��� : sonar_alert
// ���� : ����
// �Ԃ�l : 1(��Q������)/0(��Q������)
// �T�v : �����g�Z���T�ɂ���Q�����m
//*****************************************************************************
static int32_t sonar_alert(void)
{
	static uint32_t counter = 0;
	static int32_t alert = 0;

	int32_t distance;

	if (++counter == 40/4) /* ��40msec�������ɏ�Q�����m  */
	{
		/*
		 * �����g�Z���T�ɂ�鋗����������́A�����g�̌��������Ɉˑ����܂��B
		 * NXT�̏ꍇ�́A40msec�������x���o����̍ŒZ��������ł��B
		 * EV3�̏ꍇ�́A�v�m�F
		 */
		distance = sonarSensor->getDistance();
		if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
		{
			alert = 1; /* ��Q�������m */
		}
		else
		{
			alert = 0; /* ��Q������ */
		}
		counter = 0;
	}

	return alert;
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
//*******************************************************************
// �֐��� : readMapdata()
// ���� : 
// �Ԃ�l : 
// �T�v : 
//*******************************************************************
void readMapdata() {
	
	int i;
	
	std::cerr << "reading" << std::endl;

	std::ifstream ifs;  // �t�@�C���ǂݎ��p�X�g���[��
	ifs.open("/ev3rt/apps/sumL.txt");	// �t�@�C���I�[�v��

	if (ifs.fail()) {	// �t�@�C���I�[�v���Ɏ��s�����炻���ŏI��
		std::cerr << "�t�@�C�����J���܂���\n";
		return;
	}

	char buf[256];	// �f�[�^�ꎞ�ۊǗp�z��

	while (ifs.getline(buf, sizeof(buf))) {	// �t�@�C������1�s���ǂݍ���
		linenum++;	// �s�����J�E���g���Ă���
	}

	std::cerr << "�ǂݍ��񂾍s�� = " << linenum << "\n";

	ifs.clear(); // �t�@�C�������ɓ��B�Ƃ����t���O���N���A
	ifs.seekg(0, std::ios::beg);	// �t�@�C���擪�ɖ߂�

	arr0 = (int *)malloc(linenum * sizeof(int));
	arr1 = (int *)malloc(linenum * sizeof(int));

	for (i = 0; i<linenum; i++) {
		ifs.getline(buf, sizeof(buf));	// ��s�Ǎ�
		split(buf, ",",i);
		std::cout << i << " = " << arr0[i] << ", " << arr1[i] << std::endl;
	}
	
	ifs.close();

	return;
}

//*******************************************************************
// �֐��� : readMapdata()
// ���� : 
// �Ԃ�l : �Ȃ�
// �T�v : 
//*******************************************************************

void readPIDdata(){
	/* �z��̗v�f���v�Z */
	
	/* �p�����[�^�t�@�C����� */
	FILE *fp;
	int buf_size_c = sizeof(buf[0])/sizeof(buf[0][0]);/*4*/
	int buf_size_l = sizeof(buf)/sizeof(buf[0]);/*9*/
	
	/* �p�����[�^�t�@�C����ǂݍ��ݗp�Ƃ��ĊJ�� */
	int i=0,j=0;
	fp = fopen("/ev3rt/apps/param.txt","r");
	if(fp == NULL){
		printf("%s���J���܂���ł���\n","/ev3rt/apps/param.txt");
		/* default�l�������Ė߂� */
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
	
	/* �t�@�C����ǂݍ���Ŕz��buf�Ɋi�[ */
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
// �֐��� : split
// ���� : 
// �Ԃ�l : 
// �T�v : 
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
		tok = strtok(NULL, delim.c_str());  /* 2��ڈȍ~ */
		count++;
	}
	//printf("i=%d, count=%d, arr0=%d, arr1=%d\n", i, count, arr0[i], arr1[i]);
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
	sonarSensor = new SonarSensor(PORT_2);
	gyroSensor  = new GyroSensor(PORT_4);
	leftMotor   = new Motor(PORT_C);
	rightMotor  = new Motor(PORT_B);
	tailMotor   = new Motor(PORT_A);
	clock	   = new Clock();
	
	//�}�b�v�f�[�^�ǂݍ���
	readMapdata();
	//PID�p�����[�^�f�[�^�ǂݍ���
	readPIDdata();
	
}

//*******************************************************************
// �֐��� : Finalize()
// ���� : �Ȃ�
// �Ԃ�l : �Ȃ�
// �T�v : �I����������
//*******************************************************************
void Finalize(){

	free(arr0);
	free(arr1);
}



/*getset�֐�����*/
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