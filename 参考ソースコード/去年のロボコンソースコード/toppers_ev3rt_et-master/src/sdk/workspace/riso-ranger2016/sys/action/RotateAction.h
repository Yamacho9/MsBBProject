///////////////////////////////////////////////////////////
//  RotateAction.h
//  Implementation of the Class RotateAction
//  Created on:      2016/07/27 11:47:53
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_40F80250_C89E_4088_959R_43DF38B0AF05__INCLUDED_)
#define EA_40F80250_C89E_4088_959R_43DF38B0AF05__INCLUDED_

#include "Action.h"
#include "PIDControl.h"

class RotateAction : public Action
{

public:
	RotateAction(int turn);
	virtual ~RotateAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

private:
	int m_Turn;			// 回転速度

};
#endif // !defined(EA_40F80250_C89E_4088_959R_43DF38B0AF05__INCLUDED_)
