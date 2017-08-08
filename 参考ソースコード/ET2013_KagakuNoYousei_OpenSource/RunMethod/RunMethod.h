//******************************************************************************
//ファイル名 : RunMethod.h
//
//概要 : 走行方法クラス（スーパークラス）です。
//
//備考 : スーパークラスなので処理は記述されていません。
//******************************************************************************

#ifndef RUN_METHOD_H
#define RUN_METHOD_H

class TailController;
#include "RunMethod/Posture.h"

class RunMethod {

protected:
	Posture* m_posture;


public:
	RunMethod();
	virtual void run() = 0;
	virtual ~RunMethod();


};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

