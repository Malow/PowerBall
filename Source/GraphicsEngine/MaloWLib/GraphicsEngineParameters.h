#ifndef MALOWENGINEPARAMS
#define MALOWENGINEPARAMS

#include "KeyListener.h"

enum CameraType
{
	FPS = 0,
	RTS = 1,
	TRD = 2,
};

class GraphicsEngineParams
{
public:
	static int windowWidth;
	static int windowHeight;
	static int ShadowMapSettings;
	static int FXAAQuality;
	static MaloW::KeyListener* kl;
	static CameraType CamType;

	GraphicsEngineParams()
	{

	}

	virtual ~GraphicsEngineParams() { }
};

#endif