///////////////////////////////////////////////////////////
//  SubsystemBridge.h
//  Implementation of the Class SubsystemBridge
//  Created on:      2016/04/13 12:52:15
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_6B6C30FD_F2FC_404a_9BEA_EF84C743FB75__INCLUDED_)
#define EA_6B6C30FD_F2FC_404a_9BEA_EF84C743FB75__INCLUDED_

#include "std.h"

class SubsystemBridge
{

public:
	SubsystemBridge(uint16 rv_subsys_id, sint32 rv_mbox_id);
	virtual ~SubsystemBridge();
	void doInitialize();
	void receiveMessage(uint16* rv_message);


protected:
    // sendMessage -> receiveMessage のシーケンスは自作自演方式.
    // スレッドを切りたい場合は本クラスでこの対応を取る.
	void sendMessage(uint16 rv_msg_code, void* rv_message, uint16 rv_size);

private:
    uint16 subsys_id;
	sint32 mbox_id;
	virtual void actInitialize();
	virtual void actReceiveMessage(uint16 rv_msg_code, uint16* rv_message);
};
#endif // !defined(EA_6B6C30FD_F2FC_404a_9BEA_EF84C743FB75__INCLUDED_)
