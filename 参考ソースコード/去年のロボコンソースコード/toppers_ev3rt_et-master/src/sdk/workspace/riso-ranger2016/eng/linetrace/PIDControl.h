///////////////////////////////////////////////////////////
//  PIDControl.h
//  Implementation of the Class PIDControl
//  Created on:      2016/07/22 19:45:36
//  Original author: 039889
///////////////////////////////////////////////////////////

#if !defined(EA_30AB8F19_198A_4fdd_BA57_17285CA74024__INCLUDED_)
#define EA_30AB8F19_198A_4fdd_BA57_17285CA74024__INCLUDED_

//#include "src\sdk\workspace\riso-ranger2016\eng\linetrace\PIDParam.h"

class PIDControl
{

public:
	PIDControl(float p, float i, float d);
	virtual ~PIDControl();
	int PIDCalculation(float reference, float currentVal);
        void setPID(float p, float i, float d);

private:
	float m_preerror;
	float m_sumerror;
	float m_P;
	float m_I;
	float m_D;

};
#endif // !defined(EA_30AB8F19_198A_4fdd_BA57_17285CA74024__INCLUDED_)
