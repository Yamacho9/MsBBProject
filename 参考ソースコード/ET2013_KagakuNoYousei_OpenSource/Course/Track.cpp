//******************************************************************************
//�t�@�C���� : Track.cpp
//
//�T�v : ���H�N���X�ł��B
//
//���l :
//******************************************************************************

#include "Track.h"
#include "RunBlock.h"

Track::Track() {

	m_current_index = 0;
	m_list_num=0;
}

Track::~Track() {
}

void Track::addBlock(RunBlock* block) {

	m_block_list[m_list_num] = block;
	m_list_num++;
}

bool Track::start() {


	if(m_block_list[m_current_index] -> start() == true) {
		m_current_index++;
	}


	if(m_list_num <= m_current_index) {
		m_current_index=0;
		return true;
	}
	else {
		return false;
	}

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

