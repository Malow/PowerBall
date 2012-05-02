#include "SoundEffect.h"

SoundEffect::SoundEffect(FMOD::System* sys, FMOD::Channel* chan, float* masterVol)
{
	this->masterVol = masterVol;
	this->chan = chan;
	this->sys = sys;
	this->mSoundFXVolume = 1.0f;
	this->mSoundFX2D = NULL;
}

SoundEffect::~SoundEffect()
{
	if(this->mSoundFX2D)
		this->mSoundFX2D->release();
}

void SoundEffect::Play()
{
	this->sys->playSound(FMOD_CHANNEL_FREE, this->mSoundFX2D, false, &this->chan);
	this->chan->setVolume(this->mSoundFXVolume * *this->masterVol); //set volume of channel
}

void SoundEffect::SetVolume(float volume) 
{ 
	this->mSoundFXVolume = volume; 
	if(this->chan)
		this->chan->setVolume(this->mSoundFXVolume * *this->masterVol); //set volume of channel
}