/**
 * @file LineTrace.h
 * @brief ライントレース（PID制御）
 * @author Keishi MURAI
 * @date 2017/06/19
 */

#include "LineTrace_param.h"

#ifndef _LINETRACE_H_INCLUDED
#define _LINETRACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define KP	0.74 //ライントレース制御 比例係数
#define KI	0.01 //ライントレース制御 積分係数
#define KD	0.03 //ライントレース制御 微分係数

#define MAX_TURN_RIGHT	100 //右旋回最大値
#define MAX_TURN_LEFT	-100 //左旋回最大値

extern float LineTrace(enum Line_status status, int targetVal, int currentVal, float opePeriod, int errList[], int nextErrIndex, int8_t* forward);

#ifdef __cplusplus
}
#endif

#endif /* ! _LINETRACE_H_INCLUDED */
