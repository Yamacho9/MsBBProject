#ifndef SOUND_H
#define SOUND_H

#include "Speaker.h"

using namespace ecrobot;

class Sound {

private:
	int m_sound_existence;

	//ŠÖ˜A
	Speaker* m_pSpeaker;

public:
	Sound(Speaker* pSpeaker);
	void beat();
	void instructBeat();
	virtual ~Sound();


};

#endif


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

