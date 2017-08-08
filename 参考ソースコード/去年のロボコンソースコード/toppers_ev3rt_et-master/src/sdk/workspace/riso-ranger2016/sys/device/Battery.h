/*
 * Battery.h
 *
 *  Created on: 2016/08/05
 *      Author: ryuji
 */

#ifndef WORKSPACE_RISO_RANGER2016_SYS_DEVICE_BATTERY_H_
#define WORKSPACE_RISO_RANGER2016_SYS_DEVICE_BATTERY_H_

#include "ev3api.h"

class Battery {
public:
	Battery();
	virtual ~Battery();
	int getValue();
};

#endif /* WORKSPACE_RISO_RANGER2016_SYS_DEVICE_BATTERY_H_ */
