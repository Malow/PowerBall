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

	//sound effects
	this->mNrOfSoundFX = 0;
	this->mSoundFXCap = 10;
	this->mSoundFX = new FMOD::Sound*[this->mSoundFXCap];
	for(unsigned int i = 0; i < this->mSoundFXCap; i++)
	{
		this->mSoundFX[i] = NULL;
	}
	this->mSoundFXChannel2D = NULL;

	//songs
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
	for(int i = 0; i < this->mNrOfSoundFX; i++)
	{
		 if(this->mSoundFX[i])
		 {
			 ERRCHECK(this->mResult = this->mSoundFX[i]->release());
		 }
		 this->mSoundFX[i] = NULL;
	}
	SAFE_DELETE_ARRAY(this->mSoundFX);
	//do nothing with channel

	//songs
	for(int i = 0; i < this->mNrOfSongs; i++)
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

	return 1;
}


void SoundEngine::LoadSoundEffect(string filename)
{
	ERRCHECK(this->mResult = this->mSystem->createSound(filename.c_str(), FMOD_HARDWARE | FMOD_2D | FMOD_LOOP_OFF, NULL, &this->mSoundFX[this->mNrOfSoundFX++]));
}
void SoundEngine::LoadSong(string filename)
{
	ERRCHECK(this->mResult = this->mSystem->createStream(filename.c_str(), FMOD_HARDWARE | FMOD_2D, NULL, &this->mSongs[this->mNrOfSongs++]));
}


void SoundEngine::PlaySoundEffect(unsigned int index)
{
	if(index < this->mNrOfSoundFX)
	{
		ERRCHECK(this->mResult = this->mSystem->playSound(FMOD_CHANNEL_FREE, this->mSoundFX[index], false, &this->mSoundFXChannel2D));
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
		}
	}
	else
	{
		MaloW::Debug("SoundEngine: Warning: PlaySong(): Index out of bounds");
	}
}


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
void SoundEngine::ResumeSong(unsigned int index)
{
	if(index < this->mNrOfSongs)
	{
		FMOD::Sound* currentSound;
		this->mSongChannel->getCurrentSound(&currentSound);

		if(currentSound == this->mSongs[index])
		{
			ERRCHECK(this->mResult = this->mSystem->playSound(FMOD_CHANNEL_REUSE, this->mSongs[index], false, &this->mSongChannel));
		}
	}
	else
	{
		MaloW::Debug("SoundEngine: Warning: ResumtSong(): Index out of bounds");
	}
}