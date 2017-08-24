#ifndef __lookup_h__
#define __lookup_h__

#include "ev3api.h"
#include "balancer.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "app.h"
#include "Calibration.h"
#include "LineTrace.h"


using namespace ev3api;


//#define TAIL_ANGLE_STAND_UP  80  /* ���S��~���̊p�x[�x] */
//#define TAIL_ANGLE_START	90  /* ���S��~���̊p�x[�x] */
//#define TAIL_ANGLE_DRIVE      3  /* �o�����X���s���̊p�x[�x] */
//#define TAIL_ANGLE_INIT       0  /* 0�x */
//#define PGAIN             2.5F  /* ���S��~�p���[�^������W�� */
//#define PWM_ABS_MAX          60  /* ���S��~�p���[�^����PWM��΍ő�l */
//#define PWM_ABS_MAX_FAST	60  /* ���S��~�p���[�^����PWM��΍ő�l */
//#define PWM_ABS_MAX_SLOW	30  /* ���S��~�p���[�^����PWM��΍ő�l */
//#define GYRO_OFFSET_CALIBRATION   0  /*�L�����u���[�V�������̃W���C���I�t�Z�b�g�l*/
//enum speed {eFast, eSlow};

#define TAIL_ANGLE_LOOKUPGATE 48
#define TAIL_ANGLE_MIDLE2 55
#define TAIL_ANGLE_MIDLE 68
#define PWM_ABS_MAX_SLOW_LOOK	3

extern bool lookup(GyroSensor* gyro, ColorSensor* color, Motor* leftmotor,Motor* rightmotor,Motor* tail,Clock* clock);

bool tail_ctr(int32_t angle, tailSpeed sp);

enum mode_lookup{
	TAIL_STANDUP,
	BACK,
};

/*lookup_h*/
#endif