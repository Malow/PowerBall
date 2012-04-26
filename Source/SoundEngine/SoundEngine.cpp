#include "SoundEngine.h"

//private
void SoundEngine::ERRCHECK(FMOD_RESULT result)
{
#ifdef _DEBUG
	if (result != FMOD_OK)
	{
		//std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		OutputDebugString(FMOD_ErrorString(result));
	}
#endif
}

//public
SoundEngine::SoundEngine()
{
	//system
	this->mSystem = NULL;
	this->mResult = FMOD_ERR_NOTREADY;
	this->mVersion = 0;
	this->mNrOfDrivers = -1;
	this->mSpeakerMode = FMOD_SPEAKERMODE_STEREO;
	this->mCaps = FMOD_CAPS_NONE;
	this->mName = new char[256];

	//all sound
	this->mMasterVolume = 1.0f;

	//sound effects
	this->mSoundFXVolume = 1.0f;
	//sound effects 2D
	this->mNrOfSoundFX2D = 0;
	this->mSoundFXCap2D = 10;
	this->mSoundFX2D = new FMOD::Sound*[this->mSoundFXCap2D];
	for(unsigned int i = 0; i < this->mSoundFXCap2D; i++)
	{
		this->mSoundFX2D[i] = NULL;
	}
	this->mSoundFXChannel2D = NULL;
	//sound effects 3D
	this->mNrOfSoundFX3D = 0;
	this->mSoundFXCap3D = 10;
	this->mSoundFX3D = new FMOD::Sound*[this->mSoundFXCap3D];
	for(unsigned int i = 0; i < this->mSoundFXCap3D; i++)
	{
		this->mSoundFX3D[i] = NULL;
	}
	this->mSoundFXChannelsCap3D = 10;
	this->mSoundFXChannels3D = new FMOD::Channel*[this->mSoundFXChannelsCap3D];
	for(unsigned int i = 0; i < this->mSoundFXChannelsCap3D; i++)
	{
		this->mSoundFXChannels3D[i] = NULL;
	}
	this->mDistanceFactor = 100.0f;

	//songs
	this->mSongTimer = 0.0f;
	this->mSongStartTime = 0.0f;
	this->mCurrentSong = -1;
	this->mSongVolume = 1.0f;
	this->mNrOfSongs = 0;
	this->mSongsCap = 10;
	this->mSongs = new FMOD::Sound*[this->mSongsCap];
	for(unsigned int i = 0; i < this->mSongsCap; i++)
	{
		this->mSongs[i] = NULL;
	}
	this->mSongChannel = NULL;
}
SoundEngine::~SoundEngine()
{
	//sound effects
	for(unsigned int i = 0; i < this->mNrOfSoundFX2D; i++)
	{
		 if(this->mSoundFX2D[i])
		 {
			 ERRCHECK(this->mResult = this->mSoundFX2D[i]->release());
		 }
		 this->mSoundFX2D[i] = NULL;
	}
	SAFE_DELETE_ARRAY(this->mSoundFX2D);
	//do nothing with channel

	//songs
	for(unsigned int i = 0; i < this->mNrOfSongs; i++)
	{
		 if(this->mSongs[i])
		 {
			 ERRCHECK(this->mResult = this->mSongs[i]->release());
		 }
		 this->mSongs[i] = NULL;
	}
	SAFE_DELETE_ARRAY(this->mSongs);
	//do nothing with channel

	//system
	SAFE_DELETE(this->mName);
	if(this->mSystem)
	{
		ERRCHECK(this->mResult = this->mSystem->close());
		ERRCHECK(this->mResult = this->mSystem->release());
	}
}
int SoundEngine::Init()
{
	//system
	/*
		Create a System object and initialize.
	*/
	this->mResult = FMOD::System_Create(&this->mSystem);
	ERRCHECK(this->mResult);
	this->mResult = this->mSystem->getVersion(&this->mVersion);
	ERRCHECK(this->mResult);
	if(this->mVersion < FMOD_VERSION)
	{
		printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", this->mVersion, FMOD_VERSION);
		return 0;
	}
	this->mResult = this->mSystem->getNumDrivers(&this->mNrOfDrivers);
	ERRCHECK(this->mResult);
	if(this->mNrOfDrivers == 0)
	{
		this->mResult = this->mSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ERRCHECK(this->mResult);
	}
	else
	{
		this->mResult = this->mSystem->getDriverCaps(0, &this->mCaps, 0, &this->mSpeakerMode);
		ERRCHECK(this->mResult);
		/*
			Set the user selected speaker mode.
		*/
		this->mResult = this->mSystem->setSpeakerMode(this->mSpeakerMode);
		ERRCHECK(this->mResult);
		if(this->mCaps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			/*
				The user has the 'Acceleration' slider set to off! This is really bad
				for latency! You might want to warn the user about this.
			*/
			this->mResult = this->mSystem->setDSPBufferSize(1024, 10);
			ERRCHECK(this->mResult);
		}
		this->mResult = this->mSystem->getDriverInfo(0, this->mName, 256, 0);
		ERRCHECK(this->mResult);
		if(strstr(this->mName, "SigmaTel"))
		{
			/*
				Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
				PCM floating point output seems to solve it.
			*/
			this->mResult = this->mSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
			ERRCHECK(this->mResult);
		}
	}
	this->mResult = this->mSystem->init(100, FMOD_INIT_NORMAL, 0);
	if(this->mResult == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		/*
			Ok, the speaker mode selected isn't supported by this soundcard. Switch it
			back to stereo...
		*/
		this->mResult = this->mSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		ERRCHECK(this->mResult);
		/*
			... and re-init.
		*/
		this->mResult = this->mSystem->init(100, FMOD_INIT_NORMAL, 0);
	}
	ERRCHECK(this->mResult);



	//sound effects 3D
	ERRCHECK(this->mResult = this->mSystem->set3DSettings(1.0, this->mDistanceFactor, 1.0f));

	return 1;
}

