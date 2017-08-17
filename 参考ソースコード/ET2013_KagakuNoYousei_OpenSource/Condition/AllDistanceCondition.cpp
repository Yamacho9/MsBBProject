//******************************************************************************
//�t�@�C���� : AllDistanceCondition.cpp
//
//�T�v : �����N���X�i�X�[�p�[�N���X�j�̃T�u�N���X�ł��B�����s�����̌��m���s���܂��B
//
//���l :
//******************************************************************************

#include "Condition/AllDistanceCondition.h"
#include "Motor.h"

using namespace ecrobot;

AllDistanceCondition::AllDistanceCondition(int t_adis,Motor* pLeftMotor,Motor* pRightMotor)
	:m_pLeftMotor(pLeftMotor),m_pRightMotor(pRightMotor) {									//t_adis :�@target_alldistance�̗�

	m_target_alldistance = t_adis;

}

AllDistanceCondition::~AllDistanceCondition() {
}

bool AllDistanceCondition::isSatisfy() {

	int angle;	//��]�p�x
	angle = static_cast<int>( ( m_pLeftMotor->getCount() + m_pRightMotor->getCount() )/2 );

	//���s����[cm] = ��]�p�x �~ 8cm�i�ԗ֒��a�j�~ �~�����i3.14�j�^ 360�i���p�j
	//             = ��]�p�x �~ ��0.0698
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

