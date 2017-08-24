/**
 ******************************************************************************
 ** ファイル名 : StepStage.cpp
 **
 ** 概要 : 階段通過攻略用のプログラム
 **
 ** 注記 : 結合作業
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

bool tail_control_step(int32_t angle, Motor* tail, tailSpeed sp);

using namespace ev3api;

mode GarageIn(int min, int max, ev3api::ColorSensor* colorSensor,ev3api::Motor* leftMotor, ev3api::Motor* rightMotor, ev3api::GyroSensor* gyroSensor, ev3api::Motor* tailMotor, ev3api::TouchSensor* touchSensor, ev3api::Clock* clock)
{
	mode Ret = eLineTrace;
	
	int8_t forward= 0;	  /* 前後進命令 */
	float turn = 0;		 /* 旋回命令 */
	int8_t pwm_L, pwm_R; /* 左右モータPWM出力 */
	int8_t cur_brightness=0;	/* 検出した光センサ値 */
	int lastErr = 0; //前回偏差

	bool ret = false;
	
	int step_mode = 0;
	bool tail_step = false;
	int32_t motor_ang_l_bak = 0, motor_ang_r_bak = 0;
	int distance_org = 0; //走行距離
	int count_stable = 0;
	int max_s = 25;
	bool spin_frag = false;
	bool step_frag = false;
	
	int err;	//偏差
	float diff;	//偏差微分
	int32_t motor_ang_l=0, motor_ang_r=0;
	int32_t gyro, volt=0;
	int target=0;
	int distance, direction; //走行距離、向き
	
	while(1)
	{
		
			if (ev3_button_is_pressed(BACK_BUTTON)){
				//backbuttonが押されると終了
				Message("finished...");
				CurMode = eEnd;
				break;
			}
			if (touchSensor->isPressed())
			{ 
				// タッチセンサが押されると終了
				Message("finished...");
				CurMode = eEnd;
				break;
			}

			if (gyroSensor->getAnglerVelocity() > FALL_DOWN || -(gyroSensor->getAnglerVelocity()) > FALL_DOWN)
			{
				// 転倒を検知すると終了
				fprintf(bt, "getAnglerVelocity = %d\n", gyroSensor->getAnglerVelocity());
				fprintf(bt, "Emergency Stop.\n");
				Message("finished...");
				CurMode = eEnd;
				break;
			}
			
			if(!ret){
				/* バランス走行用角度に制御 */
				ret = tail_control(TAIL_ANGLE_DRIVE, eFast);
			}

		   
			cur_brightness = colorSensor->getBrightness();
			target = (max + min)/2;
			
			//turn値とforwardが返り値
			turn = LineTrace(section, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
			
			/* 倒立振子制御API に渡すパラメータを取得する */
			motor_ang_l = leftMotor->getCount();
			motor_ang_r = rightMotor->getCount();
			gyro = gyroSensor->getAnglerVelocity();
			volt = ev3_battery_voltage_mV();
		
			foward = 20;

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
			CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
			if(ENDGarageL <= distance){
				Ret = eEnd;
				break;
			}
	}
	return Ret;
}

/*
**! @note 
**! @param 
**! @return Ture:階段発見 False:階段見つかっていない
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



//*****************************************************************************
// 関数名 : tail_control_step
// 引数 : angle (モータ目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モータの角度制御
//*****************************************************************************
bool tail_control_step(int32_t angle, Motor* tail, tailSpeed sp)
{
	float pwm_max;
	float pwm = (float)(angle - tail->getCount()) * P_GAIN; // 比例制御
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
		
		// PWM出力飽和処理
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