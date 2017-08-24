/**
 ******************************************************************************
 ** �t�@�C���� : 
 **
 ** �T�v : �̃v���O����
 **
 ** ���L : �������
 ******************************************************************************
 **/


#include "ev3api.h"
#include "app.h"
#include "GarageIn.h"
#include "balancer.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "LineTrace.h"
#include "CalcDistanceAndDirection.h"

bool tail_control_garage(int32_t angle, Motor* tail, tailSpeed sp);

using namespace ev3api;

mode GarageIn(int min, int max, ev3api::ColorSensor* colorSensor,ev3api::Motor* leftMotor, ev3api::Motor* rightMotor, ev3api::GyroSensor* gyroSensor, ev3api::Motor* tailMotor, ev3api::TouchSensor* touchSensor, ev3api::Clock* clock)
{
	mode Ret = eLineTrace;
	
	int8_t forward= 0;	  /* �O��i���� */
	float turn = 0;		 /* ���񖽗� */
	int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int8_t cur_brightness=0;	/* ���o�������Z���T�l */
	int lastErr = 0; //�O��΍�

	bool ret = false;
	
	int err;	//�΍�
	float diff;	//�΍�����
	int32_t motor_ang_l=0, motor_ang_r=0;
	int32_t gyro, volt=0;
	int target=0;
	int distance, direction; //���s�����A����
	
	while(1)
	{
		
			if (ev3_button_is_pressed(BACK_BUTTON)){
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
				ret = tail_control_garage(TAIL_ANGLE_DRIVE, tailMotor, eFast);
			}

		   
			cur_brightness = colorSensor->getBrightness();
			target = (max + min)/2;
			
			//turn�l��forward���Ԃ�l
			turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
			
			/* �|���U�q����API �ɓn���p�����[�^���擾���� */
			motor_ang_l = leftMotor->getCount();
			motor_ang_r = rightMotor->getCount();
			gyro = gyroSensor->getAnglerVelocity();
			volt = ev3_battery_voltage_mV();
		
			forward = 20;

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
			CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
			if(ENDGarageL <= distance){
				Ret = eEnd;
				break;
			}
	}
	return Ret;
}

//*****************************************************************************
// �֐��� : tail_control_step
// ���� : angle (���[�^�ڕW�p�x[�x])
// �Ԃ�l : ����
// �T�v : ���s�̊��S��~�p���[�^�̊p�x����
//*****************************************************************************
bool tail_control_garage(int32_t angle, Motor* tail, tailSpeed sp)
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