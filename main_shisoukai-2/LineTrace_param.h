/**
 * @file LineTrace_param.h
 * @brief ライントレース（PID制御）のパラメータ読み込みのヘッダファイル
 * @author kanako baba ww
 * @date 2017/07/23
 */
#include "ev3api.h"
#include "app.h"

/* パラメータのデフォルト値（失敗時に使用） */
#define D_FORWARD 50
#define D_KP 0.74
#define D_KI 0.01
#define D_KD 0.03

extern void LineTrace_param(int status, int8_t* forward, float* kp, float* ki,float* kd);
