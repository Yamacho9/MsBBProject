///////////////////////////////////////////////////////////
//  ColorSensor.h
//  Implementation of the Class ColorSensor
//  Created on:      2016/07/26 21:04:52
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_19AB5B12_0A19_403d_A508_F891E15DCAFD__INCLUDED_)
#define EA_19AB5B12_0A19_403d_A508_F891E15DCAFD__INCLUDED_

#include "ev3api.h"

class ColorSensor
{

public:
	ColorSensor(sensor_port_t port);
	virtual ~ColorSensor();

	int getSensorValue();

private:
	sensor_port_t m_Port;

};
#endif // !defined(EA_19AB5B12_0A19_403d_A508_F891E15DCAFD__INCLUDED_)
