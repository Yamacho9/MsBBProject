///////////////////////////////////////////////////////////
//  EmptyCondition.h
//  Implementation of the Class EmptyCondition
//  Created on:      2016/08/04 11:06:39
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_B882AEEF_A3CD_4042_8854_E6020FAB9E81__INCLUDED_)
#define EA_B882AEEF_A3CD_4042_8854_E6020FAB9E81__INCLUDED_

#include "Condition.h"

/**
 * 必ず偽となる特殊な条件クラス。
 */
class EmptyCondition : public Condition
{

public:
	EmptyCondition();
	virtual ~EmptyCondition();

	virtual void onStart();
	virtual bool check();

};
#endif // !defined(EA_B882AEEF_A3CD_4042_8854_E6020FAB9E81__INCLUDED_)
