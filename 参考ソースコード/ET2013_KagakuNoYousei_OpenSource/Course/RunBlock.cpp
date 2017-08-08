//******************************************************************************
//ファイル名 : RunBlock.cpp
//
//概要 : 走行区画クラスです。
//
//備考 :
//******************************************************************************

#include "Course/RunBlock.h"
#include "Condition/Condition.h"
#include "RunMethod/RunMethod.h"
#include "UI/UI.h"

RunBlock::RunBlock(Condition* end_con,RunMethod* cur_meth,UI* pUI) {

	m_end_condition = end_con;
	m_current_method = cur_meth;
	m_pUI = pUI;
}

RunBlock::~RunBlock() {
}

bool RunBlock::start() {

	bool i = false;
	i = m_end_condition->isSatisfy();
	m_current_method->run();

	if ( i == true) {

		m_pUI->setSound();
		return true;

	}
	else {
		return false;
	}
}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

