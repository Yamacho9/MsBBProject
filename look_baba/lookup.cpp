#include "lookup.h"


Motor* m_tail;//しっぽ
GyroSensor* m_gyro;//じゃいろ
ColorSensor* m_color;//からーせんさ
Motor* m_leftmotor;//らいと
Motor* m_rightmotor;//れふと

/*
 * ルックアップゲート用関数
 * 返り値：true成功，false失敗
 */

bool lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail){

	//必要な定数
	int8_t forward;      /* 前後進命令 */
	int8_t hoge;
	float turn;         /* 旋回命令 */
	int lastErr=0; //前回偏差
	int8_t cur_brightness=0;	/* 検出した光センサ値 */
	bool ret = false;/*しっぽの状態*/
	int min;
	int max;
	getCalibration_pram(&min,&max);

	//必要なインスタンスをappから貰う(残念ながら引数)
	//gyro,color,leftmotor,rightmotor,tail
	m_tail = tail;
	m_gyro = gyro;
	m_color = color;
	m_leftmotor = leftmotor;
	m_rightmotor = rightmotor;
	
	//しっぽを下げる（たてるとこまで）
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

	//しっぽをぐぐれるようにさらにゆっくりさげる
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
	//ライントレース準備
	forward = 30;
	//ライントレース
	for(int i=0;i<1000;i++){
		int target=0;
		cur_brightness = m_color->getBrightness();
        	target = (max + min)/2; 
		turn = LineTrace(hoge, target, cur_brightness, DELTA_T, &lastErr, &hoge);
		m_leftmotor->setPWM(forward-turn);
		m_rightmotor->setPWM(forward+turn);
	}

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
	//ライントレース準備
	forward = 0;
	//ライントレース
	for(int i=0;i<1000;i++){
		int target = 0;
		cur_brightness = m_color->getBrightness();
        	target = (max + min)/2; 
		turn = LineTrace(hoge, target, cur_brightness, DELTA_T, &lastErr, &hoge);
		m_leftmotor->setPWM(forward-turn);
		m_rightmotor->setPWM(forward+turn);
	}
	
	//しっぽを上げながらライントレースを開始
	while(1){
		//しっぽ上げる
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_DRIVE, eSlow);
		}
		else{
			//しっぽを下げたら終わり
			ret = false;
			break;
		}
	}

	//倒立走行をONにする
	m_gyro->reset();
	balance_init();

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



