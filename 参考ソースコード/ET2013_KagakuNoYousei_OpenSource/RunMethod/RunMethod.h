//******************************************************************************
//�t�@�C���� : RunMethod.h
//
//�T�v : ���s���@�N���X�i�X�[�p�[�N���X�j�ł��B
//
//���l : �X�[�p�[�N���X�Ȃ̂ŏ����͋L�q����Ă��܂���B
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

