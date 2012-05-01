#pragma once

#include "stdafx.h"
#include <fmod.h> 
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_codec.h>
#include <fmod_output.h>
#include <fmod_dsp.h>

class SoundEffect
{
private:
	float				mSoundFXVolume;	//default value: 1.0f
	FMOD::Sound*		mSoundFX2D;
	FMOD::System*		sys;
	FMOD::Channel*		chan;
	float*				masterVol;

public:
	SoundEffect(FMOD::System* sys, FMOD::Channel* chan, float* masterVol);
	virtual ~SoundEffect();

	void SetVolume(float volume);

	FMOD::Sound* GetSound() const { return this->mSoundFX2D; }
	void SetSound(FMOD::Sound* s) { this->mSoundFX2D = s; }

	/*! Play sound effect at index. */
	void Play();

};