/*
 * TouchSensorListener.h
 *
 *  Created on: 2016/07/23
 *      Author: ryuji
 */

#ifndef RISO_RANGER2016_EVENT_TOUCHSENSORLISTENER_H_
#define RISO_RANGER2016_EVENT_TOUCHSENSORLISTENER_H_

class TouchSensorListener {
public:
	TouchSensorListener();
	virtual ~TouchSensorListener();

	virtual void onPressed()=0;
	virtual void onReleased()=0;
};

#endif /* RISO_RANGER2016_EVENT_TOUCHSENSORLISTENER_H_ */
