#ifndef FPSCAMERA
#define FPSCAMERA

#include "Camera.h"

class FPSCamera : public Camera
{
public:
	FPSCamera(HWND g_hWnd, GraphicsEngineParams params);
	virtual ~FPSCamera();

	virtual void updateSpecific(float delta);

	virtual void moveForward(float diff);
	virtual void moveBackward(float diff);
	virtual void moveLeft(float diff);
	virtual void moveRight(float diff);
};


#endif