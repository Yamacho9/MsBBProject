#include "Course/RunCourse.h"
#include "Course/Track.h"
#include "UI/UI.h"

RunCourse::RunCourse(UI* ui) {

	m_current_index = 0;
	m_list_num = 0;
	m_UI = ui;

}

RunCourse::~RunCourse() {
}

void RunCourse::addTrack(Track* track) {

	m_track_list[m_list_num] = track;
	m_list_num++;
}
void RunCourse::prepareRun() {
	m_UI->initialize();
	m_UI->tailBalance();
	m_UI->prepareRun();

}

bool RunCourse::startRun() {

	if(m_track_list[m_current_index] -> start() == true) {
		m_current_index++;
	}
	if(m_list_num <= m_current_index) {
		m_current_index= 0;
		return true;
	}
	else {
		return false;
	}

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

