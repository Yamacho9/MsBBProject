/*
 * Battery.cpp
 *
 *  Created on: 2016/08/05
 *      Author: ryuji
 */

#include "Battery.h"

Battery::Battery() {

}

Battery::~Battery() {

}


int Battery::getValue(){
	return ev3_battery_voltage_mV();
}
