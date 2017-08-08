///////////////////////////////////////////////////////////
//  Engine.h
//  Implementation of the Class Engine
//  Created on:      2016/05/02 0:52:06
//  Original author: tak
///////////////////////////////////////////////////////////

#if !defined(EA_2D7BA025_7C6A_4b77_95D5_F437A38B9B98__INCLUDED_)
#define EA_2D7BA025_7C6A_4b77_95D5_F437A38B9B98__INCLUDED_

#include "Task.h"

class Engine : public Task
{

public:
	Engine(sint32 rv_tsk_id, sint32 rv_mbox_id);
	virtual ~Engine();

	virtual void start();

private:
	virtual void loop();

};
#endif // !defined(EA_2D7BA025_7C6A_4b77_95D5_F437A38B9B98__INCLUDED_)
