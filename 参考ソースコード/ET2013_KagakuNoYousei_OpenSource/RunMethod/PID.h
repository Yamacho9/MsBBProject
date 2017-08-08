#ifndef PID_H
#define PID_H


class PID {

private:
	float m_P_coefficient;
	float m_I_coefficient;
	float m_D_coefficient;

	int m_d_now;
	int m_d_pre;
	int m_i_list[13];
	int m_num;

public:
	PID(float p_value,float i_value ,float d_value);
	int calcControllValue(int now_value);
	virtual ~PID();

};
#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

