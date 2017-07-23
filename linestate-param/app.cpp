/**
 ******************************************************************************
 ** �t�@�C���� : app.cpp
 **
 ** �T�v : Bluetooth�X�^�[�g �{ ���C���g���[�X�iPID����j+�L�����u���[�V����
 **
 ** ���L : �������
 ******************************************************************************
 **/

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
#include "CalcDistanceAndDirection.h"

using namespace ev3api;

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif


/*�O���[�o���ϐ�*/
/*���{�b�g�ɕ�����\�����邽�߂̃O���[�o���ϐ�*/
int count;
static char message[MESSAGE_LEN + 1] = {0};
/*�X�s�[�h���グ�邽�߂̃O���[�o���ϐ�*/
int speed_count=0;
int speed=0;
/*PID����̕΍������e�[�u��*/
int errorList[INT_NUM];	//�΍������e�[�u��
/* Bluetooth */
static int32_t   bt_cmd = 0;      /* Bluetooth�R�}���h 1:�����[�g�X�^�[�g */
static FILE     *bt = NULL;      /* Bluetooth�t�@�C���n���h�� */

/*�֐��̃v���g�^�C�v�錾*/
//���b�Z�[�W�������֐�
static void Message(const char* str);
//�e�Z���T�̏�����������֐�
static void Init();
//�����g�Z���T
static int32_t sonar_alert(void);
//�����ۃR���g���[��
static bool tail_control(int32_t angle, tailSpeed sp);


/* �I�u�W�F�N�g�ւ̃|�C���^��` */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;


/* ���C���^�X�N */
void main_task(intptr_t unused)
{
    int8_t forward;      /* �O��i���� */
	float turn;         /* ���񖽗� */
	int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int8_t cur_brightness=0;	/* ���o�������Z���T�l */

	int i;
	for (i = 0; i < INT_NUM; i++) { //�e�[�u��������
		errorList[i] = 0;
	}
	int nextErrorIndex = 0;	//���̕ύX�����̃C���f�b�N�X
	int max=255;//�L�����u���[�V�����̍ő�l
	int min=0;//�L�����u���[�V�����̍ŏ��l
	bool ret = false;

	/*�O���[�o���ϐ��̏�����*/
	count = 1;
	
	/*�e�Z���T�̃|�[�g�ݒ�*/
	Init();
	
	/* �K�����[�^�[�̃��Z�b�g */
    tailMotor->reset();

	
    /* Open Bluetooth file */
	/*�V���A���|�[�g���J��*/
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);
	Message("bluetooth serial port open");
    
    /* Bluetooth�ʐM�^�X�N�̋N�� */
	act_tsk(BT_TASK);
	Message("Bluetooth task Start");
	
    ev3_led_set_color(LED_ORANGE); /* �����������ʒm */
	Message("Init finished.");
	
	//�L�����u���[�V����
	//min,max�ɃL�����u���[�V�����̌��ʂ��o�͂����
	Message("Calibration waiting..");
	Message("push touch sensor : do calibration");
	Message("push back button : don't calibration");
	Calibration(&min, &max, colorSensor, leftMotor, rightMotor, gyroSensor, tailMotor, touchSensor, clock);
	fprintf(bt,"Calibration result\nmax:%d min:%d",max,min);
	
    ev3_led_set_color(LED_GREEN); /* �X�^�[�g�ʒm */
	

	//bluetooth start
	Message("bluetooth start waiting...");
	while(1){
		if(!ret){
			ret = tail_control(TAIL_ANGLE_STAND_UP, eSlow);
		}
		if (bt_cmd == 1){//bluetooth start
			fprintf(bt,"bluetooth start");
    		break;
    	}
		if (touchSensor->isPressed())
        {
		 	Message("touch sensor start");
            break; /* �^�b�`�Z���T�������ꂽ */
        }
		clock->sleep(10);
	}
	
	/* ���s���[�^�[�G���R�[�_�[���Z�b�g */
    leftMotor->reset();
    rightMotor->reset();
	
	/* �W���C���Z���T�[���Z�b�g */
    gyroSensor->reset();
    balance_init(); /* �|���U�qAPI������ */

	/* ���s�̂̏�Ԃ��N���� */
	while(1)
	{
		float pwm = (float)(TAIL_ANGLE_START - tailMotor->getCount()); // ��ᐧ��
		if (pwm > 0)
		{
			tailMotor->setPWM(20);
		}
		else if (pwm < 0)
		{
			break;
		}
		clock->sleep(4);
	}
	ret = false;
	
    /**
    * ���C�����[�v
    */
    while(1)
    {
    	int32_t motor_ang_l=0, motor_ang_r=0;
		int32_t gyro, volt=0;
    	int target=0;
    	int distance, direction; //���s�����A����
    	
    	if (ev3_button_is_pressed(BACK_BUTTON)){
    		//backbutton���������ƏI��
    		Message("finished...");
    		break;
    	}
		if (touchSensor->isPressed())
		{ 
			// �^�b�`�Z���T���������ƏI��
			Message("finished...");
			break;
		}
    	
    	
        if(!ret){
        	/* �o�����X���s�p�p�x�ɐ��� */
			ret = tail_control(TAIL_ANGLE_DRIVE, eFast);
		}

        if (sonar_alert() == 1) /* ��Q�����m */
        {
			forward = turn = 0; /* ��Q�������m�������~ */
		}
        else
        {
        	/*
        	//3s��ɑ��x��45�ɓ��B����悤�ɏ���������������
            forward = 10 + speed;
        	speed_count = speed_count + 464;
        	if(speed_count > 10000){
        		if(speed < 35){
        			speed++;
        		}
        		speed_count = speed_count - 10000;
        	}
        	*/
        	//forward = 10; /* �O�i���� */
			cur_brightness = colorSensor->getBrightness();
        	target = (max + min)/2;
        	//turn�l��forward���Ԃ�l
			turn = LineTrace(LINESTATUS_DEFAULT, target, cur_brightness, DELTA_T, errorList, nextErrorIndex, &forward);
		}

        /* �|���U�q����API �ɓn���p�����[�^���擾���� */
        motor_ang_l = leftMotor->getCount();
        motor_ang_r = rightMotor->getCount();
        gyro = gyroSensor->getAnglerVelocity();
        volt = ev3_battery_voltage_mV();

        /* �|���U�q����API���Ăяo���A�|�����s���邽�߂� */
        /* ���E���[�^�o�͒l�𓾂� */
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

#if 0
		int angle;
    	angle = (motor_ang_l + motor_ang_r) / 2;	// ���[�^�[�̌��o�p�x�i�ݐϒl�j
    	distance = angle * PI * DIAMETER / 360;		// ����
    	direction = (motor_ang_l % (360 * 4) - motor_ang_r % (360 * 4)) / 4; // �����i�X�^�[�g���̌�����0�x�Ƃ��āA���v���̊p�x�j
#endif
#if 1
    	distance = 0;
    	direction = 0;
		CalcDistanceAndDirection(motor_ang_l, motor_ang_r, &distance, &direction);
#endif
    	fprintf(bt, "distance = %d, direction = %d\n", distance, direction);
    	fprintf(bt, "cur_brightness = %d, turn = %f, forward = %d\n", cur_brightness, turn, forward);
        clock->sleep(4); /* 4msec�����N�� */
    }
    leftMotor->reset();
    rightMotor->reset();
	tailMotor->reset();

	/*�I������*/
    ter_tsk(BT_TASK);
    fclose(bt);

    ext_tsk();
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
        distance = sonarSensor->getDistance();
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

