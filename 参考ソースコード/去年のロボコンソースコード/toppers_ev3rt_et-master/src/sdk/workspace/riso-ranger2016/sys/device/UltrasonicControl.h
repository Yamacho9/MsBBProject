///////////////////////////////////////////////////////////
//  UltrasonicControl.h
//  Implementation of the Class UltrasonicControl
//  Created on:      2016/07/15 16:04:13
//  Original author: 039889
///////////////////////////////////////////////////////////

#if !defined(EA_AD0BBBF1_1A41_451b_9449_09D1A5901DD2__INCLUDED_)
#define EA_AD0BBBF1_1A41_451b_9449_09D1A5901DD2__INCLUDED_

#include "ev3api.h"

class UltrasonicControl
{

public:
	UltrasonicControl(sensor_port_t PortNum);
	virtual ~UltrasonicControl();
	int16_t getUltrasonicValue();

private:
	sensor_port_t m_Port;

};
#endif // !defined(EA_AD0BBBF1_1A41_451b_9449_09D1A5901DD9__INCLUDED_)
