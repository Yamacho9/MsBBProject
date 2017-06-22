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

using namespace ev3api;

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/* Bluetooth */
static int32_t   bt_cmd = 0;      /* Bluetooth�R�}���h 1:�����[�g�X�^�[�g */
static FILE     *bt = NULL;      /* Bluetooth�t�@�C���n���h�� */

/* ���L�̃}�N���͌�/���ɍ��킹�ĕύX����K�v������܂� */
#define GYRO_OFFSET           0  /* �W���C���Z���T�I�t�Z�b�g�l(�p���x0[deg/sec]��) */
#define SONAR_ALERT_DISTANCE 30  /* �����g�Z���T�ɂ���Q�����m����[cm] */
#define TAIL_ANGLE_STAND_UP  93  /* ���S��~���̊p�x[�x] */
#define TAIL_ANGLE_DRIVE      3  /* �o�����X���s���̊p�x[�x] */
#define TAIL_ANGLE_INIT       0  /* 0�x */
#define P_GAIN             2.5F  /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60  /* ���S��~�p���[�^����PWM��΍ő�l */
//#define DEVICE_NAME     "ET0"  /* Bluetooth�� hrp2/target/ev3.h BLUETOOTH_LOCAL_NAME�Őݒ� */
//#define PASS_KEY        "1234" /* �p�X�L�[    hrp2/target/ev3.h BLUETOOTH_PIN_CODE�Őݒ� */
#define CMD_START         '1'    /* �����[�g�X�^�[�g�R�}���h */
#define TARGET				35	 //���C���g���[�X���� ���ʃ^�[�Q�b�g�l
#define DELTA_T				0.004 //���������is�j
#define INT_NUM				250	//�ϕ�����΍���(1s��)

/* LCD�t�H���g�T�C�Y */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)
#define MESSAGE_LEN 8

/* �֐��v���g�^�C�v�錾 */
static int32_t sonar_alert(void);
static void tail_control(int32_t angle);
//���b�Z�[�W�������֐�
static void Message(const char* str);
//��Ԃ�\������֐�
void display();
//�e�Z���T�̏�����������֐�
static void Init();
//�L�����u���[�V����
void Calibration(int* min, int* max, Motor* left, Motor* right, GyroSensor* gyroSen, TouchSensor* touch);

/* �I�u�W�F�N�g�ւ̃|�C���^��` */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;

/*�\�����邽�߂̃O���[�o���ϐ�*/
int count;
static char message[MESSAGE_LEN + 1] = {0};

