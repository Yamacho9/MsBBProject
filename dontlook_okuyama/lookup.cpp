#include "lookup.h"


Motor* m_tail;//しっぽ
GyroSensor* m_gyro;//じゃいろ
ColorSensor* m_color;//からーせんさ
Motor* m_leftmotor;//らいと
Motor* m_rightmotor;//れふと

mode_lookup nowMode;
bool ret;//しっぽの状態

/*
 * ルックアップゲート用関数
 * 返り値：true成功，false失敗
 */

bool lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail,Clock* clock){

	//モードの初期化
	nowMode = TAIL_STANDUP;
	//必要な変数
	ret = false;
	int32_t motor_ang_l=0, motor_ang_r=0;
	int32_t gyro_angle, volt=0;
	int8_t pwm_L, pwm_R;
	int count = 0;

	//必要なインスタンスをappから貰う(残念ながら引数)
	//gyro,color,leftmotor,rightmotor,tail
	m_tail = tail;
	m_gyro = gyro;
	m_color = color;
	m_leftmotor = leftmotor;
	m_rightmotor = rightmotor;
	
	//モータリセット
	//m_leftmotor->reset();
	//m_rightmotor->reset();
	
	static float forward = 0;
	static float turn = 0;
	//しっぽを下げる
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eSlow);
		}
		
		motor_ang_l = m_leftmotor->getCount();
        motor_ang_r = m_rightmotor->getCount();
        gyro_angle = m_gyro->getAnglerVelocity();
        volt = ev3_battery_voltage_mV();
		balance_control(
	         	forward,
			    turn,
            	(float)gyro_angle,
        		(float)GYRO_OFFSET_PID,
            	(float)motor_ang_l,
            	(float)motor_ang_r,
            	(float)volt,
            	(int8_t *)&pwm_L,
            	(int8_t *)&pwm_R);
		
		if(ret){
			if(count < 250){
				forward = -10;
				turn = 0;
				count++;
			}
			else if(pwm_L < 0 && pwm_R < 0 && gyro_angle < 0){
				m_leftmotor->setPWM(0);
				m_rightmotor->setPWM(0);
				m_gyro->reset();
				balance_init();
				ret = false;
				clock->sleep(1200);
				count = 0;
				
				break;
			}
		}else{
			forward = 0;
			turn = 0;
		}
		
		m_leftmotor->setPWM(pwm_L);
        m_rightmotor->setPWM(pwm_R);
		clock->sleep(4);
		
	}
	
	count = 0;
	
	//中間の角度1
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_MIDLE, eSlow);
		}
		m_leftmotor->setPWM(5);
		m_rightmotor->setPWM(5);
		clock->sleep(4);
		count++;
		
		if(count > 1000){
			break;
		}
	}
	count = 0;
	
	//中間の角度2
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_MIDLE, eSlow);
		}
		m_leftmotor->setPWM(5);
		m_rightmotor->setPWM(5);
		clock->sleep(4);
		count++;
		
		if(count > 1000){
			break;
		}
	}
	count = 0;	

	//すすむ
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_LOOKUPGATE, eSlow);
		}
		m_leftmotor->setPWM(20);
		m_rightmotor->setPWM(20);
		clock->sleep(4);
		count++;
		
		if(count > 1000){
			break;
		}
	}
	count = 0;

	
	//中間の角度
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_MIDLE, eSlow);
		}
		m_leftmotor->setPWM(0);
		m_rightmotor->setPWM(0);
		clock->sleep(4);
		count++;
		
		if(count > 1000){
			break;
		}
	}
	count = 0;
	
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eSlow);
		}
		m_leftmotor->setPWM(0);
		m_rightmotor->setPWM(0);
		clock->sleep(4);
		count++;
		
		if(count > 1000){
			break;
		}
	}
	count = 0;
	
	
	
	//モータリセット
	//m_leftmotor->reset();
	//m_rightmotor->reset();
	//ジャイロリセット
	//m_gyro->reset();
	//balance_init();
	//ライントレース
	motor_ang_l = m_leftmotor->getCount();
	motor_ang_r = m_rightmotor->getCount();
	gyro_angle = m_gyro->getAnglerVelocity();
	volt = ev3_battery_voltage_mV();
	balance_control(
	         	forward,
			    turn,
            	(float)gyro_angle,
        		(float)GYRO_OFFSET_PID,
            	(float)motor_ang_l,
            	(float)motor_ang_r,
            	(float)volt,
            	(int8_t *)&pwm_L,
            	(int8_t *)&pwm_R);
	
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
			pwm_max = PWM_ABS_MAX_SLOW_LOOK;
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




