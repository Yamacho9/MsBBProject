//******************************************************************************
//ファイル名 : Condition.h
//
//概要 : 条件クラス（スーパークラス）のヘッダーファイルです。
//
//備考 : スーパークラスなので処理は記述されていません。
//******************************************************************************

#ifndef CONDITION_H
#define CONDITION_H

class Condition {

public:
	Condition::Condition();
	virtual bool isSatisfy() = 0;	//純粋仮想関数になるので「= 0」を付けます
	virtual ~Condition();

};


#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

