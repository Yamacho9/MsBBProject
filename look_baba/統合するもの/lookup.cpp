#include "lookup.h"


Motor* m_tail;//しっぽ
GyroSensor* m_gyro;//じゃいろ
ColorSensor* m_color;//からーせんさ
Motor* m_leftmotor;//らいと
Motor* m_rightmotor;//れふと
TouchSensor* m_touch;
SonarSensor* m_sonar;

mode_lookup nowMode;
bool ret;//しっぽの状態
int32_t angle;//しっぽの角度
int time_count;//カウント関数
bool stand;//ture :倒立走行ON | false :倒立走行OFF
bool time;//ture : 最初　false:最後

/*
 * ルックアップゲート用関数
 * 返り値：true成功，false失敗
 */
Mode lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail,Clock* clock,TouchSensor* touch, SonarSensor* sonar){

	//モードの初期化
	nowMode = INIT;
	//必要な変数
	ret = false;
	int32_t motor_ang_l=0, motor_ang_r=0;
	int32_t gyro_angle, volt=0;
	int8_t pwm_L, pwm_R;
	int time_count = 0;
	static float forward = 30;
	static float turn = 0;
	Mode Main_mode;
	int err;	//偏差
	float diff;	//偏差微分

	//必要なインスタンスをappから貰う(残念ながら引数)
	//gyro,color,leftmotor,rightmotor,tail
	m_tail = tail;
	m_gyro = gyro;
	m_color = color;
	m_leftmotor = leftmotor;
	m_rightmotor = rightmotor;
	m_touch = touch;
	m_sonar = sonar;

	//モータリセット
	//m_leftmotor->reset();
	//m_rightmotor->reset();
	
	//初期化
	init_lookup();
	angle = TAIL_ANGLE_DRIVE;
	stand = true;
	time = true;
	

	while(1){

		//モードごとの制御
		if(nowMode == INIT){//超音波センサから検出されるまでINIT
			if(sonar_alert() == 1){//障害物検知
				angle = TAIL_ANGLE_DRIVE;
				forward = 20;
				turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
				time_count++;
				if(time_count > 300){//一定時間経ったら次のモードへ
					nowMode = TAIL_STANDUP;
					init_lookup;
				}
			}
		}
		else if(nowMode == TAIL_STANDUP){//しっぽを途中(STANDUP)まで動かす
			angle = TAIL_ANGLE_STAND_UP;
			//しっぽを動かしている途中はLinetraceする
			forward = 0;
			turn = 0;
			stand = true;
			if(ret){//しっぽを動かし終わった
				if(time_count < 250){
					forward = -10;
					turn = 0;
					time_count++;
				}
				else if(pwm_L < 0 && pwm_R < 0 && gyro_angle < 0){
					//一定時間たつと倒立走行をやめる
					stand = false;
					m_leftmotor->setPWM(0);
					m_rightmotor->setPWM(0);
					clock->sleep(1300);
					init_lookup();
					//モードを変更
					nowMode = TAIL_MIDDLE;
				}
			}
		}
		else if(nowMode == TAIL_MIDDLE){//しっぽを途中(MIDDLE)まで動かす
			angle = TAIL_ANGLE_MIDLE;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			if(ret && time_count > 250){
				//一定時間経った，かつしっぽを動かし終わった
				init_lookup();
				//モードを変更
				nowMode = TAIL_MIDDLE2;
			}
		}
		else if(nowMode == TAIL_MIDDLE2){//しっぽを途中(MIDDLE2)まで動かす
			angle = TAIL_ANGLE_MIDLE2;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			if(ret && time_count > 250){
				//一定時間経った，かつしっぽを動かし終わった
				init_lookup();
				//モードを変更
				nowMode = TAIL_LOOKUP;
			}
		}
		else if(nowMode == TAIL_LOOKUP){//しっぽを(LOOKUP)まで動かす
			angle = TAIL_ANGLE_LOOKUPGATE;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			if(ret && time_count > 250){
				//一定時間経った，かつしっぽを動かし終わった
				init_lookup();
				//モードを変更
				nowMode = ADVANCE;
			}
		}
		else if(nowMode == ADVANCE){//前進する
			angle = TAIL_ANGLE_LOOKUPGATE;
			m_leftmotor->setPWM(10);
			m_rightmotor->setPWM(10);
			
			time_count++;
			if(ret && time_count > 1000){//一定時間経った1200
				init_lookup();
				//モードを変更
				nowMode = BACK;
			}
		}
		else if(nowMode == BACK){//バックする
			angle = TAIL_ANGLE_LOOKUPGATE-2;
			time_count++;
			if(/*ret && */time_count > 2000){//一定時間経った
				m_leftmotor->setPWM(10);
				m_rightmotor->setPWM(10);
				angle = TAIL_ANGLE_LOOKUPGATE;
				if(time_count > 3000){
				//モードを変更
				nowMode = ADVANCE2;
				init_lookup();
				}
			}
			else{
				m_leftmotor->setPWM(-10);
				m_rightmotor->setPWM(-10);
			}
		}
		else if(nowMode == ADVANCE2){//前進する
			angle = TAIL_ANGLE_LOOKUPGATE;
			//m_leftmotor->setPWM(10);
			//m_rightmotor->setPWM(10);
			//time_count++;
			//if(/*ret &&*/ time_count > 2500){//一定時間経った
				init_lookup();
				//モードを変更
				nowMode = TAIL_MIDDLE2_2;
			//}
		}
		else if(nowMode == TAIL_MIDDLE2_2){//しっぽを途中(MIDDLE2)まで動かす
			angle = TAIL_ANGLE_MIDLE2;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			time = false;
			if(ret && time_count > 100){
				//一定時間経った，かつしっぽを動かし終わった
				init_lookup();
				//モードを変更
				nowMode = TAIL_MIDDLE_2;
			}
		}
		else if(nowMode == TAIL_MIDDLE_2){//しっぽを途中(MIDDLE)まで動かす
			angle = TAIL_ANGLE_MIDLE;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			time = false;
			if(ret && time_count > 100){
				//一定時間経った，かつしっぽを動かし終わった
				init_lookup();
				//モードを変更
				nowMode = TAIL_STANDUP_2;
			}
		}
		else if(nowMode == TAIL_STANDUP_2){//しっぽをSTANDまで動かす
			angle = TAIL_ANGLE_STAND_UP;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			time = false;
			if(ret && time_count > 100){
				//一定時間経った，かつしっぽを動かし終わった
				init_lookup();
				//倒立走行を初期化する
				balance_init();
				m_leftmotor->reset();
				m_rightmotor->reset();
				
				//モードを変更
				nowMode = STANDUP;
			}
		}
		else if(nowMode == STANDUP){//倒立走行
			//倒立走行ONにする
			stand = false;
			forward = 0;
			turn = 0;
			time_count++;
			if(time_count > 300){//一定時間経ったら，次のモードへ
				init_lookup();
				nowMode = END;
			}
		}
		else if(nowMode == END){
			//しっぽをバランス走行時に戻す
			angle = TAIL_ANGLE_DRIVE;
			time_count++;
			if(ret && time_count > 300){
				init_lookup();
				Main_mode = eGarageIn;
				break;//ループから抜ける
			}
		}
		else{//不正な値は終了
			Main_mode = eEnd;
			break;
		}

		/*以下はモードにかかわらず必ず通る*/

		//しっぽの操作
		if(!ret){
			if(!time){//最後のしっぽを下げる動作は早く
				ret = tail_ctr(angle, eFast);
			}
			else{//最初の上げる動作は遅く
				ret = tail_ctr(angle, eSlow);
			}
			
		}
		//モータ
		if(stand){//倒立走行する
			motor_ang_l = m_leftmotor->getCount();
			motor_ang_r = m_rightmotor->getCount();
			gyro_angle = m_gyro->getAnglerVelocity();
			volt = ev3_battery_voltage_mV();
			balance_control(forward,turn,(float)gyro_angle,(float)GYRO_OFFSET_PID,
			(float)motor_ang_l,(float)motor_ang_r,(float)volt,(int8_t *)&pwm_L,(int8_t *)&pwm_R);
			
			m_leftmotor->setPWM(pwm_L);
			m_rightmotor->setPWM(pwm_R);

		}
		else{//倒立走行しない
		}
		
		if(m_touch->isPressed()){
			Main_mode = eEnd;
			break;
		}
		if(ev3_button_is_pressed(BACK_BUTTON)){
			Main_mode = eEnd;
			break;
		}
		
		clock->sleep(4);//4ms周期

	}

	return Main_mode;
	

}

//しっぽコントロール
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
			pwm_max = PWM_ABS_MAX_FAST_LOOK;
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

//グローバル変数を初期化する関数
void init_lookup(){
	ret = false;
	time_count = 0;
}


//*****************************************************************************
// 関数名 : sonar_alert
// 引数 : 無し
// 返り値 : 1(障害物あり)/0(障害物無し)
// 概要 : 超音波センサによる障害物検知
//*****************************************************************************
static int32_t sonar_alert(void)
{
    static uint32_t counter = 0;
    static int32_t alert = 0;

    int32_t distance;

    if (++counter == 40/4) /* 約40msec周期毎に障害物検知  */
    {
        /*
         * 超音波センサによる距離測定周期は、超音波の減衰特性に依存します。
         * NXTの場合は、40msec周期程度が経験上の最短測定周期です。
         * EV3の場合は、要確認
         */
        distance = m_sonar->getDistance();
        if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
        {
            alert = 1; /* 障害物を検知 */
        }
        else
        {
            alert = 0; /* 障害物無し */
        }
        counter = 0;
    }

    return alert;
}