#include "RunMethod/PID.h"

PID::PID(float p_value,float i_value ,float d_value) {

	m_P_coefficient = p_value;
	m_I_coefficient = i_value;
	m_D_coefficient = d_value;
	m_d_now = 0;
	m_d_pre = 0;
	m_num = 0;
	for(int i=0; i<13; i++) {
		m_i_list[i]=0;
	}

}

PID::~PID() {
}

int PID::calcControllValue(int now_value) {
	float d_control = 0;
	float p_control = 0;
	float i_control = 0;
	int pid_total = 0;
	int total_i = 0;

	//P§ŒäŒvŽZ
	p_control = now_value * m_P_coefficient;

	//D§ŒäŒvŽZ
	m_d_pre = m_d_now;
	m_d_now = now_value;
	d_control = (m_d_pre - m_d_now) * m_D_coefficient;

	//I§ŒäŒvŽZ
	if(m_num>=13) {
		m_num = 0;
	}
	m_i_list[m_num] = now_value;
	for(int i=0; i<13; i++) {
		total_i += m_i_list[i];
	}
	m_num ++;
	i_control = total_i * m_I_coefficient;

	//PIDŽZo’l
	pid_total = static_cast<int>(p_control + i_control + d_control);

	return pid_total;
}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

