/**
* @file CalcDistanceAndDirection.c
* @brief 走行距離と向きの算出
* @author Keishi MURAI
* @date 2017/06/20
*/

#include "CalcDistanceAndDirection.h"

/**
* @brief 走行距離と向きの算出
* @param [in] int leftCnt 左車輪モーターのエンコーダー値
* @param [in] int rightCnt 右車輪モーターのエンコーダー値
* @param [out] int* distance 走行距離
* @param [out] int* direction 向き
* @return 無し
* @detail 左右モーターのエンコーダーの平均値から距離と向きを算出
*/
void CalcDistanceAndDirection (int leftCnt, int rightCnt, int* distance, int* direction) {
	// モーターの検出角度（累積値）
	int angle;
	int dir;
	angle = (leftCnt + rightCnt) / 2;

	*distance = angle * PI * DIAMETER / 360;		// 距離
	
	// 向き（スタート時の向きを0度として、時計回りの角度）
	dir = (leftCnt % (360 * 4) - rightCnt % (360 * 4)) / 4;
	if (dir < 0) {
		dir = dir + 360;
	}
	*direction = dir;
	
	return;
}