//*****************************************************************************
// �֐��� : tail_control
// ���� : angle (���[�^�ڕW�p�x[�x])
// �Ԃ�l : ����
// �T�v : ���s�̊��S��~�p���[�^�̊p�x����
//*****************************************************************************
static bool tail_control(int32_t angle, tailSpeed sp)
{
	float pwm_max;
	float pwm = (float)(angle - tailMotor->getCount()) * P_GAIN; // ��ᐧ��
	if (pwm<0.1 && pwm >-0.1){
		tailMotor->setBrake(true);
		tailMotor->setPWM(0);
		return true;
	}
	else{
		tailMotor->setBrake(false);
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
		tailMotor->setPWM(pwm);
		return false;
	}

}

//*****************************************************************************
// �֐��� : bt_task
// ���� : unused
// �Ԃ�l : �Ȃ�
// �T�v : 
//*****************************************************************************
void bt_task(intptr_t unused)
{
	/*�ʐM����*/
	while(1){
		//��M
		char c = fgetc(bt);
		if(c != EOF){
			fprintf(bt,"%c",c);
		}
		
		switch(c){
		case '1':
			bt_cmd = 1;
			break;
		default:
			break;
		}
		
		clock->sleep(5);
		
	}
	
}

//*******************************************************************
// �֐��� : display
// ���� : �Ȃ�
// �Ԃ�l : �Ȃ�
// �T�v : ��Ԃ�\������
//*******************************************************************
void display()
{
  ev3_lcd_set_font(EV3_FONT_SMALL);
  ev3_lcd_draw_string("Program is running", 10, 30);
  ev3_lcd_set_font(EV3_FONT_MEDIUM);
  ev3_lcd_draw_string(message, 10, 40);
}

//*******************************************************************
// �֐��� : Message
// ���� : str(�\��������������)
// �Ԃ�l : �Ȃ�
// �T�v : 
//*******************************************************************
void Message(const char* str){
	ev3_lcd_draw_string(str, 0, CALIB_FONT_HEIGHT*count);
	count++;
}


//*******************************************************************
// �֐��� : Init
// ���� : �Ȃ�
// �Ԃ�l : �Ȃ�
// �T�v : 
//*******************************************************************
void Init(){

	/* �e�I�u�W�F�N�g�𐶐��E���������� */
    touchSensor = new TouchSensor(PORT_1);
    colorSensor = new ColorSensor(PORT_3);
    sonarSensor = new SonarSensor(PORT_2);
    gyroSensor  = new GyroSensor(PORT_4);
    leftMotor   = new Motor(PORT_C);
    rightMotor  = new Motor(PORT_B);
    tailMotor   = new Motor(PORT_A);
    clock       = new Clock();
}

