/*
 * TouchSensor.h
 *
 *  Created on: 2016/07/30
 *      Author: Reina
 */

#ifndef RISO_RANGER2016_SYS_DEVICE_TOUCHSENSOR_H_
#define RISO_RANGER2016_SYS_DEVICE_TOUCHSENSOR_H_

#include "ev3api.h"

class TouchSensor {
public:
	TouchSensor(sensor_port_t port);
	virtual ~TouchSensor();

	bool getSensorValue();

private:
	sensor_port_t m_Port;
	
};

#endif /* RISO_RANGER2016_SYS_DEVICE_TOUCHSENSOR_H_ */