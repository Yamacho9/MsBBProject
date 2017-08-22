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

bool lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail,Clock* clock){

	//必要な変数
	bool ret = false;//しっぽの状態

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
			//倒立走行OFF
			//m_gyro->reset();
			//balance_init();
			break;
		}
	}
	//バックする。（じゃいろがマイナスになるまで）
	while(1){
		//しっぽを固定
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eFast);
		}
		if(m_gyro->getAngle() < 0){//じゃいろがマイナス
			//倒立走行OFF
			m_gyro->reset();
			balance_init();
			break;
		}
		m_leftmotor->setPWM(-5);
		m_rightmotor->setPWM(-5);
	}
	
	//速度0にする
	for(int i=0;i<2500;i++){
		//しっぽを固定
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eFast);
		}
		
		m_leftmotor->setPWM(0);
		m_rightmotor->setPWM(0);
		
		clock->sleep(4);
	}
	
	//ぐぐれるところまでしっぽを動かす
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
	
	//速度10で前へ
	for(int i=0;i<5000;i++){
		//しっぽを固定
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_LOOKUPGATE, eFast);//後で変更
		}
		m_leftmotor->setPWM(10);
		m_rightmotor->setPWM(10);
		
		clock->sleep(4);
	}
	//とりあえず無限ループ
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_LOOKUPGATE, eFast);//後で変更
		}
		m_leftmotor->setPWM(0);
		m_rightmotor->setPWM(0);
		clock->sleep(4);
	}


	return true;//成功

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



