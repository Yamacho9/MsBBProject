/**
* @file CalcDistanceAndDirection.c
* @brief 走行距離と向きの算出
* @author Keishi MURAI
* @date 2017/06/20
*/

#include "CalcDistanceAndDirection.h"

/**
* @brief 走行距離と向きの算出
* @param [in] int32_t leftCnt 左車輪モーターのエンコーダー値
* @param [in] int32_t rightCnt 右車輪モーターのエンコーダー値
* @param [out] int32_t distance 走行距離
* @param [out] int32_t direction 向き
* @return 無し
* @detail 左右モーターのエンコーダーの平均値から距離と向きを算出
*/
void CalcDistanceAndDirection (int32_t leftCnt, int32_t rightCnt, int32_t distance, int32_t direction) {
	int32_t angle;
	angle = (leftCnt + rightCnt) / 2;	// モーターの検出角度（累積値）
	distance = angle * PI * DIAMETER / 360;		// 距離
	// 向き（スタート時の向きを0度として、時計回りの角度）
	direction = (motor_ang_l % (360 * 4) - motor_ang_r % (360 * 4)) / 4; 
}
