//******************************************************************************
//�t�@�C���� : Condition.h
//
//�T�v : �����N���X�i�X�[�p�[�N���X�j�̃w�b�_�[�t�@�C���ł��B
//
//���l : �X�[�p�[�N���X�Ȃ̂ŏ����͋L�q����Ă��܂���B
//******************************************************************************

#ifndef CONDITION_H
#define CONDITION_H

class Condition {

public:
	Condition::Condition();
	virtual bool isSatisfy() = 0;	//�������z�֐��ɂȂ�̂Łu= 0�v��t���܂�
	virtual ~Condition();

};


#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

