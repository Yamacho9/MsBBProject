//******************************************************************************
//ファイル名 : Track.h
//
//概要 : 走路のクラス
//
//備考 :
//******************************************************************************

#ifndef TRACK_H
#define TRACK_H


class RunBlock;

class Track {
private:
	RunBlock* m_block_list[30];		//RanBlockは30まで設定できるということです
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

