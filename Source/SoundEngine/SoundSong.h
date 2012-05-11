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
	float				mSongVolume;	//default value: 1.0f
	FMOD::Sound*		mSound;
	FMOD::System*		sys;
	FMOD::Channel*		mSongChannel;
	float*				masterVol;

private:
	void ERRCHECK(FMOD_RESULT result);

public:
	SoundSong(FMOD::System* sys, float* masterVol);
	virtual ~SoundSong();

	void SetVolume(float volume);

	FMOD::Sound* GetSound() const { return this->mSound; }
	void SetSound(FMOD::Sound* s) { this->mSound = s; }
	float GetVolume() { return this->mSongVolume; }

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