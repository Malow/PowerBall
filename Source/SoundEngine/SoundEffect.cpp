#include "SoundEffect.h"

void SoundEffect::ERRCHECK(FMOD_RESULT result)
{
#ifdef _DEBUG
	if (result != FMOD_OK)
	{
		OutputDebugString(FMOD_ErrorString(result));
	}
#endif
}




/*
SoundEffect::SoundEffect(FMOD::System* sys, FMOD::Channel* chan, float* masterVol)
{
	this->masterVol = masterVol;
	this->chan = chan;
	this->sys = sys;
	this->mSoundFXVolume = 1.0f;
	this->mSoundFX2D = NULL;
}
*/
SoundEffect::SoundEffect(FMOD::System* sys, float* masterVol)
{
	this->mSoundFXVolume = 1.0f;
	this->mSoundFX2D = NULL;
	this->sys = sys;
	this->chan = NULL;
	this->masterVol = masterVol;
}
SoundEffect::~SoundEffect()
{
	if(this->mSoundFX2D)
		this->mSoundFX2D->release();
}

void SoundEffect::Play()
{
	FMOD_RESULT fr;
	ERRCHECK(fr = this->sys->playSound(FMOD_CHANNEL_REUSE, this->mSoundFX2D, false, &this->chan));
	ERRCHECK(fr = this->chan->setVolume(this->mSoundFXVolume * *this->masterVol)); //set volume of channel
}

void SoundEffect::SetVolume(float volume) 
{ 
	FMOD_RESULT fr;
	this->mSoundFXVolume = volume; 
	if(this->chan)
		ERRCHECK(fr = this->chan->setVolume(this->mSoundFXVolume * *this->masterVol)); //set volume of channel
}