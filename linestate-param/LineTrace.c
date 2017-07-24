/**
 * @file LineTrace.c
 * @brief ライントレース（PID制御）
 * @author Keishi MURAI
 * @date 2017/06/19
 */
#include "LineTrace.h"

/**
 * @brief ライントレース制御
 * @param [in] enum Line_state state ラインの状態(直線/カーブ)
 * @param [in] int targetVal 目標の光センサ値
 * @param [in] int currentVal 現在の光センサ値
 * @param [in] float opePeriod 処理周期[s]
 * @param [in,out] int errList[] 偏差履歴
 * @param [in,out] int nextErrIndex 次の偏差履歴インデックス
 * @param [in, out] int forward
 * @return 旋回角度[deg.] 100(右旋回最大値)～-100(左旋回最大値)
 * @detail 光センサ値を基にPID制御を行う
 */
float LineTrace(enum Line_status status, int targetVal, int currentVal, float opePeriod, int errList[], int nextErrIndex, int8_t* forward) {
	int err;	//偏差
	int i;	//偏差履歴用インデックス
	int errNum;	//偏差履歴数
	int lastErrIndex;	//前回偏差のインデックス
	int integral;	//偏差積分
	int diff;	//偏差微分
	float turn;	//旋回角度
	float kp,ki,kd;
	
	LineTrace_param(status,forward,&kp,&ki,&kd);

	// P制御
	err = currentVal - targetVal;	// 黒線の左側をトレース

	// I制御
	errList[nextErrIndex] = err;
	errNum = sizeof errList / sizeof errList[0];
	integral = 0;
	for (i = 0; i < errNum; i++) {
		integral += errList[i] * opePeriod;
	}

	// D制御
	if (nextErrIndex != 0) {
		lastErrIndex = nextErrIndex - 1;
	} else {
		lastErrIndex = errNum - 1;
	}
	diff = (err - errList[lastErrIndex]) / opePeriod;

	turn = kp * (float)err + ki * (float)integral + kd * (float)diff;

	if (nextErrIndex + 1 < errNum) {
		nextErrIndex++;
	} else {
		nextErrIndex = 0;
	}
	
	// 旋回角度が範囲内に収まっているか確認
	if (turn > MAX_TURN_RIGHT) {
		turn = MAX_TURN_RIGHT;
	}
	else if (turn < MAX_TURN_LEFT) {
		turn = MAX_TURN_LEFT;
	}
	return turn;
}
