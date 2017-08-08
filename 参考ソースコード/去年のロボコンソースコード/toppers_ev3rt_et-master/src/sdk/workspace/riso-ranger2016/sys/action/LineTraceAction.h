///////////////////////////////////////////////////////////
//  LineTraceAction.h
//  Implementation of the Class LineTraceAction
//  Created on:      2016/07/27 11:47:53
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_40F80250_C89E_4088_959C_43DF38A0AF05__INCLUDED_)
#define EA_40F80250_C89E_4088_959C_43DF38A0AF05__INCLUDED_

#include "Action.h"
#include "PIDControl.h"

class LineTraceAction : public Action
{

public:
	LineTraceAction(int speed, int tailAngle, float referenceLinePosition, PIDControl* pidControl);	// 走行速度 -100~100[%],尻尾角度 0～[deg]
	virtual ~LineTraceAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

private:
	PIDControl* m_PidControl;
	int m_Speed;						// 走行速度 -100~100[%]
	float m_ReferenceLinePosition;		// トレース位置[正規化カラーセンサ値]
	int m_TailAngle;					// 尻尾角度 0～[deg]

};
#endif // !defined(EA_40F80250_C89E_4088_959C_43DF38A0AF05__INCLUDED_)
