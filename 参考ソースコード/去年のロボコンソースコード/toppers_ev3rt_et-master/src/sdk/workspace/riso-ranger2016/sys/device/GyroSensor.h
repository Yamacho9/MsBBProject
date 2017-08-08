/*
 * GyroSensor.h
 *
 *  Created on: 2016/08/05
 *      Author: ryuji
 */

#ifndef WORKSPACE_RISO_RANGER2016_SYS_DEVICE_GYROSENSOR_H_
#define WORKSPACE_RISO_RANGER2016_SYS_DEVICE_GYROSENSOR_H_

#include "ev3api.h"

class GyroSensor {
public:
	GyroSensor(sensor_port_t portNum);
	virtual ~GyroSensor();
	int getSensorValue();
	int getGyroOffset();
	void reset();

private:
	sensor_port_t m_Port;			// 設定ポート
	const int c_GyroOffset = 4;		// ジャイロオフセット(センサ固有の計測値ずれ量 倒立制御に必要) TODO 実測して設定
};

#endif /* WORKSPACE_RISO_RANGER2016_SYS_DEVICE_GYROSENSOR_H_ */
