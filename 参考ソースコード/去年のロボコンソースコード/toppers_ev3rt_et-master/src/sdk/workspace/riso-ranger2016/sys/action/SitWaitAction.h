///////////////////////////////////////////////////////////
//  SitWaitAction.h
//  Implementation of the Class SitWaitAction
//  Created on:      2016/08/01 16:33:15
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_843E98B2_DB1A_40fb_B0D5_0FEA5F22A921__INCLUDED_)
#define EA_843E98B2_DB1A_40fb_B0D5_0FEA5F22A921__INCLUDED_

#include "Action.h"

class SitWaitAction : public Action
{

public:
	SitWaitAction(int tailAngle);	// 尻尾角度[deg]
	virtual ~SitWaitAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

private:
	int m_TailAngle;	// 尻尾角度[deg]

};
#endif // !defined(EA_843E98B2_DB1A_40fb_B0D5_0FEA5F22A921__INCLUDED_)
