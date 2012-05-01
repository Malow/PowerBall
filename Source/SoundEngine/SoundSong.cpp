#include "SoundSong.h"

SoundSong::SoundSong(FMOD::System* sys, float* masterVol)
{
	this->masterVol = masterVol;
	this->sys = sys;
	this->mSongVolume = 1.0f;
	this->mSongChannel = NULL;
	this->mSound = NULL;
}

SoundSong::~SoundSong()
{
	if(this->mSound)
		this->mSound->release();
}

void SoundSong::Play()
{
	FMOD::Sound* currentSound = NULL;
	if(this->mSongChannel)
		this->mSongChannel->getCurrentSound(&currentSound);
	
	if(currentSound != this->mSound)
	{
		this->sys->playSound(FMOD_CHANNEL_REUSE, this->mSound, false, &this->mSongChannel);
		this->mSongChannel->setVolume(this->mSongVolume * *this->masterVol); //set volume of channel
	}
}

void SoundSong::SetVolume(float volume) 
{ 
	this->mSongVolume = volume; 
	if(this->mSongChannel)
		this->mSongChannel->setVolume(this->mSongVolume * *this->masterVol); //set volume of channel
}

void SoundSong::Mute()
{
	this->mSongChannel->setMute(true);
}
void SoundSong::Unmute()
{
	this->mSongChannel->setMute(false);
}

void SoundSong::Pause()
{
	this->mSongChannel->setPaused(true);
}
void SoundSong::Unpause()
{
	this->mSongChannel->setPaused(false);
}