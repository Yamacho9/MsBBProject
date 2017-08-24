/**
 ******************************************************************************
 ** �t�@�C���� : StepStage.cpp
 **
 ** �T�v : �K�i�ʉߍU���p�̃v���O����
 **
 ** ���L : �������
 ******************************************************************************
 **/


#include "ev3api.h"
#include "app.h"
#include "StepStage.h"
#include "balancer.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "LineTrace.h"
#include "CalcDistanceAndDirection.h"


bool findStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak,int32_t gyro);
bool findStep2(int32_t gyro);
bool clearStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak);
bool tail_control_step(int32_t angle, Motor* tail, tailSpeed sp);

using namespace ev3api;

mode StepStage(int min, int max, ev3api::ColorSensor* colorSensor,ev3api::Motor* leftMotor, ev3api::Motor* rightMotor, ev3api::GyroSensor* gyroSensor, ev3api::Motor* tailMotor, ev3api::TouchSensor* touchSensor, ev3api::Clock* clock)
{
	mode Ret = eLineTrace;
	
	int8_t forward= 0;	  /* �O��i���� */
	float turn = 0;		 /* ���񖽗� */
	int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int8_t cur_brightness=0;	/* ���o�������Z���T�l */
	int lastErr = 0; //�O��΍�

	bool ret = false;
	
	int step_mode = 0;
	bool tail_step = false;
	int32_t motor_ang_l_bak = 0, motor_ang_r_bak = 0;
	int distance_org = 0; //���s����
	int count_stable = 0;
	int max_s = 25;
	bool spin_frag = false;
	bool step_frag = false;
	
	int err;	//�΍�
	float diff;	//�΍�����
	int32_t motor_ang_l=0, motor_ang_r=0;
	int32_t gyro, volt=0;
	int target=0;
	int distance, direction; //���s�����A����
	
	while(1)
	{
		
		if (ev3_button_is_pressed(BACK_BUTTON)){
			//backbutton���������ƏI��
			Ret = eEnd;
			break;
		}
		if (touchSensor->isPressed())
		{ 
			// �^�b�`�Z���T���������ƏI��
			Ret = eEnd;
			break;
		}

		if (gyroSensor->getAnglerVelocity() > FALL_DOWN || -(gyroSensor->getAnglerVelocity()) > FALL_DOWN)
		{
			// �]�|�����m����ƏI��
			Ret = eEnd;
			break;
		}
		
		if(!ret){
			/* �o�����X���s�p�p�x�ɐ��� */
			ret = tail_control_step(TAIL_ANGLE_DRIVE, tailMotor, eFast);
		}

	   
		cur_brightness = colorSensor->getBrightness();
		target = (max + min)/2;
		
		/* �|���U�q����API �ɓn���p�����[�^���擾���� */
		motor_ang_l = leftMotor->getCount();
		motor_ang_r = rightMotor->getCount();
		gyro = gyroSensor->getAnglerVelocity();
		volt = ev3_battery_voltage_mV();
		
		if (step_mode == 0){	//�X�e�b�v0�F�i�����m�܂�
			if(findStep(motor_ang_l,motor_ang_r,motor_ang_l_bak,motor_ang_r_bak,gyro)==true){	//true�F�i�������m����
				step_mode = 1;
				forward = 0;
				turn = 0;
				motor_ang_l_bak = 0;
				motor_ang_r_bak = 0;
				CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);	//�i���̈ʒu���L�^����
				distance_org = distance;
			}else{
				//turn�l��forward���Ԃ�l
				turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
				forward = 35;
				motor_ang_l_bak = motor_ang_l;
				motor_ang_r_bak = motor_ang_r;
			}
		}
		else if(step_mode == 1){	//�X�e�b�v�P�F�i�����オ��܂�
			if(!tail_step){	//�܂��K����������=�]�|�h�~�̂���
				tail_step = tail_control_step(TAIL_ANGLE_STAND_UP, tailMotor, eSlow);
				forward = 0;
				turn = 0;
			}
			else{
				forward = 30;	//�X�s�[�h���グ�Ēi�������
				turn = 0;
				if(clearStep(motor_ang_l,motor_ang_r,motor_ang_l_bak,motor_ang_r_bak)==true){	//true�F�i���������
					step_mode = 2;
					forward = 0;
					turn = 0;
					tail_step = false;
					motor_ang_l_bak = 0;
					motor_ang_r_bak = 0;
				}
				else{
					motor_ang_l_bak = motor_ang_l;
					motor_ang_r_bak = motor_ang_r;
				}
				//fprintf(bt, "%d\t%d\t%d\t%d\t%d\n",step_mode,motor_ang_l,motor_ang_r,gyro,gyro_ang);
			}
		}
			else if(step_mode == 2){	//�X�e�b�v�Q�F�X�s���ʒu�ɂ��܂�(�i���ʒu����15cm�ȏ�17cm�����̈ʒu)
			turn = 0;
			CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
			if((distance - distance_org) >= 150 && (distance - distance_org) < 170){
				if(gyro < 0 &&(pwm_L<0 && pwm_R<0)){	//���s�́F��ނ��K�v=���ɓ|�ꂻ�� gyro�l���}�C�i�X=���ɓ|��铮��
					leftMotor->reset();
					rightMotor->reset();
					step_mode = 3;
					clock->sleep(250); /* 0.2sec�҂� */					
				}else{
					//forward = -10; ���ύX
					forward = 0;
				}
			}if(distance-distance_org < 150){
				forward = 10;
			}else{
				forward = -10;
			}
		}
		else if(step_mode == 3){	//�X�e�b�v�R�F�X�s���I���܂�(���v���)
			leftMotor->setPWM(10);
			rightMotor->setPWM(-10);
			CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
			if(direction > 358){ //358�x�ȏ��]������K�����C���g���[�X����
				leftMotor->reset();
				rightMotor->reset();
				gyroSensor->reset();
				step_mode = 4;
			}
		}
		else if(step_mode == 4){	//�X�e�b�v�S�F�K�����s�J�n
			if(cur_brightness > max_s ){
				max_s = cur_brightness;
			}
			target = (max_s + min)/2;
			turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
			pwm_L = turn + 12;	//�����l�ōœK�Ȓl���v�Z����
			pwm_R = -turn + 12;
			leftMotor->setPWM(pwm_L);
			rightMotor->setPWM(pwm_R);
			if(findStep2(gyro)==true){	//2�i�ڂ̒i���𔭌��������U��~(1�b)
				leftMotor->reset();
				rightMotor->reset();
				step_mode = 5;
				clock->sleep(250);
			}
		}
		else if(step_mode == 5){	//�X�e�b�v�T�F2�i�ڂ�o��̊J�n(���ԗ�)
			rightMotor->setPWM(0);
			leftMotor->setPWM(30);	//���ԗւ���]
			float pwm = (float)(85 - tailMotor->getCount()); // �K����80->85�x�ɏグ��
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
			}
		}
			else if(step_mode == 6){	//�X�e�b�v�U�F2�i�ڂ�o��̊J�n(�E�ԗ�)
			rightMotor->setPWM(30);
			leftMotor->setPWM(0);	//�E�ԗւ���]
			float pwm = (float)(90 - tailMotor->getCount()); // �K����85->90�x�ɏグ��
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
			}
		}
		else if(step_mode == 7){	//�X�e�b�v�V�F2�i�ڂ�o��̊J�n(�K��)
			if(step_frag == false && findStep2(gyro)==true){
				step_frag = true;
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
		}
		else if(step_mode == 8){	//�X�e�b�v�W�F2�i�ڏ����i��
			if(!tail_step){
				tail_step = tail_control_step(TAIL_ANGLE_STAND_UP, tailMotor, eSlow);
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
			}
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
				}
			}
		}
		else if(step_mode == 10){	//�X�e�b�v�P�O�F��֓|���ĊJ
			float pwm = (float)(TAIL_ANGLE_START - tailMotor->getCount()); // ��ᐧ��
			if (pwm > 0)
			{
				tailMotor->setPWM(20);
			}
			else if (pwm < 0)
			{
				tailMotor->setPWM(-20);
				step_mode = 11;
				lastErr = 0;
				forward = 0;
				turn = 0;
				ret = false;
			}

		}
		else if(step_mode == 11){	//�X�e�b�v�P�P�F2�b�ԁA�O�ɐi�݂܂�
			turn = 0;
			forward = 20;
			count_stable++;
			if(count_stable > 500){
				count_stable = 0;
				Ret = eGarageIn;
				break;
			}
		}

		if(step_mode == 0 || step_mode == 1 || step_mode == 2 || step_mode == 2 || step_mode == 11){
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
		}
		clock->sleep(4); /* 4msec�����N�� */
	}
	return Ret;
}

