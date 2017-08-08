///////////////////////////////////////////////////////////
//  Calibration.h
//  Implementation of the Class Calibration
//  Created on:      2016/07/05 11:43:06
//  Original author: 039918
///////////////////////////////////////////////////////////

#if !defined(EA_C665904E_761B_426a_B0DB_BE0B2866A22C__INCLUDED_)
#define EA_C665904E_761B_426a_B0DB_BE0B2866A22C__INCLUDED_

#include "ev3api.h"
#include "TouchSensorListener.h"
#include "ColorSensor.h"

class Calibration : TouchSensorListener
{

public:
	Calibration(ColorSensor* colorSensor);
	virtual ~Calibration();

	void onPressed();	//override
	void onReleased();	//override
	float GetNormalizedSensorValue();

private:
	ColorSensor* m_ColorSensor;

	int m_LightValueMax;
	int m_LightValueMin;

	int m_CalibrationIndex;

	void ConfigureWhiteValue();
	void ConfigureLineValue();

};
#endif // !defined(EA_C665904E_761B_426a_B0DB_BE0B2866A22C__INCLUDED_)
