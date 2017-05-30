/**
 ******************************************************************************
 ** �t�@�C���� : app.cpp
 **
 ** �T�v : 2�֓|���U�q���C���g���[�X���{�b�g��TOPPERS/HRP2�pC++�T���v���v���O����
 **
 ** ���L : sample_cpp (���C���g���[�X/�K�����[�^/�����g�Z���T/�����[�g�X�^�[�g)
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
//#define LIGHT_WHITE          40  /* ���F�̌��Z���T�l */
//#define LIGHT_BLACK           0  /* ���F�̌��Z���T�l */
#define SONAR_ALERT_DISTANCE 30  /* �����g�Z���T�ɂ���Q�����m����[cm] */
#define TAIL_ANGLE_STAND_UP  93  /* ���S��~���̊p�x[�x] */
#define TAIL_ANGLE_DRIVE      3  /* �o�����X���s���̊p�x[�x] */
#define TAIL_ANGLE_INIT       0  /* 0�x */
#define P_GAIN             2.5F  /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60  /* ���S��~�p���[�^����PWM��΍ő�l */

#define DEVICE_NAME     "RisoRanger2016-mk2"  /* Bluetooth�� hrp2/target/ev3.h BLUETOOTH_LOCAL_NAME�Őݒ� */
#define PASS_KEY        "1111" /* �p�X�L�[    hrp2/target/ev3.h BLUETOOTH_PIN_CODE�Őݒ� */
#define CMD_START         '1'    /* �����[�g�X�^�[�g�R�}���h */

#define KP					0.74/*0.74*/ //���C���g���[�X���� ���W��
#define KI					0.01/*0.01*/ //���C���g���[�X���� �ϕ��W��
#define KD					0.03/*0.20*/ //���C���g���[�X���� �����W��
#define TARGET				35	 //���C���g���[�X���� ���ʃ^�[�Q�b�g�l
#define DELTA_T				0.004 //���������is�j
#define INT_NUM				250	//�ϕ�����΍���(1s��)

#define MESSAGE_LEN			20

/* LCD�t�H���g�T�C�Y */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

/* �֐��v���g�^�C�v�錾 */
static int32_t sonar_alert(void);
static void tail_control(int32_t angle);
//���b�Z�[�W�������֐�
static void Message(const char* str);


/* �I�u�W�F�N�g�ւ̃|�C���^��` */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;

/*�O���[�o���ϐ��FMassage�֐��Ŏg�p*/
int count;
//static char message[MESSAGE_LEN + 1] = {0};
int colorSensor_data = 0;//�J���[�Z���T�̒l�����������


