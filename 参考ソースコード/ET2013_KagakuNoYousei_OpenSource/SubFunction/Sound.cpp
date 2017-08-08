#include "SubFunction/Sound.h"

#include "Speaker.h"

using namespace ecrobot;

Sound::Sound(Speaker* pSpeaker)
	:m_pSpeaker(pSpeaker) {
}

Sound::~Sound() {
}

void Sound::beat() {

	m_pSpeaker->playTone(200,200,100);

}


void Sound::instructBeat() {

	beat();

}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

