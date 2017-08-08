///////////////////////////////////////////////////////////
//  EmergencyStopAction.h
//  Implementation of the Class EmergencyStopAction
//  Created on:      2016/07/27 14:50:56
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_E48F66AC_4754_4aff_8259_122DADEFEF57__INCLUDED_)
#define EA_E48F66AC_4754_4aff_8259_122DADEFEF57__INCLUDED_

#include "Action.h"

class EmergencyStopAction : public Action
{

public:
	EmergencyStopAction();
	virtual ~EmergencyStopAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

};
#endif // !defined(EA_E48F66AC_4754_4aff_8259_122DADEFEF57__INCLUDED_)
