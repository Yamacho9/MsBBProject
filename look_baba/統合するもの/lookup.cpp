#include "lookup.h"


Motor* m_tail;//������
GyroSensor* m_gyro;//���Ⴂ��
ColorSensor* m_color;//����[����
Motor* m_leftmotor;//�炢��
Motor* m_rightmotor;//��ӂ�
TouchSensor* m_touch;
SonarSensor* m_sonar;

mode_lookup nowMode;
bool ret;//�����ۂ̏��
int32_t angle;//�����ۂ̊p�x
int time_count;//�J�E���g�֐�
bool stand;//ture :�|�����sON | false :�|�����sOFF
bool time;//ture : �ŏ��@false:�Ō�

/*
 * ���b�N�A�b�v�Q�[�g�p�֐�
 * �Ԃ�l�Ftrue�����Cfalse���s
 */
Mode lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail,Clock* clock,TouchSensor* touch, SonarSensor* sonar){

	//���[�h�̏�����
	nowMode = INIT;
	//�K�v�ȕϐ�
	ret = false;
	int32_t motor_ang_l=0, motor_ang_r=0;
	int32_t gyro_angle, volt=0;
	int8_t pwm_L, pwm_R;
	int time_count = 0;
	static float forward = 30;
	static float turn = 0;
	Mode Main_mode;
	int err;	//�΍�
	float diff;	//�΍�����

	//�K�v�ȃC���X�^���X��app����Ⴄ(�c�O�Ȃ������)
	//gyro,color,leftmotor,rightmotor,tail
	m_tail = tail;
	m_gyro = gyro;
	m_color = color;
	m_leftmotor = leftmotor;
	m_rightmotor = rightmotor;
	m_touch = touch;
	m_sonar = sonar;

	//���[�^���Z�b�g
	//m_leftmotor->reset();
	//m_rightmotor->reset();
	
	//������
	init_lookup();
	angle = TAIL_ANGLE_DRIVE;
	stand = true;
	time = true;
	

	while(1){

		//���[�h���Ƃ̐���
		if(nowMode == INIT){//�����g�Z���T���猟�o�����܂�INIT
			if(sonar_alert() == 1){//��Q�����m
				angle = TAIL_ANGLE_DRIVE;
				forward = 20;
				turn = LineTrace(1, target, cur_brightness, DELTA_T, &lastErr, &forward, &err, &diff);
				time_count++;
				if(time_count > 300){//��莞�Ԍo�����玟�̃��[�h��
					nowMode = TAIL_STANDUP;
					init_lookup;
				}
			}
		}
		else if(nowMode == TAIL_STANDUP){//�����ۂ�r��(STANDUP)�܂œ�����
			angle = TAIL_ANGLE_STAND_UP;
			//�����ۂ𓮂����Ă���r����Linetrace����
			forward = 0;
			turn = 0;
			stand = true;
			if(ret){//�����ۂ𓮂����I�����
				if(time_count < 250){
					forward = -10;
					turn = 0;
					time_count++;
				}
				else if(pwm_L < 0 && pwm_R < 0 && gyro_angle < 0){
					//��莞�Ԃ��Ɠ|�����s����߂�
					stand = false;
					m_leftmotor->setPWM(0);
					m_rightmotor->setPWM(0);
					clock->sleep(1300);
					init_lookup();
					//���[�h��ύX
					nowMode = TAIL_MIDDLE;
				}
			}
		}
		else if(nowMode == TAIL_MIDDLE){//�����ۂ�r��(MIDDLE)�܂œ�����
			angle = TAIL_ANGLE_MIDLE;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			if(ret && time_count > 250){
				//��莞�Ԍo�����C�������ۂ𓮂����I�����
				init_lookup();
				//���[�h��ύX
				nowMode = TAIL_MIDDLE2;
			}
		}
		else if(nowMode == TAIL_MIDDLE2){//�����ۂ�r��(MIDDLE2)�܂œ�����
			angle = TAIL_ANGLE_MIDLE2;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			if(ret && time_count > 250){
				//��莞�Ԍo�����C�������ۂ𓮂����I�����
				init_lookup();
				//���[�h��ύX
				nowMode = TAIL_LOOKUP;
			}
		}
		else if(nowMode == TAIL_LOOKUP){//�����ۂ�(LOOKUP)�܂œ�����
			angle = TAIL_ANGLE_LOOKUPGATE;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			if(ret && time_count > 250){
				//��莞�Ԍo�����C�������ۂ𓮂����I�����
				init_lookup();
				//���[�h��ύX
				nowMode = ADVANCE;
			}
		}
		else if(nowMode == ADVANCE){//�O�i����
			angle = TAIL_ANGLE_LOOKUPGATE;
			m_leftmotor->setPWM(10);
			m_rightmotor->setPWM(10);
			
			time_count++;
			if(ret && time_count > 1000){//��莞�Ԍo����1200
				init_lookup();
				//���[�h��ύX
				nowMode = BACK;
			}
		}
		else if(nowMode == BACK){//�o�b�N����
			angle = TAIL_ANGLE_LOOKUPGATE-2;
			time_count++;
			if(/*ret && */time_count > 2000){//��莞�Ԍo����
				m_leftmotor->setPWM(10);
				m_rightmotor->setPWM(10);
				angle = TAIL_ANGLE_LOOKUPGATE;
				if(time_count > 3000){
				//���[�h��ύX
				nowMode = ADVANCE2;
				init_lookup();
				}
			}
			else{
				m_leftmotor->setPWM(-10);
				m_rightmotor->setPWM(-10);
			}
		}
		else if(nowMode == ADVANCE2){//�O�i����
			angle = TAIL_ANGLE_LOOKUPGATE;
			//m_leftmotor->setPWM(10);
			//m_rightmotor->setPWM(10);
			//time_count++;
			//if(/*ret &&*/ time_count > 2500){//��莞�Ԍo����
				init_lookup();
				//���[�h��ύX
				nowMode = TAIL_MIDDLE2_2;
			//}
		}
		else if(nowMode == TAIL_MIDDLE2_2){//�����ۂ�r��(MIDDLE2)�܂œ�����
			angle = TAIL_ANGLE_MIDLE2;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			time = false;
			if(ret && time_count > 100){
				//��莞�Ԍo�����C�������ۂ𓮂����I�����
				init_lookup();
				//���[�h��ύX
				nowMode = TAIL_MIDDLE_2;
			}
		}
		else if(nowMode == TAIL_MIDDLE_2){//�����ۂ�r��(MIDDLE)�܂œ�����
			angle = TAIL_ANGLE_MIDLE;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			time = false;
			if(ret && time_count > 100){
				//��莞�Ԍo�����C�������ۂ𓮂����I�����
				init_lookup();
				//���[�h��ύX
				nowMode = TAIL_STANDUP_2;
			}
		}
		else if(nowMode == TAIL_STANDUP_2){//�����ۂ�STAND�܂œ�����
			angle = TAIL_ANGLE_STAND_UP;
			m_leftmotor->setPWM(1);
			m_rightmotor->setPWM(1);
			time_count++;
			time = false;
			if(ret && time_count > 100){
				//��莞�Ԍo�����C�������ۂ𓮂����I�����
				init_lookup();
				//�|�����s������������
				balance_init();
				m_leftmotor->reset();
				m_rightmotor->reset();
				
				//���[�h��ύX
				nowMode = STANDUP;
			}
		}
		else if(nowMode == STANDUP){//�|�����s
			//�|�����sON�ɂ���
			stand = false;
			forward = 0;
			turn = 0;
			time_count++;
			if(time_count > 300){//��莞�Ԍo������C���̃��[�h��
				init_lookup();
				nowMode = END;
			}
		}
		else if(nowMode == END){
			//�����ۂ��o�����X���s���ɖ߂�
			angle = TAIL_ANGLE_DRIVE;
			time_count++;
			if(ret && time_count > 300){
				init_lookup();
				Main_mode = eGarageIn;
				break;//���[�v���甲����
			}
		}
		else{//�s���Ȓl�͏I��
			Main_mode = eEnd;
			break;
		}

		/*�ȉ��̓��[�h�ɂ�����炸�K���ʂ�*/

		//�����ۂ̑���
		if(!ret){
			if(!time){//�Ō�̂����ۂ������铮��͑���
				ret = tail_ctr(angle, eFast);
			}
			else{//�ŏ��̏グ�铮��͒x��
				ret = tail_ctr(angle, eSlow);
			}
			
		}
		//���[�^
		if(stand){//�|�����s����
			motor_ang_l = m_leftmotor->getCount();
			motor_ang_r = m_rightmotor->getCount();
			gyro_angle = m_gyro->getAnglerVelocity();
			volt = ev3_battery_voltage_mV();
			balance_control(forward,turn,(float)gyro_angle,(float)GYRO_OFFSET_PID,
			(float)motor_ang_l,(float)motor_ang_r,(float)volt,(int8_t *)&pwm_L,(int8_t *)&pwm_R);
			
			m_leftmotor->setPWM(pwm_L);
			m_rightmotor->setPWM(pwm_R);

		}
		else{//�|�����s���Ȃ�
		}
		
		if(m_touch->isPressed()){
			Main_mode = eEnd;
			break;
		}
		if(ev3_button_is_pressed(BACK_BUTTON)){
			Main_mode = eEnd;
			break;
		}
		
		clock->sleep(4);//4ms����

	}

	return Main_mode;
	

}

//�����ۃR���g���[��
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
		
		// PWM�o�͖O�a����
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

//�O���[�o���ϐ�������������֐�
void init_lookup(){
	ret = false;
	time_count = 0;
}


//*****************************************************************************
// �֐��� : sonar_alert
// ���� : ����
// �Ԃ�l : 1(��Q������)/0(��Q������)
// �T�v : �����g�Z���T�ɂ���Q�����m
//*****************************************************************************
static int32_t sonar_alert(void)
{
    static uint32_t counter = 0;
    static int32_t alert = 0;

    int32_t distance;

    if (++counter == 40/4) /* ��40msec�������ɏ�Q�����m  */
    {
        /*
         * �����g�Z���T�ɂ�鋗����������́A�����g�̌��������Ɉˑ����܂��B
         * NXT�̏ꍇ�́A40msec�������x���o����̍ŒZ��������ł��B
         * EV3�̏ꍇ�́A�v�m�F
         */
        distance = m_sonar->getDistance();
        if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
        {
            alert = 1; /* ��Q�������m */
        }
        else
        {
            alert = 0; /* ��Q������ */
        }
        counter = 0;
    }

    return alert;
}