///////////////////////////////////////////////////////////
//  TailStopCondition.h
//  Implementation of the Class TailStopCondition
//  Created on:      2016/09/06 21:01:25
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_C0DF3543_A2EE_4352_85BB_FE4858BC600A__INCLUDED_)
#define EA_C0DF3543_A2EE_4352_85BB_FE4858BC600A__INCLUDED_

#include "Condition.h"

/**
 * 尻尾停止検出
 */
class TailStopCondition : public Condition
{

public:
	TailStopCondition(int targetStableTime);
	virtual ~TailStopCondition();

	virtual void onStart();
	virtual bool check();

private:
	int m_stableTime;		// 停止時間 [msec]
	int m_targetStableTime;	// 目標停止時間 [msec]

};
#endif // !defined(EA_C0DF3543_A2EE_4352_85BB_FE4858BC600A__INCLUDED_)
