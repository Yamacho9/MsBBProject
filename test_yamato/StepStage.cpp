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


bool findStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak,int32_t gyro);
bool findStep2(int32_t gyro);
bool clearStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak);
bool tail_control_step(int32_t angle, Motor* tail, tailSpeed sp);

using namespace ev3api;

mode StepStage(int min, int max, ev3api::ColorSensor* colorSensor,ev3api::Motor* leftMotor, ev3api::Motor* rightMotor, ev3api::GyroSensor* gyroSensor, ev3api::Motor* tailMotor, ev3api::TouchSensor* touchSensor, ev3api::Clock* clock)
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
			Ret = eEnd;
			break;
		}
		if (touchSensor->isPressed())
		{ 
			// タッチセンサが押されると終了
			Ret = eEnd;
			break;
		}

		if (gyroSensor->getAnglerVelocity() > FALL_DOWN || -(gyroSensor->getAnglerVelocity()) > FALL_DOWN)
		{
			// 転倒を検知すると終了
			Ret = eEnd;
			break;
		}
		
		if(!ret){
			/* バランス走行用角度に制御 */
			ret = tail_control_step(TAIL_ANGLE_DRIVE, tailMotor, eFast);
		}

	   
		cur_brightness = colorSensor->getBrightness();
		target = (max + min)/2;
		
		/* 倒立振子制御API に渡すパラメータを取得する */
		motor_ang_l = leftMotor->getCount();
		motor_ang_r = rightMotor->getCount();
		gyro = gyroSensor->getAnglerVelocity();
		volt = ev3_battery_voltage_mV();
		
		if (step_mode == 0){	//ステップ0：段差検知まで
			if(findStep(motor_ang_l,motor_ang_r,motor_ang_l_bak,motor_ang_r_bak,gyro)==true){	//true：段差を検知した
				step_mode = 1;
				forward = 0;
				turn = 0;
				motor_ang_l_bak = 0;
				motor_ang_r_bak = 0;
				CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);	//段差の位置を記録する
				distance_org = distance;
			}else{
				//turn値とforwardが返り値
				turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
				forward = 35;
				motor_ang_l_bak = motor_ang_l;
				motor_ang_r_bak = motor_ang_r;
			}
		}
		else if(step_mode == 1){	//ステップ１：段差を上がるまで
			if(!tail_step){	//まず尻尾を下げる=転倒防止のため
				tail_step = tail_control_step(TAIL_ANGLE_STAND_UP, tailMotor, eSlow);
				forward = 0;
				turn = 0;
			}
			else{
				forward = 30;	//スピードを上げて段差を上る
				turn = 0;
				if(clearStep(motor_ang_l,motor_ang_r,motor_ang_l_bak,motor_ang_r_bak)==true){	//true：段差を上った
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
			else if(step_mode == 2){	//ステップ２：スピン位置につくまで(段差位置から15cm以上17cm未満の位置)
			turn = 0;
			CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
			if((distance - distance_org) >= 150 && (distance - distance_org) < 170){
				if(gyro < 0 &&(pwm_L<0 && pwm_R<0)){	//走行体：後退が必要=後ろに倒れそう gyro値がマイナス=後ろに倒れる動き
					leftMotor->reset();
					rightMotor->reset();
					step_mode = 3;
					clock->sleep(250); /* 0.2sec待つ */					
				}else{
					//forward = -10; ↓変更
					forward = 0;
				}
			}if(distance-distance_org < 150){
				forward = 10;
			}else{
				forward = -10;
			}
		}
		else if(step_mode == 3){	//ステップ３：スピン終わるまで(時計回り)
			leftMotor->setPWM(10);
			rightMotor->setPWM(-10);
			CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
			if(direction > 358){ //358度以上回転したら尻尾ライントレースする
				leftMotor->reset();
				rightMotor->reset();
				gyroSensor->reset();
				step_mode = 4;
			}
		}
		else if(step_mode == 4){	//ステップ４：尻尾走行開始
			if(cur_brightness > max_s ){
				max_s = cur_brightness;
			}
			target = (max_s + min)/2;
			turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
			pwm_L = turn + 12;	//実測値で最適な値を計算した
			pwm_R = -turn + 12;
			leftMotor->setPWM(pwm_L);
			rightMotor->setPWM(pwm_R);
			if(findStep2(gyro)==true){	//2段目の段差を発見したら一旦停止(1秒)
				leftMotor->reset();
				rightMotor->reset();
				step_mode = 5;
				clock->sleep(250);
			}
		}
		else if(step_mode == 5){	//ステップ５：2段目を登るの開始(左車輪)
			rightMotor->setPWM(0);
			leftMotor->setPWM(30);	//左車輪を回転
			float pwm = (float)(85 - tailMotor->getCount()); // 尻尾を80->85度に上げる
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
			else if(step_mode == 6){	//ステップ６：2段目を登るの開始(右車輪)
			rightMotor->setPWM(30);
			leftMotor->setPWM(0);	//右車輪を回転
			float pwm = (float)(90 - tailMotor->getCount()); // 尻尾を85->90度に上げる
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
		else if(step_mode == 7){	//ステップ７：2段目を登るの開始(尻尾)
			if(step_frag == false && findStep2(gyro)==true){
				step_frag = true;
			}
			if(step_frag){	
				float pwm = (float)(TAIL_ANGLE_STAND_UP - tailMotor->getCount()); // 比例制御
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
				float pwm = (float)(90 - tailMotor->getCount()); // 比例制御
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
		else if(step_mode == 8){	//ステップ８：2段目少し進む
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
		else if(step_mode == 9){	//ステップ９：スピン終わるまで(2段目)
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
					balance_init(); /* 倒立振子API初期化 */
					lastErr = 0;
					step_mode = 10;
				}
			}
		}
		else if(step_mode == 10){	//ステップ１０：二輪倒立再開
			float pwm = (float)(TAIL_ANGLE_START - tailMotor->getCount()); // 比例制御
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
		else if(step_mode == 11){	//ステップ１１：2秒間、前に進みます
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
		}
		clock->sleep(4); /* 4msec周期起動 */
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


/*
**! @note しっぽ走行時用の段差検知
**! @param 
**! @return Ture:階段発見 False:階段見つかっていない
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
**! @return Ture:階段クリア False:階段クリアしていない
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