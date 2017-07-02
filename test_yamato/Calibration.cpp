
//******
// Calibration.c
// 入力：なし
// 出力：int min,int max
//******

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

using namespace ev3api;

void Calibration(int* min, int* max, ev3api::ColorSensor* color,ev3api::Motor* left, ev3api::Motor* right, ev3api::GyroSensor* gyroSen, ev3api::Motor* tail, ev3api::TouchSensor* touch, ev3api::Clock* clock){
	int8_t cur_brightness;	/* 検出した光センサ値 */
	int8_t pwm_L, pwm_R; /* 左右モータPWM出力 */
	
	/* キャリブレーション待機 */
    while(1)
    {
    	tail_control(TAIL_ANGLE_STAND_UP,tail); /* 完全停止用角度に制御 */
        if (touch->isPressed())
        {
            break; /* タッチセンサが押された */
        }
        clock->sleep(10);
        
    }
    
    /* 走行モーターエンコーダーリセット */
    left->reset();
    right->reset();
    
    /* ジャイロセンサーリセット */
    gyroSen->reset();
    balance_init(); /* 倒立振子API初期化 */
	
    ev3_led_set_color(LED_GREEN); /* スタート通知 */

    /**
    * Main loop for the self-balance control algorithm
    */
	//clock_t start = clock();    // スタート時間
	int forward = 23; /* 前進命令 */
	int turn = 0;
	int count=0, count2=0;

    while(1)
    {
    	int32_t motor_ang_l, motor_ang_r;
		int32_t gyro, volt;

        if (ev3_button_is_pressed(BACK_BUTTON)) break;

        tail_control(TAIL_ANGLE_DRIVE,tail); /* バランス走行用角度に制御 */

        cur_brightness = color->getBrightness();
			
		if(cur_brightness>=*max){
			*max = cur_brightness;
		}
		if(cur_brightness<=*min){
			*min = cur_brightness;
		}
			//fprintf(bt, "max = %d, min = %d\n", *max, *min);
		//}

        /* 倒立振子制御API に渡すパラメータを取得する */
        motor_ang_l = left->getCount();
        motor_ang_r = right->getCount();
        gyro = gyroSen->getAnglerVelocity();
        volt = ev3_battery_voltage_mV();


        /* 倒立振子制御APIを呼び出し、倒立走行するための */
        /* 左右モータ出力値を得る */
        balance_control(
            (float)forward,
            (float)turn,
            (float)gyro,
            (float)GYRO_OFFSET_CALIBRATION,
            (float)motor_ang_l,
            (float)motor_ang_r,
            (float)volt,
            (int8_t *)&pwm_L,
            (int8_t *)&pwm_R);

        left->setPWM(pwm_L);
        right->setPWM(pwm_R);
    	
        clock->sleep(4); /* 4msec周期起動 */
        if(count>=250){
			forward=forward*(-1);
			count2++;
			if(count2>=4){
					break;
			}
			count=0;
		}
        count++;
    }
    left->reset();
    right->reset();
}

//*****************************************************************************
// 関数名 : tail_control
// 引数 : angle (モータ目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モータの角度制御
//*****************************************************************************
void tail_control(int32_t angle, ev3api::Motor* tail)
{
    float pwm = (float)(angle - tail->getCount()) * P_GAIN; // 比例制御
    // PWM出力飽和処理
    if (pwm > PWM_ABS_MAX)
    {
        pwm = PWM_ABS_MAX;
    }
    else if (pwm < -PWM_ABS_MAX)
    {
        pwm = -PWM_ABS_MAX;
    }

    tail->setPWM(pwm);
}