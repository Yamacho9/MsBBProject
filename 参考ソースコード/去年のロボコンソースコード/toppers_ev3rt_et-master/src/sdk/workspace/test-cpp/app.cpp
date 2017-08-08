/**
 * This sample program balances a two-wheeled Segway type robot such as Gyroboy in EV3 core set.
 *
 * References:
 * http://www.hitechnic.com/blog/gyro-sensor/htway/
 * http://www.cs.bgu.ac.il/~ami/teaching/Lejos-2013/classes/src/lejos/robotics/navigation/Segoway.java
 */

#include "ev3api.h"
#include "app.h"

//#include "libcpp-test.h"

#include "Motor.h"
#include "WheelControl.h"
#include "TailControl.h"
#include "ColorSensor.h"
#include "StateObserver.h"
#include "Sequencer.h"
#include "Scenario.h"
#include "LeftCourseScenario.h"
#include "RightCourseScenario.h"
#include "Sequence.h"
#include "SitWaitAction.h"
#include "EmptyCondition.h"
#include "Battery.h"
#include "GyroSensor.h"
#include "Action.h"
#include "Condition.h"
#include "TimeCondition.h"
#include "UltrasonicControl.h"
#include "balancer_private.h"
#include "Bluetooth.h"
#include "PIDControl.h"
#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif
/*
class TestClass {
public:
    TestClass() {
        //static char buf[256];
        //sprintf(buf, "Object has been created.");
        //ev3_lcd_draw_string(buf, 0, 16);
        member = 0x12345678;
    }

    void testMethod() {
        static char buf[256];
        sprintf(buf, "Member is 0x%08x.", member);
        ev3_lcd_draw_string(buf, 0, 32);
    }
private:
    int member;
};

void idle_task(intptr_t unused) {
    while(1) {
    	fprintf(bt, "Press 'h' for usage instructions.\n");
    	tslp_tsk(1000);
    }
}
*/

// グローバル変数宣言
static int heartBeatCount = 0;

// インスタンス生成、関連構築、初期化
static Sequencer* sequencer = new Sequencer(new Sequence(new SitWaitAction(90), new EmptyCondition()));

static Scenario* scenario = new LeftCourseScenario();
//static Scenario* scenario = new RightCourseScenario();

static Motor* leftMotor = new Motor(EV3_PORT_C);
static Motor* rightMotor = new Motor(EV3_PORT_B);
static Motor* tailMotor = new Motor(EV3_PORT_A);
static Battery* battery = new Battery();
static GyroSensor* gyroSensor = new GyroSensor(EV3_PORT_4);
static ColorSensor* colorSensor = new ColorSensor(EV3_PORT_3);
static UltrasonicControl* ultrasonicControl = new UltrasonicControl(EV3_PORT_2);


static StateObserver* stateObserver = new StateObserver(leftMotor, rightMotor, tailMotor, colorSensor);
static WheelControl* wheelControl = new WheelControl(leftMotor, rightMotor, battery, gyroSensor);

static PIDControl* pidControl = new PIDControl(1, 0, 0);
static TailControl* tailControl = new TailControl(tailMotor, pidControl);
void main_task(intptr_t unused) {

	
	scenario->init(sequencer);
	Action::init(stateObserver, tailControl, wheelControl);
	Condition::init(stateObserver, ultrasonicControl);

	wheelControl->Init();
	TimeCondition::s_AbsoluteTime = 0;	// TODO Timer置き換え.

	// シナリオ生成
	scenario->start();

	// 4ms周期タスク起動

    act_tsk(CONSOLE_TASK);

	ev3_sta_cyc(ID_EV3CYC_4MS);

    while(1) {
        tslp_tsk(100);
/*
		while (!ev3_bluetooth_is_connected()) tslp_tsk(100);
		char c = fgetc(btlog);
		switch(c) {
		case 'q':
			K_F[0] += 0.1;
			fprintf(btlog, "K_F[0]=%f\r\n", K_F[0]);
			break;
		case 'a':
			K_F[0] -= 0.1;
			fprintf(btlog, "K_F[0]=%f\r\n", K_F[0]);
			break;
		case 'w':
			K_F[1] += 0.1;
			fprintf(btlog, "K_F[1]=%f\r\n", K_F[1]);
			break;		case 's':
			K_F[1] -= 0.1;
			fprintf(btlog, "K_F[1]=%f\r\n", K_F[1]);
			break;
		case 'e':
			K_F[2] += 0.1;
			fprintf(btlog, "K_F[2]=%f\r\n", K_F[2]);
			break;
		case 'd':
			K_F[2] -= 0.1;
			fprintf(btlog, "K_F[2]=%f\r\n", K_F[2]);
			break;
		case 'r':
			K_F[3] += 0.1;
			fprintf(btlog, "K_F[3]=%f\r\n", K_F[3]);
			break;
		case 'f':
			K_F[3] -= 0.1;
			fprintf(btlog, "K_F[3]=%f\r\n", K_F[3]);
			break;
		case 't':
			K_I += 0.01;
			fprintf(btlog, "K_I=%f\r\n", K_I);
			break;
		case 'g':
			K_I -= 0.01;
			fprintf(btlog, "K_I=%f\r\n", K_I);
			break;
		case 'y':
			K_PHIDOT += 0.1;
			fprintf(btlog, "K_PHIDOT=%f\r\n", K_PHIDOT);
			break;
		case 'h':
			K_PHIDOT -= 0.1;
			fprintf(btlog, "K_PHIDOT=%f\r\n", K_PHIDOT);
			break;
		case 'u':
			K_THETADOT += 0.1;
			fprintf(btlog, "K_THETADOT=%f\r\n", K_THETADOT);
			break;
		case 'j':
			K_THETADOT -= 0.1;
			fprintf(btlog, "K_THETADOT=%f\r\n", K_THETADOT);
			break;
		default:
			fprintf(btlog, "Unknown key '%c' pressed.\r\n", c);
		}
*/
    }

}
void Cyc4msecInterval(intptr_t unused) {

	leftMotor->UpdateAngularVelocity();		// 中
	rightMotor->UpdateAngularVelocity();	// 中
	tailMotor->UpdateAngularVelocity();		// 中
	stateObserver->Calc();					// 中
	tailControl->Control();					// 高
	wheelControl->Control();				// 高
	sequencer->cycle();						// 低

	// ハートビート
	if(heartBeatCount%250 == 0) {
	ev3_led_set_color(LED_ORANGE);
		ev3_led_set_color(LED_ORANGE);
	}
	else if((heartBeatCount+125)%250 == 0) {
		ev3_led_set_color(LED_GREEN);
	}
	heartBeatCount++;

	TimeCondition::s_AbsoluteTime+=4;	// TODO Timer置き換え

}
