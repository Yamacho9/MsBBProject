//******************************************************************************
//ファイル名 : AllDistanceCondition.h
//
//概要 : 条件クラス（スーパークラス）のサブクラスです。総走行距離の検知を行います。
//
//備考 :
//******************************************************************************

#ifndef ALLDISTANCE_CONDITION_H
#define ALLDISTANCE_CONDITION_H

#include "Condition.h"
#include "Motor.h"
using namespace ecrobot;

class AllDistanceCondition:public Condition {

private:
	int m_target_alldistance;
	//関連
	Motor* m_pLeftMotor;
	Motor* m_pRightMotor;

public:
	bool isSatisfy();
	AllDistanceCondition(int t_adis,Motor* pLeftMotor,Motor* pRightMotor);
	virtual ~AllDistanceCondition();
};


#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

