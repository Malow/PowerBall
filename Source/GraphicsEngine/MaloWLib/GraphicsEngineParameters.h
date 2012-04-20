#ifndef MALOWENGINEPARAMS
#define MALOWENGINEPARAMS

#include "KeyListener.h"

enum CameraType
{
	FPS = 0,
	RTS = 1,
};

class GraphicsEngineParams
{
public:
	int windowWidth;
	int windowHeight;
	int ShadowMapSettings;
	int FXAAQuality;
	MaloW::KeyListener* kl;
	CameraType CamType;

	GraphicsEngineParams()
	{
		this->windowWidth = 1024;
		this->windowHeight = 768;
		this->ShadowMapSettings = 0;
		this->FXAAQuality = 0;
		this->kl = NULL;
		this->CamType = FPS;
	}

	virtual ~GraphicsEngineParams() { }
};

#endif