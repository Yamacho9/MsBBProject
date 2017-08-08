//******************************************************************************
//ファイル名 : RunCourse.h
//
//概要 : コースクラスのヘッダーファイルです。
//
//備考 :
//******************************************************************************

#ifndef RUN_COURSE_H
#define RUN_COURSE_H


class Track;
class UI;


class RunCourse {
private:
	Track* m_track_list[10];
	int m_current_index;
	UI* m_UI;
	int m_list_num;

public:
	void addTrack(Track* track);
	RunCourse(UI* ui);
	bool startRun();
	void prepareRun();
	virtual ~RunCourse();
};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

