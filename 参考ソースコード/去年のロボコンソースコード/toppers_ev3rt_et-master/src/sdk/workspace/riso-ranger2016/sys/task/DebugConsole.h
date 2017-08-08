///////////////////////////////////////////////////////////
//  DebugConsole.h
//  Implementation of the Class DebugConsole
//  Created on:      2016/05/24 13:09:41
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_AA1C2012_4042_4870_83CC_31C308E51B70__INCLUDED_)
#define EA_AA1C2012_4042_4870_83CC_31C308E51B70__INCLUDED_

#include "Task.h"

class DebugConsole : public Task
{

public:
	DebugConsole(sint32 rv_tsk_id, sint32 rv_mbox_id);
	virtual ~DebugConsole();

	virtual void start();

private:
    virtual void loop();
	virtual bool isOwnMessage();

};
#endif // !defined(EA_AA1C2012_4042_4870_83CC_31C308E51B70__INCLUDED_)
