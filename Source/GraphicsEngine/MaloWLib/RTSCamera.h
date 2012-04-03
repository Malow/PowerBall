#ifndef RTSCAMERA
#define RTSCAMERA

#include "Camera.h"

class RTSCamera : public Camera
{
public:
	RTSCamera(HWND g_hWnd, GraphicsEngineParams params);
	virtual ~RTSCamera();

	virtual void updateSpecific(float delta);

	virtual void moveForward(float diff);
	virtual void moveBackward(float diff);
	virtual void moveLeft(float diff);
	virtual void moveRight(float diff);
};


#endif