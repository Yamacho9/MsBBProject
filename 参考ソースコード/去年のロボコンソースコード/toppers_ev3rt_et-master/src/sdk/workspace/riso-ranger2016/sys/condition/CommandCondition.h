///////////////////////////////////////////////////////////
//  CommandCondition.h
//  Implementation of the Class CommandCondition
//  Created on:      2016/08/03 12:55:29
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_897C60C0_24FE_4387_B056_9AAEBEC3E6FB__INCLUDED_)
#define EA_897C60C0_24FE_4387_B056_9AAEBEC3E6FB__INCLUDED_

#include "Condition.h"

class CommandCondition : public Condition
{

public:
	CommandCondition();
	virtual ~CommandCondition();

	virtual void onStart();
	virtual bool check();

};
#endif // !defined(EA_897C60C0_24FE_4387_B056_9AAEBEC3E6FB__INCLUDED_)
