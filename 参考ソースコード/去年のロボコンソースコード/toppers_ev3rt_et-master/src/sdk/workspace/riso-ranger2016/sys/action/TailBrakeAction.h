///////////////////////////////////////////////////////////
//  TailBrakeAction.h
//  Implementation of the Class TailBrakeAction
//  Created on:      2016/08/09 20:55:39
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_633BD4DA_FF02_49a5_99AD_3A5A997830D0__INCLUDED_)
#define EA_633BD4DA_FF02_49a5_99AD_3A5A997830D0__INCLUDED_

#include "Action.h"

class TailBrakeAction : public Action
{

public:
	TailBrakeAction();
	virtual ~TailBrakeAction();

	virtual void onStart();
	virtual void onCycle();
	virtual void onStop();

};
#endif // !defined(EA_633BD4DA_FF02_49a5_99AD_3A5A997830D0__INCLUDED_)
