///////////////////////////////////////////////////////////
//  Task.h
//  Implementation of the Class Task
//  Created on:      2016/04/28 11:49:10
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_C47B409B_FB08_431b_B2E8_7EFDD1752A11__INCLUDED_)
#define EA_C47B409B_FB08_431b_B2E8_7EFDD1752A11__INCLUDED_

#include "std.h"
#include "SubsystemId.h"

class SubsystemBridge;

class Task
{

public:
	Task(sint32 rv_tsk_id, sint32 rv_mbox_id);
	virtual ~Task();

	virtual void start() = 0;

protected:
    virtual void loop() = 0;

private:
	sint32 task_id;
	sint32 mbox_id;
    SubsystemBridge* the_subsystem_bridge[kMaxSubsystemId];

	virtual bool isOwnMessage(EmSubsystemId rv_subsys_id);



};
#endif // !defined(EA_C47B409B_FB08_431b_B2E8_7EFDD1752A11__INCLUDED_)
