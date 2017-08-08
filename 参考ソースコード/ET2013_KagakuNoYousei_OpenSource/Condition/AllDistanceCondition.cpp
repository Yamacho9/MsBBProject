//******************************************************************************
//t@CΌ : AllDistanceCondition.cpp
//
//Tv : πNXiX[p[NXjΜTuNXΕ·Bs£Μmπs’ά·B
//
//υl :
//******************************************************************************

#include "Condition/AllDistanceCondition.h"
#include "Motor.h"

using namespace ecrobot;

AllDistanceCondition::AllDistanceCondition(int t_adis,Motor* pLeftMotor,Motor* pRightMotor)
	:m_pLeftMotor(pLeftMotor),m_pRightMotor(pRightMotor) {									//t_adis :@target_alldistanceΜͺ

	m_target_alldistance = t_adis;

}

AllDistanceCondition::~AllDistanceCondition() {
}

bool AllDistanceCondition::isSatisfy() {

	int angle;	//ρ]px
	angle = static_cast<int>( ( m_pLeftMotor->getCount() + m_pRightMotor->getCount() )/2 );

	//s£[cm] = ρ]px ~ 8cmiΤΦΌaj~ ~ό¦i3.14j^ 360iόpj
	//             = ρ]px ~ ρ0.0698
	if(m_target_alldistance <= angle * 0.0698)
	{
		return true;
	}

	else
	{
		return false;
	}

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

