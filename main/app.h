/*
 *      サンプルプログラム(1)のヘッダファイル
 */
 
#ifndef __api_h__
#define __api_h__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ターゲット依存の定義
 */
#include "target_test.h"

/*
 *  各タスクの優先度の定義
 */

#define MAIN_PRIORITY   5           /* メインタスクの優先度 */
                                    /* HIGH_PRIORITYより高くすること */

#define HIGH_PRIORITY   9           /* 並行実行されるタスクの優先度 */
#define MID_PRIORITY    10
#define LOW_PRIORITY    11

/*
 *  ターゲットに依存する可能性のある定数の定義
 */

#ifndef TASK_PORTID
#define TASK_PORTID     1           /* 文字入力するシリアルポートID */
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define STACK_SIZE      4096        /* タスクのスタックサイズ */
#endif /* STACK_SIZE */
	
/* 下記のマクロは個体/環境に合わせて変更する必要があります */
#define SONAR_ALERT_DISTANCE 30  //超音波センサによる障害物検知距離[cm]
#define P_GAIN             2.5F  //完全停止用モータ制御比例係数
#define CMD_START         '1'    //リモートスタートコマンド
#define TARGET				35	 //ライントレース制御 光量ターゲット値
#define DELTA_T				0.004 //処理周期（s）
#define INT_NUM				250	//積分する偏差数(1s分)
#define GYRO_OFFSET_PID 3  //PID制御時のジャイロセンサのオフセット値
#define FALL_DOWN	400	//転倒検知の閾値
	
/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)
#define MESSAGE_LEN 8
	
/* パラメータファイル情報 */
#define BUF_COLUMN_SIZE 5
#define BUF_LINE_SIZE 10

extern int buf[BUF_LINE_SIZE][BUF_COLUMN_SIZE];
extern int param[BUF_COLUMN_SIZE];
	
/* map data */
extern int *arr0, *arr1;
extern int linenum; // データの行数を数える

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void bt_task(intptr_t exinf);

extern int getBufLineSize();
extern int getBufColumnSize();
extern int getlinenum();
extern void setlinenum(int num);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

/*__api_h__*/
#endif

