///////////////////////////////////////////////////////////
//  TailCalibrationAction.h
//  Implementation of the Class TailCalibrationAction
//  Created on:      2016/09/12 16:01:03
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_55301C9E_E18F_4ab0_9945_4C311E833D82__INCLUDED_)
#define EA_55301C9E_E18F_4ab0_9945_4C311E833D82__INCLUDED_

#include "Action.h"

class TailCalibrationAction : public Action
{

public:
	TailCalibrationAction();
	virtual ~TailCalibrationAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

private:
	int m_TimeCount;	// 時間カウント用
	float m_RefTailAngle;	// 尻尾角度目標値

};
#endif // !defined(EA_55301C9E_E18F_4ab0_9945_4C311E833D82__INCLUDED_)
