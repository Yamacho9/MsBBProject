#include "lookup.h"


Motor* m_tail;//������
GyroSensor* m_gyro;//���Ⴂ��
ColorSensor* m_color;//����[����
Motor* m_leftmotor;//�炢��
Motor* m_rightmotor;//��ӂ�

/*
 * ���b�N�A�b�v�Q�[�g�p�֐�
 * �Ԃ�l�Ftrue�����Cfalse���s
 */

bool lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail,Clock* clock){

	//�K�v�ȕϐ�
	bool ret = false;//�����ۂ̏��

	//�K�v�ȃC���X�^���X��app����Ⴄ(�c�O�Ȃ������)
	//gyro,color,leftmotor,rightmotor,tail
	m_tail = tail;
	m_gyro = gyro;
	m_color = color;
	m_leftmotor = leftmotor;
	m_rightmotor = rightmotor;
	
	
	//�����ۂ�������i���Ă�Ƃ��܂Łj
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eSlow);
		}
		else{
			//�����ۂ���������I���
			ret = false;
			//�|�����sOFF
			//m_gyro->reset();
			//balance_init();
			break;
		}
	}
	//�o�b�N����B�i���Ⴂ�낪�}�C�i�X�ɂȂ�܂Łj
	while(1){
		//�����ۂ��Œ�
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eFast);
		}
		if(m_gyro->getAngle() < 0){//���Ⴂ�낪�}�C�i�X
			//�|�����sOFF
			m_gyro->reset();
			balance_init();
			break;
		}
		m_leftmotor->setPWM(-5);
		m_rightmotor->setPWM(-5);
	}
	
	//���x0�ɂ���
	for(int i=0;i<2500;i++){
		//�����ۂ��Œ�
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eFast);
		}
		
		m_leftmotor->setPWM(0);
		m_rightmotor->setPWM(0);
		
		clock->sleep(4);
	}
	
	//�������Ƃ���܂ł����ۂ𓮂���
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_LOOKUPGATE, eSlow);
		}
		else{
			//�����ۂ���������I���
			ret = false;
			break;
		}
	}
	
	//���x10�őO��
	for(int i=0;i<5000;i++){
		//�����ۂ��Œ�
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_LOOKUPGATE, eFast);//��ŕύX
		}
		m_leftmotor->setPWM(10);
		m_rightmotor->setPWM(10);
		
		clock->sleep(4);
	}
	//�Ƃ肠�����������[�v
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_LOOKUPGATE, eFast);//��ŕύX
		}
		m_leftmotor->setPWM(0);
		m_rightmotor->setPWM(0);
		clock->sleep(4);
	}


	return true;//����

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



