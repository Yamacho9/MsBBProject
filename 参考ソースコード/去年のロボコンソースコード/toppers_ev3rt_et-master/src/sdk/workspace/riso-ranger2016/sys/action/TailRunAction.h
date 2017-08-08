/*
 * TailRunAction.h
 *
 *  Created on: 2016/08/11
 *      Author: ryuji
 */

#ifndef WORKSPACE_RISO_RANGER2016_SYS_ACTION_TAILRUNACTION_H_
#define WORKSPACE_RISO_RANGER2016_SYS_ACTION_TAILRUNACTION_H_

#include "Action.h"

class TailRunAction: public Action {
public:
	TailRunAction(int speed, int turn);
	virtual ~TailRunAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

private:
	int m_Speed;						// 前進速度 -100~100[%]
	int m_Turn;							// 旋回速度 -100~100[%]

};

#endif /* WORKSPACE_RISO_RANGER2016_SYS_ACTION_TAILRUNACTION_H_ */
