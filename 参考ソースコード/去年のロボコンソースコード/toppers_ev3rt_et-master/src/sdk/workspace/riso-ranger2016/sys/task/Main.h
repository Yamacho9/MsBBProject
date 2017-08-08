///////////////////////////////////////////////////////////
//  Main.h
//  Implementation of the Class Main
//  Created on:      2016/04/28 11:49:13
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_1786ABD9_CBEF_4442_86F2_F722887FE3A0__INCLUDED_)
#define EA_1786ABD9_CBEF_4442_86F2_F722887FE3A0__INCLUDED_

#include "Task.h"
#include "app.h"


class Main : public Task
{

public:
	virtual ~Main();

	virtual void start();
	inline Task* get_the_task(EmTaskId rv_id);
    static Main* getInstance();

private:
    Main(sint32 rv_tsk_id, sint32 rv_mbox_id);
    
	Task *the_task[kMaxTaskId];
    static Main* instance;
    
	virtual void loop();

};

inline Task* Main::get_the_task(EmTaskId rv_id){

	return the_task[rv_id];
}

#endif // !defined(EA_1786ABD9_CBEF_4442_86F2_F722887FE3A0__INCLUDED_)