/* ���C���^�X�N */
void main_task(intptr_t unused)
{
	//�O���[�o���ϐ��̏�����
	count = 1;
	
    int8_t forward;      /* �O��i���� */
	float turn;         /* ���񖽗� */
	int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	//int8_t white;		/* ���F�̌��Z���T�l */
	//int8_t black;		/* ���F�̌��Z���T�l */
	int8_t cur_brightness;	/* ���o�������Z���T�l */
	
	
	float error=0, lasterror=0, integral=0;
	float errorList[INT_NUM];	// �΍�����
	int i = 0, j = 0;
	for (i = 0; i < INT_NUM; i++) {
		errorList[i] = 0;
	}
	
    /* �e�I�u�W�F�N�g�𐶐��E���������� */
    touchSensor = new TouchSensor(PORT_1);
    colorSensor = new ColorSensor(PORT_3);
    sonarSensor = new SonarSensor(PORT_2);
    gyroSensor  = new GyroSensor(PORT_4);
    leftMotor   = new Motor(PORT_C);
    rightMotor  = new Motor(PORT_B);
    tailMotor   = new Motor(PORT_A);
    clock       = new Clock();


    /* �K�����[�^�[�̃��Z�b�g */
    tailMotor->reset();
	tail_control(TAIL_ANGLE_INIT); /* 0�x�ɐ��� */

    /* Open Bluetooth file */
	/* �ڑ���Ԃ��m�F�A�ڑ������Ɣ�����*/
	Message("waitting bluetooth connect");
	while(!ev3_bluetooth_is_connected()){
		tslp_tsk(100);
	}
	/* �V���A���|�[�g���J�� */
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);
	Message("bluetooth serial port open");

    /* Bluetooth�ʐM�^�X�N�̋N�� */
    act_tsk(BT_TASK);
	Message("Bluetooth task Start");

    ev3_led_set_color(LED_ORANGE); /* �����������ʒm */
	Message("Init finished.");

   /* �J���[�Z���T�[�L�����u���[�V�����̕��� */
   ////��Œǉ�////////////////////////////////////////
	/* �X�^�[�g�ҋ@ */
	/* �X�^�[�g����܂ł̓��� */
	
    while(1)
    {
        //tail_control(TAIL_ANGLE_STAND_UP); // ���S��~�p�p�x�ɐ���

        if (bt_cmd == 1)
        {
            break; // bluetooth�X�^�[�g
        }

        if (touchSensor->isPressed())
        {
            break; //�^�b�`�Z���T�������ꂽ
        }

        clock->sleep(10);
    }

    /* ���s���[�^�[�G���R�[�_�[���Z�b�g */
    leftMotor->reset();
    rightMotor->reset();
    
    /* �W���C���Z���T�[���Z�b�g */
    gyroSensor->reset();
    balance_init(); /* �|���U�qAPI������ */

	//count = 0; /* �A������񐔏�����*/
	//history = 0; //���߂̐��񏉊���
	
    ev3_led_set_color(LED_GREEN); /* �X�^�[�g�ʒm */
	Message("main loop start");

    /**
    * Main loop for the self-balance control algorithm
    */
    while(1)
    {
    	int32_t motor_ang_l, motor_ang_r;
        int32_t gyro, volt;

        if (ev3_button_is_pressed(BACK_BUTTON)){
    		//backbutton���������ƏI��
    		Message("finished...");
    		break;
    	}
    	
    	/*teraterm�ŕ\�����邽�߁A�J���[�Z���T����l���Ƃ��Ă���*/
    	colorSensor_data = colorSensor->getBrightness();

        tail_control(TAIL_ANGLE_DRIVE); /* �o�����X���s�p�p�x�ɐ��� */

        if (sonar_alert() == 1) /* ��Q�����m */
        {
			//fprintf(bt, "Detect things\n");
			forward = turn = 0; /* ��Q�������m�������~ */
		}
        else
        {
            forward = 45; /* �O�i���� */
			cur_brightness = colorSensor->getBrightness();
			//monochrome = (cur_brightness - white) / (black - white) * 100;
			//color = colorSensor->getColorNumber();

			// P����
			//error = cur_brightness - (white + black) / 2;
			//error = TARGET - cur_brightness;	// �����̉E�����g���[�X
			error = cur_brightness - TARGET;	// �����̍������g���[�X

			// I����
			errorList[j] = error;
			integral = 0;
			for (i = 0; i < INT_NUM; i++) {
				integral += errorList[i] * DELTA_T;
			}
			j + 1 < INT_NUM ? j++ : j = 0;
        	//integral += (lasterror + error) / 2 * DELTA_T;

			// D����
			turn = KP * error + KI * integral + KD * (error - lasterror) / DELTA_T;
			lasterror = error;
			//turn = KP * error;
			if (turn > 100) {
				turn = 100;
			}
			else if (turn < -100) {
				turn = -100;
			}

			fprintf(bt, "cur_brightness = %d, turn = %f\n", cur_brightness, turn);
			//fprintf(bt, "color_id = %d\n", color);
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
		//fprintf(bt, "left = %d, right = %d\n", pwm_L, pwm_R);

        clock->sleep(4); /* 4msec�����N�� */
    }
    leftMotor->reset();
    rightMotor->reset();

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
// �T�v : Bluetooth�ʐM�ɂ�郊���[�g�X�^�[�g�B Tera Term�Ȃǂ̃^�[�~�i���\�t�g����A
//       ASCII�R�[�h��1�𑗐M����ƁA�����[�g�X�^�[�g����B
//*****************************************************************************
void bt_task(intptr_t unused)
{
    while(1)
    {
        /*uint8_t c = fgetc(bt); // ��M
        switch(c)
        {
        case '1':
            bt_cmd = 1;
            break;
        default:
            break;
        }
        fputc(c, bt); //�G�R�[�o�b�N
    	*/
    	
    	
    	//colorSensor_data�𑗐M������
		/*�ʐM����*/
		
		char str[4];
		char* color = "c";
		char* nn = "|";
		
		sprintf(str, "%d", colorSensor_data);

		fwrite(color,1,3,bt);
		fwrite(str,1,5,bt);
		fwrite(nn,1,3,bt);
		
		clock->sleep(500);

    }
}

