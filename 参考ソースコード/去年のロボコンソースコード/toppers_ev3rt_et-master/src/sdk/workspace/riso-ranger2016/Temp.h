///////////////////////////////////////////////////////////
//  Temp.h
//  Implementation of the Class Temp
//  Created on:      2016/07/21 19:58:14
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_329EB60B_1959_4205_80F0_3B93161E230D__INCLUDED_)
#define EA_329EB60B_1959_4205_80F0_3B93161E230D__INCLUDED_

#include "ev3api.h"
#include "Motor.h"
#include "WheelControl.h"
#include "TailControl.h"
//#include "Linetrace.h"
#include "ColorSensor.h"
#include "StateObserver.h"

class Temp
{

public:
	Temp();
	virtual ~Temp();

	static void init();		// 周期呼び出し開始前に一度だけ実行
	static void cycle();	// 周期呼び出し
	static int getBattery();
	static int getGyro();
	static int getGyroOffset();

	static const sensor_port_t colorSensorPort = EV3_PORT_3;
	static const sensor_port_t gyroPort = EV3_PORT_4; // 間違ってたら直す
	static const motor_port_t wheelLPort = EV3_PORT_C;
	static const motor_port_t wheelRPort = EV3_PORT_B;
	static const motor_port_t tailPort = EV3_PORT_A;

	static int gyroOffset;

	static Motor* wheelMotorL;
	static Motor* wheelMotorR;
	static Motor* tailMotor;

	static WheelControl* wheelControl;
	static TailControl* tailControl;

//	static Linetrace* linetrace;
	static Calibration* calibration;
	static ColorSensor* getSensorValue;

	static StateObserver* stateObserver;

private:




};
#endif // !defined(EA_329EB60B_1959_4205_80F0_3B93161E230D__INCLUDED_)
