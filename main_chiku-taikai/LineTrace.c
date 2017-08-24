/**
 * @file LineTrace.c
 * @brief ライントレース（PD制御）
 * @author Keishi MURAI
 * @date 2017/06/19
 */
#include "LineTrace.h"

#define DEBUG 0

/**
 * @brief ライントレース制御
 * @param [in] int state ラインの状態(直線/カーブ)
 * @param [in] int targetVal 目標の光センサ値
 * @param [in] int currentVal 現在の光センサ値
 * @param [in] float opePeriod 処理周期[s]
 * @param [in,out] int* lastErr 前回偏差
 * @param [in, out] int forward
 * @param [out] int* errParam 偏差（デバッグ出力用）
 * @param [out] int* diffParam 偏差微分（デバッグ出力用）
 * @return 旋回角度[deg.] 100(右旋回最大値)～-100(左旋回最大値)
 * @detail 光センサ値を基にPD制御を行う
 */
float LineTrace(int status, int targetVal, int currentVal, float opePeriod, int* lastErr, int8_t* forward, int* errParam, float* diffParam) {
	int err;	//偏差
	int integral=0;	//偏差積分
	float diff=0.0;	//偏差微分
	float turn;	//旋回角度
	float kp,ki,kd;
	
#if DEBUG
	LineTrace_param(status,forward,&kp,&ki,&kd);
#else
	*forward = 45;	// 試走会2では"100"をセット
#endif
	
	// P制御
	err = currentVal - targetVal;	// 黒線の左側をトレース

	// D制御
	diff = (err - *lastErr) / opePeriod;
	*lastErr = err;

#if DEBUG
	turn = kp * (float)err + ki * (float)integral + kd * diff;
#else
	turn = KP * (float)err + KI * (float)integral + KD * diff;
#endif
	// 旋回角度が範囲内に収まっているか確認
	if (turn > MAX_TURN_RIGHT) {
		turn = MAX_TURN_RIGHT;
	}
	else if (turn < MAX_TURN_LEFT) {
		turn = MAX_TURN_LEFT;
	}
	*errParam = err;
	*diffParam = diff;
	return turn;
}
