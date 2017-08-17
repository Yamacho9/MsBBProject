//******************************************************************************
//�t�@�C���� : Track.h
//
//�T�v : ���H�̃N���X
//
//���l :
//******************************************************************************

#ifndef TRACK_H
#define TRACK_H


class RunBlock;

class Track {
private:
	RunBlock* m_block_list[30];		//RanBlock��30�܂Őݒ�ł���Ƃ������Ƃł�
	int m_current_index;
	int m_list_num;

public:
	bool start();
	void addBlock(RunBlock* block);
	Track();
	virtual ~Track();

};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

