#ifndef __lookup_h__
#define __lookup_h__

#include "ev3api.h"
#include "app.h"
#include "balancer.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "Calibration.h"


using namespace ev3api;


//#define TAIL_ANGLE_STAND_UP  80  /* 完全停止時の角度[度] */
//#define TAIL_ANGLE_START	90  /* 完全停止時の角度[度] */
//#define TAIL_ANGLE_DRIVE      3  /* バランス走行時の角度[度] */
//#define TAIL_ANGLE_INIT       0  /* 0度 */
//#define PGAIN             2.5F  /* 完全停止用モータ制御比例係数 */
//#define PWM_ABS_MAX          60  /* 完全停止用モータ制御PWM絶対最大値 */
//#define PWM_ABS_MAX_FAST	60  /* 完全停止用モータ制御PWM絶対最大値 */
//#define PWM_ABS_MAX_SLOW	30  /* 完全停止用モータ制御PWM絶対最大値 */
//#define GYRO_OFFSET_CALIBRATION   0  /*キャリブレーション時のジャイロオフセット値*/
//enum speed {eFast, eSlow};

#define TAIL_ANGLE_LOOKUPGATE 15

extern bool lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail);

bool tail_ctr(int32_t angle, tailSpeed sp);

bool standuptail();

/*lookup_h*/
#endif