///////////////////////////////////////////////////////////
//  PIDControl.cpp
//  Implementation of the Class PIDControl
//  Created on:      2016/07/22 19:45:36
//  Original author: 039889
///////////////////////////////////////////////////////////

#include "PIDControl.h"


PIDControl::PIDControl(float p, float i, float d){

	// メンバ初期化
	m_P = p;
	m_I = i;
	m_D = d;
	m_preerror = 0;
	m_sumerror = 0;

}


PIDControl::~PIDControl(){

}


int PIDControl::PIDCalculation(float reference, float currentVal){

	int result = 0;
	float error = 0;

	// 偏差
	error = reference - currentVal;

	// 積分計算
	m_sumerror += error;

	// 操作量計算
	result = (m_P*error) + (m_I*m_sumerror) + m_D*(error - m_preerror);

	// 微分計算用誤差
	m_preerror = error;

	return result;
}

void PIDControl::setPID(float p, float i, float d) {
	m_P = p;
	m_I = i;
	m_D = d;
}
