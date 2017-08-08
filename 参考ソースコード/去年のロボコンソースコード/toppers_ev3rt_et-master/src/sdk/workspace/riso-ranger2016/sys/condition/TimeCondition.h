/*
 * TimeCondition.h
 *
 *  Created on: 2016/08/05
 *      Author: ryuji
 */

#ifndef WORKSPACE_RISO_RANGER2016_SYS_CONDITION_TIMECONDITION_H_
#define WORKSPACE_RISO_RANGER2016_SYS_CONDITION_TIMECONDITION_H_

#include "Condition.h"

class TimeCondition : public Condition
{

public:
	TimeCondition(unsigned int milliSecondLength);
	virtual ~TimeCondition();

	virtual void onStart();
	virtual bool check();

	static unsigned int s_AbsoluteTime;		// TODO Timerクラスのものを使う

private:
	unsigned long m_StartMilliSecond;	// 開始時刻[ms]
	unsigned long m_MilliSecondLength;	// 設定時間長[ms]

};
#endif /* WORKSPACE_RISO_RANGER2016_SYS_CONDITION_TIMECONDITION_H_ */
