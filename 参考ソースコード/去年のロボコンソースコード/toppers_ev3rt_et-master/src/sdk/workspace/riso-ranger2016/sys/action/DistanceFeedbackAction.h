///////////////////////////////////////////////////////////
//  DistanceFeedbackAction.h
//  Implementation of the Class DistanceFeedbackAction
//  Created on:      2016/07/27 11:47:53
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_40F80250_C89E_4088_959D_43DF38B0AF05__INCLUDED_)
#define EA_40F80250_C89E_4088_959D_43DF38B0AF05__INCLUDED_

#include "Action.h"
#include "PIDControl.h"

class DistanceFeedbackAction : public Action
{

public:
	DistanceFeedbackAction(int referenceDistance);
	virtual ~DistanceFeedbackAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

private:
	int m_ReferenceDistance;			// 目標距離 [mm]
	PIDControl *m_PIDContol;

};
#endif // !defined(EA_40F80250_C89E_4088_959D_43DF38B0AF05__INCLUDED_)
