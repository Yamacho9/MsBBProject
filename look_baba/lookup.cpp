#include "ev3api.h"
#include "app.h"
#include "lookup.h"


Motor* m_tail;//しっぽ
GyroSensor* m_gyro;//じゃいろ

/*
 * ルックアップゲート用関数
 * 返り値：true成功，false失敗
 */

bool lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail){


	//必要なインスタンスをappから貰う(残念ながら引数)
	//gyro,color,leftmotor,rightmotor,tail
	m_tail = tail;
	m_gyro = gyro;
	
	//しっぽを下げる（たてるとこまで）
	//bool result = standuptail();
	//if(result == false){
	//	return false;
	//}
	bool ret = false;
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eSlow);
		}
		else{
			//しっぽを下げたら終わり
			ret = false;
			break;
		}
	}
	//倒立走行OFF
	m_gyro->reset();
	balance_init();

	//しっぽをぐぐれるようにゆっくりさげる
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_LOOKUPGATE, eSlow);
		}
		else{
			//しっぽを下げたら終わり
			ret = false;
			break;
		}
	}	

	//ラインにそってゲートをくぐる
	

	//しっぽをあげる（たてるとこまで）
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eSlow);
		}
		else{
			//しっぽを下げたら終わり
			ret = false;
			break;
		}
	}

	//倒立走行OFF，速度0で走る

	//しっぽを上げながらライントレースを開始

	return true;//成功

}

bool standuptail(){

	//しっぽで建てるところまでしっぽを下げる
	return true;
}

bool tail_ctr(int32_t angle, tailSpeed sp){
	
	float pwm_max;
	float pwm = (float)(angle - m_tail->getCount()) * P_GAIN;

	if(pwm<0.1 && pwm >-0.1){
		m_tail->setBrake(true);
		m_tail->setPWM(0);
		return true;
	}
	else{
		m_tail->setBrake(false);
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
		m_tail->setPWM(pwm);
		return false;
	
	}
}



