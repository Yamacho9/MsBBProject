#ifndef GET_LOG_DATA_H
#define GET_LOG_DATA_H

#include "Daq.h"
using namespace ecrobot;

class GetLogData {

private:
	S8 m_data1;
	S8 m_data2;
	S32 m_data3;
	//ŠÖ˜A
	Daq* m_pDaq;

public:
	GetLogData(Daq* pDaq);
	virtual ~GetLogData();
	void getLogData();
	void setLogData1(S8 data1);
	void setLogData2(S8 data2);
	void setLogData3(S32 data3);

};


#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

