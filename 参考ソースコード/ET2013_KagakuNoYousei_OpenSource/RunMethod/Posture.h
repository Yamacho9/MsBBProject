#ifndef POSTURE_H
#define POSTURE_H


class Posture {

public:
	enum PostureType {VER,INC};
	Posture::Posture();
	virtual void adjust(int speed,signed char circle,signed char& L,signed char& R)=0;
	virtual ~Posture();

private:
	virtual void changePosture();

protected:
	static enum PostureType m_current_posture;
};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