//set
void SoundEngine::SetMasterVolume(float volume)
{
	this->mMasterVolume = volume;
	//update volume of channels
	this->mSoundFXChannel2D->setVolume(this->mSoundFXVolume * this->mMasterVolume);
	this->mSongChannel->setVolume(this->mSongVolume* this->mMasterVolume);
}
void SoundEngine::SetSoundEffectVolume(float volume)
{
	this->mSoundFXVolume = volume;
	//update channel
	this->mSoundFXChannel2D->setVolume(this->mSoundFXVolume * this->mMasterVolume);
}
void SoundEngine::SetSongVolume(float volume)
{
	this->mSongVolume = volume;
	//update channel
	this->mSongChannel->setVolume(this->mSongVolume * this->mMasterVolume);
}


//other
void SoundEngine::LoadSoundEffect(string filename, bool as3D)
{
	if(as3D)
	{
		//**
		// we should probably take the position, velocity, and orientation from the camera's vectors and matrix
		//ERRCHECK(this->mResult = this->mSystem->set3DListenerAttributes(0, &m_listenerpos, &m_velocity, &m_forward, &m_up) );
		FMOD_VECTOR pos, vel, forward, up;
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
		vel.x = 0;
		vel.y = 0;
		vel.z = 0;
		forward.x = 0;
		forward.y = 0;
		forward.z = 1;
		up.x = 0;
		up.y = 1;
		up.z = 0;
		ERRCHECK(this->mResult = this->mSystem->set3DListenerAttributes(0, &pos, &vel, &forward, &up));

		ERRCHECK(this->mResult = this->mSystem->createSound(filename.c_str(), FMOD_3D, NULL, &this->mSoundFX3D[this->mNrOfSoundFX3D++]));
		ERRCHECK(this->mResult = this->mSoundFX3D[this->mNrOfSoundFX3D++]->set3DMinMaxDistance(0.5f * this->mDistanceFactor, 5000.0f * this->mDistanceFactor));
		ERRCHECK(this->mResult = this->mSoundFX3D[this->mNrOfSoundFX3D++]->setMode(FMOD_LOOP_OFF));
	}
	else
	{
		ERRCHECK(this->mResult = this->mSystem->createSound(filename.c_str(), FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_OFF, NULL, &this->mSoundFX2D[this->mNrOfSoundFX2D++]));
	}
}
void SoundEngine::LoadSong(string filename, bool loop)
{
	FMOD_MODE mode = FMOD_HARDWARE | FMOD_2D;
	if(loop)
	{
		mode |= FMOD_LOOP_NORMAL;
	}
	ERRCHECK(this->mResult = this->mSystem->createStream(filename.c_str(), mode, NULL, &this->mSongs[this->mNrOfSongs++]));
}


