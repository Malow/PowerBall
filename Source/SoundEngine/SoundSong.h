#pragma once

#include "stdafx.h"
#include <fmod.h> 
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_codec.h>
#include <fmod_output.h>
#include <fmod_dsp.h>

class SoundSong
{
private:
	FMOD::Channel*		mSongChannel;
	float				mSongVolume;	//default value: 1.0f
	FMOD::Sound*		mSound;
	FMOD::System*		sys;
	float*				masterVol;

public:
	SoundSong(FMOD::System* sys, float* masterVol);
	virtual ~SoundSong();

	void SetVolume(float volume);

	FMOD::Sound* GetSound() const { return this->mSound; }
	void SetSound(FMOD::Sound* s) { this->mSound = s; }

	/*! Play song at index. */
	void Play();
	/*! Mute song channel. */
	void Mute();
	/*! Unmute song channel. */
	void Unmute();
	/*! Pause song channel. */
	void Pause();
	/*! Unpause song channel. */
	void Unpause();


};