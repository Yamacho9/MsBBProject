/*
 * TouchSensorCondition.h
 *
 *  Created on: 2016/08/28
 *      Author: Reina
 */

#ifndef WORKSPACE_RISO_RANGER2016_SYS_CONDITION_TOUCHSENSORCONDITION_H_
#define WORKSPACE_RISO_RANGER2016_SYS_CONDITION_TOUCHSENSORCONDITION_H_

#include "Condition.h"

class TouchSensorCondition : public Condition
{

public:
	TouchSensorCondition();
	virtual ~TouchSensorCondition();

	virtual void onStart();
	virtual bool check();

private:

};
#endif /* WORKSPACE_RISO_RANGER2016_SYS_CONDITION_TOUCHSNENSORCONDITION_H_ */
 