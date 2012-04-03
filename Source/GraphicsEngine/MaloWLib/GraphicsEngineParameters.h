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
	MaloW::KeyListener* kl;
	CameraType CamType;

	GraphicsEngineParams()
	{
		this->windowWidth = 1024;
		this->windowHeight = 768;
		this->kl = NULL;
		this->CamType = FPS;
	}

	virtual ~GraphicsEngineParams() { }
};

#endif