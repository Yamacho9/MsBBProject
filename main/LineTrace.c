/**
 * @file LineTrace.c
 * @brief ライントレース（PID制御）
 * @author Keishi MURAI
 * @date 2017/06/19
 */
#include "LineTrace.h"

/**
 * @brief ライントレース制御
 * @param [in] int status ラインの状態(直線/カーブ)
 * @param [in] int targetVal 目標の光センサ値
 * @param [in] int currentVal 現在の光センサ値
 * @param [in] float opePeriod 処理周期[s]
 * @param [in,out] int* lastErr 前回偏差
 * @param [in, out] int forward
 * @return 旋回角度[deg.] 100(右旋回最大値)～-100(左旋回最大値)
 * @detail 光センサ値を基にPID制御を行う
 */
float LineTrace(int status, int targetVal, int currentVal, float opePeriod, int* lastErr, int8_t* forward) {
	int integral=0;	//偏差積分
	float turn;	//旋回角度
	
	LineTrace_param(status,forward,&kp,&ki,&kd);

	// P制御
	errParam = currentVal - targetVal;	// 黒線の左側をトレース

	// D制御
	diffParam = (errParam - *lastErr) / opePeriod;
	*lastErr = errParam;

	// PD制御に基づいた旋回値を算出
	// I制御の係数、及び、変数は不要だが、コースマップに合わせて含んだ形にする。
	turn = kp * (float)errParam + ki * (float)integral + kd * diffParam;

	// 旋回角度が範囲内に収まっているか確認
	if (turn > MAX_TURN_RIGHT) {
		turn = MAX_TURN_RIGHT;
	}
	else if (turn < MAX_TURN_LEFT) {
		turn = MAX_TURN_LEFT;
	}
	return turn;
}

// PID制御係数取得（デバッグ用）
void GetPID(float* kkp,float* kki, float* kkd){
	*kkp = kp;
	*kki = ki;
	*kkd = kd;
}

// PID制御変数取得（デバッグ用）
void GetVar(int* err, float* diff){
	*err = errParam;
	*diff = diffParam;
}