/* ���C���^�X�N */
void main_task(intptr_t unused)
{
    int8_t forward;      /* �O��i���� */
	float turn;         /* ���񖽗� */
	int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int8_t cur_brightness;	/* ���o�������Z���T�l */
	int errorList[INT_NUM];	//�΍������e�[�u��
	int i;
	for (i = 0; i < INT_NUM; i++) { //�e�[�u��������
		errorList[i] = 0;
	}
	int nextErrorIndex = 0;	//���̕ύX�����̃C���f�b�N�X
	int max=-255;//�L�����u���[�V�����̍ő�l
	int min=255;//�L�����u���[�V�����̍ŏ��l

	/*�O���[�o���ϐ��̏�����*/
	count = 1;
	
	/*�e�Z���T�̃|�[�g�ݒ�*/
	Init();
	
	/* �K�����[�^�[�̃��Z�b�g */
    tailMotor->reset();
	tail_control(TAIL_ANGLE_INIT); /* 0�x�ɐ��� */

	
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
	
	//bluetooth start
	Message("bluetooth start waiting...");
	while(1){
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
	
	//�L�����u���[�V����
	//min,max�ɃL�����u���[�V�����̌��ʂ��o�͂����
	//Calibration(&min, &max);
	Calibration(&min, &max, leftMotor, rightMotor, gyroSensor, touchSensor);

    ev3_led_set_color(LED_GREEN); /* �X�^�[�g�ʒm */
	
	/* ���s���[�^�[�G���R�[�_�[���Z�b�g */
    leftMotor->reset();
    rightMotor->reset();
    
    /* �W���C���Z���T�[���Z�b�g */
    gyroSensor->reset();
    balance_init(); /* �|���U�qAPI������ */
	
    /**
    * ���C�����[�v
    */
    while(1)
    {
    	if (ev3_button_is_pressed(BACK_BUTTON)){
    		//backbutton���������ƏI��
    		Message("finished...");
    		break;
    	}
    	
        tail_control(TAIL_ANGLE_DRIVE); /* �o�����X���s�p�p�x�ɐ��� */

        if (sonar_alert() == 1) /* ��Q�����m */
        {
			forward = turn = 0; /* ��Q�������m�������~ */
		}
        else
        {
            forward = 45; /* �O�i���� */
			cur_brightness = colorSensor->getBrightness();
			turn = LineTrace(TARGET, cur_brightness, DELTA_T, errorList, nextErrorIndex);
			fprintf(bt, "cur_brightness = %d, turn = %f\n", cur_brightness, turn);
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
            (float)GYRO_OFFSET,
            (float)motor_ang_l,
            (float)motor_ang_r,
            (float)volt,
            (int8_t *)&pwm_L,
            (int8_t *)&pwm_R);

        leftMotor->setPWM(pwm_L);
        rightMotor->setPWM(pwm_R);

        clock->sleep(4); /* 4msec�����N�� */
    }
    leftMotor->reset();
    rightMotor->reset();

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
static void tail_control(int32_t angle)
{
    float pwm = (float)(angle - tailMotor->getCount()) * P_GAIN; /* ��ᐧ�� */
    /* PWM�o�͖O�a���� */
    if (pwm > PWM_ABS_MAX)
    {
        pwm = PWM_ABS_MAX;
    }
    else if (pwm < -PWM_ABS_MAX)
    {
        pwm = -PWM_ABS_MAX;
    }

    tailMotor->setPWM(pwm);
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

//******
// Calibration
// ���́F�Ȃ�
// �o�́Fint min,int max
//******
void Calibration(int* min, int* max, ev3api::Motor* left, ev3api::Motor* right, ev3api::GyroSensor* gyroSen, ev3api::TouchSensor* touch){
	int8_t cur_brightness;	/* ���o�������Z���T�l */
	int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	
		/* �L�����u���[�V�����ҋ@ */
    while(1)
    {
        tail_control(TAIL_ANGLE_STAND_UP); /* ���S��~�p�p�x�ɐ��� */
        if (touchSensor->isPressed())
        {
            break; /* �^�b�`�Z���T�������ꂽ */
        }
        clock->sleep(10);
        
    }
    
    /* ���s���[�^�[�G���R�[�_�[���Z�b�g */
    left->reset();
    right->reset();
    
    /* �W���C���Z���T�[���Z�b�g */
    gyroSen->reset();
    balance_init(); /* �|���U�qAPI������ */
	
    ev3_led_set_color(LED_GREEN); /* �X�^�[�g�ʒm */

    /**
    * Main loop for the self-balance control algorithm
    */
	//clock_t start = clock();    // �X�^�[�g����
	int forward = 23; /* �O�i���� */
	int turn = 0;
	int count=0, count2=0;
	//*min=255;
	//*max=-255;
    while(1)
    {

        if (ev3_button_is_pressed(BACK_BUTTON)) break;

        tail_control(TAIL_ANGLE_DRIVE); /* �o�����X���s�p�p�x�ɐ��� */

        if (sonar_alert() == 1) /* ��Q�����m */
        {
			forward = turn = 0; /* ��Q�������m�������~ */
		}
        else
        {
			cur_brightness = colorSensor->getBrightness();
			
			if(cur_brightness>=*max){
				*max = cur_brightness;
			}
			if(cur_brightness<=*min){
				*min = cur_brightness;
			}
			fprintf(bt, "max = %d, min = %d\n", *max, *min);
		}

        /* �|���U�q����API �ɓn���p�����[�^���擾���� */
        motor_ang_l = left->getCount();
        motor_ang_r = right->getCount();
        gyro = gyroSen->getAnglerVelocity();
        volt = ev3_battery_voltage_mV();


        /* �|���U�q����API���Ăяo���A�|�����s���邽�߂� */
        /* ���E���[�^�o�͒l�𓾂� */
        balance_control(
            (float)forward,
            (float)turn,
            (float)gyro,
            (float)GYRO_OFFSET,
            (float)motor_ang_l,
            (float)motor_ang_r,
            (float)volt,
            (int8_t *)&pwm_L,
            (int8_t *)&pwm_R);

        left->setPWM(pwm_L);
        right->setPWM(pwm_R);
    	
        clock->sleep(4); /* 4msec�����N�� */
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
