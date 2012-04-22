#pragma once

#include "..\stdafx.h"
#include <fmod.h> //<..\SoundEngine\Include\fmod.h>
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

	private:
		void ERRCHECK(FMOD_RESULT result);

	public:
		SoundEngine();
		virtual ~SoundEngine();
		int Init();
};