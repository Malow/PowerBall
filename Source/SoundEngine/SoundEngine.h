/*!
	Simple class for playing sounds and songs in 2D.
*/


#pragma once

#include "..\stdafx.h"
#include <fmod.h> 
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_codec.h>
#include <fmod_output.h>
#include <fmod_dsp.h>

class SoundEngine
{
	private:
		FMOD::System*		mSystem;
		FMOD_RESULT			mResult;		//default value: FMOD_ERR_NOTREADY.
		unsigned int		mVersion;
		int					mNrOfDrivers;
		FMOD_SPEAKERMODE	mSpeakerMode;	//default value: FMOD_SPEAKERMODE_STEREO.
		FMOD_CAPS			mCaps;			//default value: FMOD_CAPS_NONE.
		char*				mName;

		unsigned int		mNrOfSoundFX;
		unsigned int		mSoundFXCap;
		FMOD::Sound**		mSoundFX;
		FMOD::Channel*		mSoundFXChannel2D;

		unsigned int		mNrOfSongs;
		unsigned int		mSongsCap;
		FMOD::Sound**		mSongs;
		FMOD::Channel*		mSongChannel;

	private:
		void ERRCHECK(FMOD_RESULT result);

	public:
		SoundEngine();
		virtual ~SoundEngine();
		int Init();

		void LoadSoundEffect(string filename);
		void LoadSong(string filename);

		void PlaySoundEffect(unsigned int index);
		void PlaySong(unsigned int index);

		void PauseSong(unsigned int index);
		void ResumeSong(unsigned int index); //**börjar om från början**

		
};