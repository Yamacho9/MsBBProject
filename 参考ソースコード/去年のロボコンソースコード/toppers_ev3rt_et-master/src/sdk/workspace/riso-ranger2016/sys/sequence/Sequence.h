///////////////////////////////////////////////////////////
//  Sequence.h
//  Implementation of the Class Sequence
//  Created on:      2016/08/03 12:27:13
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_8BF75F76_FF10_44f5_BC0A_DCCAAAE0560E__INCLUDED_)
#define EA_8BF75F76_FF10_44f5_BC0A_DCCAAAE0560E__INCLUDED_

#include "Action.h"
#include "Condition.h"

class Sequence
{

public:
	Sequence(Action* action, Condition* condition);
	virtual ~Sequence();

	Action* getAction();
	Condition* getCondition();
	Sequence* getNextSequence();
	void deleteAllFollowingSequences();
	Sequence* setNextSequence(Sequence* nextSequence);

private:
	Action* m_CurrentAction;
	Condition* m_CurrentCondition;
	Sequence* m_NextSequence;

};
#endif // !defined(EA_8BF75F76_FF10_44f5_BC0A_DCCAAAE0560E__INCLUDED_)
