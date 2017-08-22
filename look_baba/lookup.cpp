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

bool lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail){

	//�K�v�Ȓ萔
	int8_t forward;      /* �O��i���� */
	int8_t hoge;
	float turn;         /* ���񖽗� */
	int lastErr=0; //�O��΍�
	int8_t cur_brightness=0;	/* ���o�������Z���T�l */
	bool ret = false;/*�����ۂ̏��*/
	int min;
	int max;
	getCalibration_pram(&min,&max);

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
			break;
		}
	}
	//�|�����sOFF
	m_gyro->reset();
	balance_init();

	//�����ۂ��������悤�ɂ���ɂ�����肳����
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

	//���C���ɂ����ăQ�[�g��������
	//���C���g���[�X����
	forward = 30;
	//���C���g���[�X
	for(int i=0;i<1000;i++){
		int target=0;
		cur_brightness = m_color->getBrightness();
        	target = (max + min)/2; 
		turn = LineTrace(hoge, target, cur_brightness, DELTA_T, &lastErr, &hoge);
		m_leftmotor->setPWM(forward-turn);
		m_rightmotor->setPWM(forward+turn);
	}

	//�����ۂ�������i���Ă�Ƃ��܂Łj
	while(1){
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_STAND_UP, eSlow);
		}
		else{
			//�����ۂ���������I���
			ret = false;
			break;
		}
	}

	//�|�����sOFF�C���x0�ő���
	//���C���g���[�X����
	forward = 0;
	//���C���g���[�X
	for(int i=0;i<1000;i++){
		int target = 0;
		cur_brightness = m_color->getBrightness();
        	target = (max + min)/2; 
		turn = LineTrace(hoge, target, cur_brightness, DELTA_T, &lastErr, &hoge);
		m_leftmotor->setPWM(forward-turn);
		m_rightmotor->setPWM(forward+turn);
	}
	
	//�����ۂ��グ�Ȃ��烉�C���g���[�X���J�n
	while(1){
		//�����ۏグ��
		if(!ret){
			ret = tail_ctr(TAIL_ANGLE_DRIVE, eSlow);
		}
		else{
			//�����ۂ���������I���
			ret = false;
			break;
		}
	}

	//�|�����s��ON�ɂ���
	m_gyro->reset();
	balance_init();

	return true;//����

}

bool standuptail(){

	//�����ۂŌ��Ă�Ƃ���܂ł����ۂ�������
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



