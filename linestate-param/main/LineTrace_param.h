/**
 * @file LineTrace_param.h
 * @brief ライントレース（PID制御）のパラメータ読み込みのヘッダファイル
 * @author kanako baba ww
 * @date 2017/07/23
 */
#include <stdio.h>
#include "ev3api.h"

/* パラメータのデフォルト値（失敗時に使用） */
#define D_FORWARD 50
#define D_KP 0.74
#define D_KI 0.01
#define D_KD 0.03

enum Line_status{
	LINESTATUS_DEFAULT,
	LINESTATUS_STRAIGHT,
	LINESTATUS_CURVE_WEAK,
	LINESTATUS_CURVE_STRONG,
};	

extern int LineTrace_param(enum Line_status status, int8_t* forward, float* kp, float* ki,float* kd);