void SoundEngine::PlaySoundEffect(unsigned int index)
{
	if(index < this->mNrOfSoundFX2D)
	{
		ERRCHECK(this->mResult = this->mSystem->playSound(FMOD_CHANNEL_FREE, this->mSoundFX2D[index], false, &this->mSoundFXChannel2D));
	}
	else
	{
		MaloW::Debug("SoundEngine: Warning: PlaySoundEffect(): Index out of bounds");
	}
}
void SoundEngine::PlaySong(unsigned int index)
{
	if(index < this->mNrOfSongs)
	{
		FMOD::Sound* currentSound;
		this->mSongChannel->getCurrentSound(&currentSound);
		if(currentSound != this->mSongs[index])
		{
			ERRCHECK(this->mResult = this->mSystem->playSound(FMOD_CHANNEL_REUSE, this->mSongs[index], false, &this->mSongChannel));
			this->mSongStartTime = this->mSongTimer;
			this->mCurrentSong = index;
		}
	}
	else
	{
		MaloW::Debug("SoundEngine: Warning: PlaySong(): Index out of bounds");
	}
}
void SoundEngine::RestartSong(unsigned int index)
{
	if(index < this->mNrOfSongs)
	{
		FMOD::Sound* currentSound;
		this->mSongChannel->getCurrentSound(&currentSound);
		if(currentSound == this->mSongs[index])
		{
			ERRCHECK(this->mResult = this->mSystem->playSound(FMOD_CHANNEL_REUSE, this->mSongs[index], false, &this->mSongChannel));
			this->mSongStartTime = this->mSongTimer;
			this->mCurrentSong = index;
		}
	}
	else
	{
		MaloW::Debug("SoundEngine: Warning: ResumeSong(): Index out of bounds");
	}
}
void SoundEngine::MuteSongChannel()
{
	this->mSongChannel->setMute(true);
}
void SoundEngine::UnmuteSongChannel()
{
	this->mSongChannel->setMute(false);
}

void SoundEngine::PauseSongChannel()
{
	ERRCHECK(this->mResult = this->mSongChannel->setPaused(true));
}
void SoundEngine::UnpauseSongChannel()
{
	ERRCHECK(this->mResult = this->mSongChannel->setPaused(false));
}

//**todo**
/*
void SoundEngine::Play3DSoundEffect(unsigned int soundEffectIndex, D3DXVECTOR3 position)
{
	FMOD_VECTOR pos = {position.x, position.y, position.z};
	FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f};

	//**todo**
	//ERRCHECK(this->mResult = this->mSystem->playSound(FMOD_CHANNEL_FREE, sound3D[number], true, &sfx3D[number]));
	//ERRCHECK(this->mResult = sfx3D[number]->set3DAttributes( &pos, &vel ) );
	//ERRCHECK(this->mResult = sfx3D[number]->setPaused(false) );
}

/*
void SoundEngine::PauseSong(unsigned int index)
{
	if(index < this->mNrOfSongs)
	{
		FMOD::Sound *currentSound;
		this->mSongChannel->getCurrentSound(&currentSound);
		if(currentSound == this->mSongs[index])
		{
			ERRCHECK(this->mResult = this->mSystem->playSound(FMOD_CHANNEL_REUSE, this->mSongs[index], true, &this->mSongChannel));
			
		}
	}
	else
	{
		MaloW::Debug("SoundEngine: Warning: PauseSong(): Index out of bounds");
	}
}

*/
