#include "Communication/GetLogData.h"
#include "Daq.h"

using namespace ecrobot;

GetLogData::GetLogData(Daq* pDaq):m_pDaq(pDaq) {
}

GetLogData::~GetLogData() {
}

void GetLogData::setLogData1(S8 data1) {

	m_data1 = data1;
}
void GetLogData::setLogData2(S8 data2) {

	m_data2 = data2;
}
void GetLogData::setLogData3(S32 data3) {

	m_data3 = data3;
}

void GetLogData::getLogData() {

	S8 dataS08[2] = {0};
	S32 dataS32 = 0;

	dataS08[0] = m_data1;
	dataS08[1] = m_data2;
	dataS32 = m_data3;

	m_pDaq->send(dataS08, dataS32);

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

