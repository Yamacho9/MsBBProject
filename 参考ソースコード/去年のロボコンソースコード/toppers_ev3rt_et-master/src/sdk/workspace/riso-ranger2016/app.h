///////////////////////////////////////////////////////////
//  app.h
//  Implementation of the Class system_task
//  Created on:      2016/04/28 11:21:26
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_B0DE3A01_003D_4139_ABC8_D1E1FF53B040__INCLUDED_)
#define EA_B0DE3A01_003D_4139_ABC8_D1E1FF53B040__INCLUDED_

#include "target_test.h"        //ターゲット依存の定義.


// -----------------------------------------------------------------------------
//! @name タスク優先度定義
//! タスクの優先度値の定義. OS のコンフィグ設定で使用する.
// -----------------------------------------------------------------------------
// @{
#define MAXIMUM_PRIORITY_VAL     1     //! 最高優先度
#define HIGH_PRIORITY_VAL        5     //!< 高優先度
#define BASE_PRIORITY_VAL       10     //!< 基準優先度.
#define LOW_PRIORITY_VAL        15     //!< 低優先度.
#define MINIMUM_PRIORITY_VAL    16     //!< 最低優先度.
// @}

#ifndef STACK_SIZE
//! @brief スタックサイズ定義
//! タスクのスタックサイズを定義する.
#define STACK_SIZE      4096
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

// -----------------------------------------------
//! @enum  EmTaskId
//! @brief タスク ID 定義.
//!
//! タスク ID を定義する.
//! 主に関連の識別子、多重度の為に定義しており、
//! OS の設定値とは一致しない内部定義である.
//! うまくOSの設定と一致させる方法があればそちらを採用したい.
// -----------------------------------------------
typedef enum _EmTaskId {
    kTaskMain,                  //!< メインタスク.
    kTaskHighPriority,          //!< 高優先度処理タスク.
    kTaskEngine,                //!< エンジン(通常処理)タスク.
    kTaskDebugConsole,          //!< デバッグコンソールタスク.

    kMaxTaskId                  //!< 終端定義(システムタスク数).
} EmTaskId;

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//! @brief main タスクエントリ関数.
//!
//! 電源 ON 時カーネルが自動的に起動をかけるタスク.
//! 主な責務は以下.
//!   - 他のタスクを起動する
//!   - ハートビート、ウォッチドッグなど、軽い非機能定常処理.
//! @exception none
// -----------------------------------------------------------------------------
extern void TaskMain(intptr_t unused);


// -----------------------------------------------------------------------------
//! @brief 高優先度処理タスクエントリ関数.
//!
//! 通常の処理より優先して処理したい場合、こちらのタスクを使う.
//! 例えば下記のような遅延があるとリスクになるようなもの.
//!   - ユーザーI/F からの要求受信処理
//!   - 即時性を要するフィードバック制御.
//! @exception none
// -----------------------------------------------------------------------------
extern void TaskHighPriority(intptr_t unused);


// -----------------------------------------------------------------------------
//! @brief エンジン(通常処理)タスクエントリ関数.
//!
//! 通常、特に理由が無い限り、このタスクで処理を実施する.
//! @exception none
// -----------------------------------------------------------------------------
extern void TaskEngine(intptr_t unused);


// -----------------------------------------------------------------------------
//! @brief デバッグコンソールタスクエントリ関数.
//!
//! デバッグログ取得用のタスク.
//! 非機能なので優先度は最低である.
//! @exception none
// -----------------------------------------------------------------------------
extern void TaskDebugConsole(intptr_t unused);

#if 0                       // 未実装.
extern void TaskCommunication();

extern void TaskLongTermOperation();

extern void TaskShortCycleOperation();


#endif

extern void Cyc1msecInterval(intptr_t exinf);

#ifdef __cplusplus
} // extern "C" {
#endif

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
// 無駄コード.
//   ea が吐き出してしまうからして…
class system_task
{

public:
	system_task();
	virtual ~system_task();

};
#endif // #ifdef __cplusplus
#endif // !defined(EA_B0DE3A01_003D_4139_ABC8_D1E1FF53B040__INCLUDED_)
