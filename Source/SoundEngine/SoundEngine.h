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
		//system
		FMOD::System*		mSystem;
		FMOD_RESULT			mResult;		//default value: FMOD_ERR_NOTREADY.
		unsigned int		mVersion;
		int					mNrOfDrivers;
		FMOD_SPEAKERMODE	mSpeakerMode;	//default value: FMOD_SPEAKERMODE_STEREO.
		FMOD_CAPS			mCaps;			//default value: FMOD_CAPS_NONE.
		char*				mName;

		//all sound
		float				mMasterVolume;	//default value: 1.0f

		//sound effects 
		float				mSoundFXVolume;	//default value: 1.0f
		//sound effects 2D
		unsigned int		mNrOfSoundFX2D;
		unsigned int		mSoundFXCap2D;
		FMOD::Sound**		mSoundFX2D;
		FMOD::Channel*		mSoundFXChannel2D;
		//sound effects 3D todo**
		unsigned int		mNrOfSoundFX3D;
		unsigned int		mSoundFXCap3D;
		FMOD::Sound**		mSoundFX3D;
		unsigned int		mSoundFXChannelsCap3D;
		FMOD::Channel**		mSoundFXChannels3D;
		float				mDistanceFactor;//default value: 100.0f. Units per meter (100 cm).

		//songs (2D)
		float				mSongVolume;	//default value: 1.0f
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

		//Set functions
		/*! Set the master volume. Range is [0,1]. */
		void SetMasterVolume(float volume);
		/*! Set the sound effect volume. Range is [0,1]. */
		void SetSoundEffectVolume(float volume);
		/*! Set the song volume. Range is [0,1]. */
		void SetSongVolume(float volume);

		//Sound effects
		/*! Load sound effect from soundfile. **3D currently unavailable** */
		void LoadSoundEffect(string filename, bool as3D);
		/*! Play sound effect at index. */
		void PlaySoundEffect(unsigned int index);
		
		//Song
		/*! Load song from soundfile. */
		void LoadSong(string filename, bool loop);
		/*! Play song at index. */
		void PlaySong(unsigned int songIndex);
		/*! Mute song channel. */
		void MuteSongChannel();
		/*! Unmute song channel. */
		void UnmuteSongChannel();
		/*! Pause song channel. */
		void PauseSongChannel();
		/*! Unpause song channel. */
		void UnpauseSongChannel();


		//**todo**
		//void Play3DSoundEffect(unsigned int soundEffectIndex, D3DXVECTOR3 position);
		
};