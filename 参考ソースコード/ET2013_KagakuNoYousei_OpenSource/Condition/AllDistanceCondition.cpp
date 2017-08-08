//******************************************************************************
//ファイル名 : AllDistanceCondition.cpp
//
//概要 : 条件クラス（スーパークラス）のサブクラスです。総走行距離の検知を行います。
//
//備考 :
//******************************************************************************

#include "Condition/AllDistanceCondition.h"
#include "Motor.h"

using namespace ecrobot;

AllDistanceCondition::AllDistanceCondition(int t_adis,Motor* pLeftMotor,Motor* pRightMotor)
	:m_pLeftMotor(pLeftMotor),m_pRightMotor(pRightMotor) {									//t_adis :　target_alldistanceの略

	m_target_alldistance = t_adis;

}

AllDistanceCondition::~AllDistanceCondition() {
}

bool AllDistanceCondition::isSatisfy() {

	int angle;	//回転角度
	angle = static_cast<int>( ( m_pLeftMotor->getCount() + m_pRightMotor->getCount() )/2 );

	//走行距離[cm] = 回転角度 × 8cm（車輪直径）× 円周率（3.14）／ 360（周角）
	//             = 回転角度 × 約0.0698
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

