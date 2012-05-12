#pragma once
#include "SoundEngine\SoundEngine.h"

struct BackgroundSong
{
	/*Options for sound*/
	static SoundSong* mSong;
	static bool mPlaying;
	static SoundEffect* mMouseClick;
	static SoundEngine* mSe;
};