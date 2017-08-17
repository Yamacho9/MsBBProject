//******************************************************************************
//�t�@�C���� : AllDistanceCondition.h
//
//�T�v : �����N���X�i�X�[�p�[�N���X�j�̃T�u�N���X�ł��B�����s�����̌��m���s���܂��B
//
//���l :
//******************************************************************************

#ifndef ALLDISTANCE_CONDITION_H
#define ALLDISTANCE_CONDITION_H

#include "Condition.h"
#include "Motor.h"
using namespace ecrobot;

class AllDistanceCondition:public Condition {

private:
	int m_target_alldistance;
	//�֘A
	Motor* m_pLeftMotor;
	Motor* m_pRightMotor;

public:
	bool isSatisfy();
	AllDistanceCondition(int t_adis,Motor* pLeftMotor,Motor* pRightMotor);
	virtual ~AllDistanceCondition();
};


#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

