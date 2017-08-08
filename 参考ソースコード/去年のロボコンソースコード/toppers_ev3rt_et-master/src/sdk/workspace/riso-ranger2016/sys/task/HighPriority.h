///////////////////////////////////////////////////////////
//  HighPriority.h
//  Implementation of the Class HighPriority
//  Created on:      2016/05/24 11:33:04
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_94B2C302_5605_4975_8286_36F9622C7EA3__INCLUDED_)
#define EA_94B2C302_5605_4975_8286_36F9622C7EA3__INCLUDED_

#include "std.h"
#include "Task.h"

class HighPriority : public Task
{

public:
	HighPriority(sint32 rv_tsk_id, sint32 rv_mbox_id);
	virtual ~HighPriority();

	virtual void start();

private:
	virtual void loop();
	virtual bool isOwnMessage();

};
#endif // !defined(EA_94B2C302_5605_4975_8286_36F9622C7EA3__INCLUDED_)
