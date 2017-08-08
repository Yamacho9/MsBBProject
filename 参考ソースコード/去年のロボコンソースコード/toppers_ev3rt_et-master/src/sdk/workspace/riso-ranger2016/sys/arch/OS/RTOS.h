///////////////////////////////////////////////////////////
//  RTOS.h
//  Implementation of the Class RTOS
//  Created on:      2016/04/13 12:52:15
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_A611960B_E06B_4372_91D0_B7AA38A53E49__INCLUDED_)
#define EA_A611960B_E06B_4372_91D0_B7AA38A53E49__INCLUDED_

#include "std.h"

class RTOS
{
private:
	static uint16 lockcpu_count;


public:
	RTOS();
	~RTOS();                    // 継承されるクラスではないので、仮想関数にはしない.
    // タスクの起動.
	static void startTask(uint16 rv_task_id, uint16 rm_prm = 0);
    // タスク間通信における送信.
	static void sndMessage(uint16 rv_mbx_id, void* rv_message);
    // タスク間通信における送信(非タスクコンテキスト用).
	static void intrSndMessage(uint16 rv_mbx_id, void* rv_message);
    // タスク間通信における受信.
	static void rcvMessage(void** rv_message, uint16 rv_mbx_id);
	static void waiSemaphore(uint16 rv_sem_id);
	static void sigSemaphore(uint16 rv_sem_id);
	static void intrSetFlag(uint16 rv_flg_id, uint32 rv_flg_pattern);
	static uint32 waiFlag(uint16 rv_flg_id);
	static void lockCpu();
	static void unlockCpu();
	static void waiTask(uint16 rv_timeout_cnt = 0);
    static void startCyclicHandler(uint16 rv_cyc_id);
	static void stopCyclicHandler(uint16 rv_cyc_id);

};
#endif // !defined(EA_A611960B_E06B_4372_91D0_B7AA38A53E49__INCLUDED_)
