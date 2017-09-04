/**
 * @file LineTrace.h
 * @brief ライントレース（PID制御）
 * @author Keishi MURAI
 * @date 2017/06/19
 */

#ifndef _LINETRACE_H_INCLUDED
#define _LINETRACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

#define KP	0.74 //ライントレース制御 比例係数
#define KI	0.01 //ライントレース制御 積分係数
#define KD	0.03 //ライントレース制御 微分係数

#define MAX_TURN_RIGHT	100 //右旋回最大値
#define MAX_TURN_LEFT	-100 //左旋回最大値

#define FORWARD	100 //速度

extern float LineTrace(int status, int targetVal, int currentVal, float opePeriod, int* lastErr, int8_t* forward, int* errParam, float* diffParam);

#ifdef __cplusplus
}
#endif

#endif /* ! _LINETRACE_H_INCLUDED */
