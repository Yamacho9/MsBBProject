#ifndef REMOTESTART_H
#define REMOTESTART_H

#include "Bluetooth.h"

using namespace ecrobot;

class RemoteStart {

private:

	//ŠÖ˜A
	Bluetooth* m_pBluetooth;

	const char CMD_START;



public:
	virtual ~RemoteStart();
	RemoteStart(Bluetooth* pBluetooth);
	bool remote_start();

};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

