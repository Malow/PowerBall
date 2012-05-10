#include "SoundSong.h"

void SoundSong::ERRCHECK(FMOD_RESULT result)
{
#ifdef _DEBUG
	if (result != FMOD_OK)
	{
		OutputDebugString(FMOD_ErrorString(result));
	}
#endif
}



SoundSong::SoundSong(FMOD::System* sys, float* masterVol)
{
	this->mSongVolume = 1.0f;
	this->mSound = NULL;
	this->sys = sys;
	this->mSongChannel = NULL;
	this->masterVol = masterVol;
}

SoundSong::~SoundSong()
{
	if(this->mSound)
		this->mSound->release();
}

void SoundSong::Play()
{
	FMOD_RESULT fr;
	FMOD::Sound* currentSound = NULL;
	if(this->mSongChannel)
		ERRCHECK(fr = this->mSongChannel->getCurrentSound(&currentSound));
	
	if(currentSound != this->mSound)
	{
		ERRCHECK(fr = this->sys->playSound(FMOD_CHANNEL_FREE, this->mSound, false, &this->mSongChannel));
		ERRCHECK(fr = this->mSongChannel->setVolume(this->mSongVolume * *this->masterVol)); //set volume of channel
	}
}

void SoundSong::SetVolume(float volume) 
{ 
	FMOD_RESULT fr;
	this->mSongVolume = volume; 
	if(this->mSongChannel)
		ERRCHECK(fr = this->mSongChannel->setVolume(this->mSongVolume * *this->masterVol)); //set volume of channel
}

void SoundSong::Mute()
{
	FMOD_RESULT fr;
	ERRCHECK(fr = this->mSongChannel->setMute(true));
}
void SoundSong::Unmute()
{
	FMOD_RESULT fr;
	ERRCHECK(fr = this->mSongChannel->setMute(false));
}

void SoundSong::Pause()
{
	FMOD_RESULT fr;
	ERRCHECK(fr = this->mSongChannel->setPaused(true));
}
void SoundSong::Unpause()
{
	FMOD_RESULT fr;
	ERRCHECK(fr = this->mSongChannel->setPaused(false));
}