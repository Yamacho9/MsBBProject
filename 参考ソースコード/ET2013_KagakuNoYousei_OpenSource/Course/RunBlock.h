//******************************************************************************
//ファイル名 : RunBlock.h
//
//概要 : 走行区画クラスです。
//
//備考 :
//******************************************************************************


#ifndef RUN_BLOCK_H
#define RUN_BLOCK_H

class Condition;
class RunMethod;

#include "UI/UI.h"


class RunBlock
{

protected:
	Condition* m_end_condition;
	RunMethod* m_current_method;
	UI* m_pUI;

public:
	virtual bool start();
	RunBlock(Condition* end_con,RunMethod* cur_meth,UI* pUI);

	virtual ~RunBlock();
};


#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

