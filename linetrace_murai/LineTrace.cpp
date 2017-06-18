/**
 * @brief ライントレース制御
 * @param [in] targetVal 目標の光センサ値
 * @param [in] currentVal 現在の光センサ値
 * @param [in] opePeriod 処理周期[s]
 * @param [in,out] errList 偏差履歴
 * @param [in,out] nextErrIndex 次の偏差履歴インデックス
 * @return 旋回角度[deg.] 100(右旋回最大値)～-100(左旋回最大値)
 * @detail 光センサ値を基にPID制御を行う
 */

#include "LineTrace.h"

#define KP					0.74 //ライントレース制御 比例係数
#define KI					0.01 //ライントレース制御 積分係数
#define KD					0.03 //ライントレース制御 微分係数

#define MAX_TURN_RIGHT	100
#define MAX_TURN_LEFT	-100

float LineTrace(int targetVal, int currentVal, float opePeriod, int errList[], int nextErrIndex) {
	int err;	//偏差
	int i;	//偏差履歴用インデックス
	int errNum;	//偏差履歴数
	int lastErrIndex;	//前回偏差のインデックス
	int integral;	//偏差積分
	int diff;	//偏差微分
	float turn;	//旋回角度

	// P制御
	err = currentVal - targetVal;	// 黒線の左側をトレース

	// I制御
	errList[nextErrIndex] = err;
	errNum = sizeof errList / sizeof errList[0];
	integral = 0;
	for (i = 0; i < errNum; i++) {
		integral += errorList[i] * opePeriod;
	}

	// D制御
	nextErrIndex == 0 ? lastErrIndex = errNum - 1 : lastErrIndex = nextErrIndex - 1;
	diff = (err - errList[lastErrIndex]) / opePeriod;

	turn = KP * (float)error + KI * (float)integral + KD * (float)diff;

	nextErrIndex + 1 < errNum ? nextErrIndex++ : nextErrIndex = 0;

	if (turn > MAX_TURN_RIGHT) {
		turn = MAX_TURN_RIGHT;
	}
	else if (turn < MAX_TURN_LEFT) {
		turn = MAX_TURN_LEFT;
	}

}