/*
**! @note 
**! @param 
**! @return Ture:�K�i���� False:�K�i�������Ă��Ȃ�
*/
bool findStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak,int32_t gyro)
{
	bool ret = false;	
	
	if(((motor_ang_r - motor_ang_r_bak) < 0) && ((motor_ang_l - motor_ang_l_bak) < 0)){
		//ret_motor = true;
		ret = true;
	}
	/*if(gyro > 150){
		ret_gyro = true;
	}
	
	if(ret_motor && ret_gyro){
		ret = true;
		ret_motor = false;
		ret_gyro = false;
	}*/
	
	return ret;
}


/*
**! @note �����ۑ��s���p�̒i�����m
**! @param 
**! @return Ture:�K�i���� False:�K�i�������Ă��Ȃ�
*/
bool findStep2(int32_t gyro)
{
	bool ret = false;	
	
	if(gyro>=40 || gyro<=-40){
		ret = true;
	}
	return ret;
}
/*
**! @note 
**! @param 
**! @return Ture:�K�i�N���A False:�K�i�N���A���Ă��Ȃ�
*/
bool clearStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak)
{
	bool right_frag = false;
	bool left_frag = false;
	
	bool ret = false;
	
	if((motor_ang_r - motor_ang_r_bak) > 2){
		right_frag = true;
	}
	if((motor_ang_l - motor_ang_l_bak) > 2){
		left_frag = true;
	}
	
	if(right_frag == true && left_frag == true){
		ret = true;
	}
	
	return ret;
}

//*****************************************************************************
// �֐��� : tail_control_step
// ���� : angle (���[�^�ڕW�p�x[�x])
// �Ԃ�l : ����
// �T�v : ���s�̊��S��~�p���[�^�̊p�x����
//*****************************************************************************
bool tail_control_step(int32_t angle, Motor* tail, tailSpeed sp)
{
	float pwm_max;
	float pwm = (float)(angle - tail->getCount()) * P_GAIN; // ��ᐧ��
	if (pwm<0.1 && pwm >-0.1){
		tail->setBrake(true);
		tail->setPWM(0);
		return true;
	}else{
		tail->setBrake(false);
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
		tail->setPWM(pwm);
		return false;
	}
}