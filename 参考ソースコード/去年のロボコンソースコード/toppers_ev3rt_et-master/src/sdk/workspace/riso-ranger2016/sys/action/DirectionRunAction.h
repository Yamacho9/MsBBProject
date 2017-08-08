///////////////////////////////////////////////////////////
//  DirectionRunAction.h
//  Implementation of the Class DirectionRunAction
//  Created on:      2016/07/27 11:47:53
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_40F80250_C89E_4088_959C_43DF38B0AF05__INCLUDED_)
#define EA_40F80250_C89E_4088_959C_43DF38B0AF05__INCLUDED_

#include "Action.h"
#include "PIDControl.h"

class DirectionRunAction : public Action
{

public:
	DirectionRunAction(int speed, int tailAngle, int direction);
	virtual ~DirectionRunAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

private:
	PIDControl* m_PidControl;
	int m_Speed;						// 走行速度 -100~100[%]
	int m_TailAngle;					// 尻尾角度 [deg]
	int m_ReferenceDirection;			// 目標方位角 [deg]

};
#endif // !defined(EA_40F80250_C89E_4088_959C_43DF38B0AF05__INCLUDED_)
