#ifndef TRDCAMERA
#define TRDCAMERA

#include "Camera.h"

class Ball;
class PowerBall;
class TRDCamera : public Camera
{
private: 
		D3DXVECTOR3 mTargetVector;
		Ball* mBallToFollow;
		PowerBall* mPowerBallToFollow;
		bool mIsClicked;
		POINT mOldPos;
public:
	TRDCamera(HWND g_hWnd, GraphicsEngineParams params);
	virtual ~TRDCamera();

	virtual void updateSpecific(float delta);

	virtual void moveForward(float diff);
	virtual void moveBackward(float diff);
	virtual void moveLeft(float diff);
	virtual void moveRight(float diff);

	void setBallToFollow(Ball* ball);
	Ball* getBallToFollow();
	void setPowerBallToFollow(PowerBall* ball);
	PowerBall* getPowerBallToFollow();
	void removeFollowBall() { this->mBallToFollow = NULL; this->mPowerBallToFollow = NULL;}
	void calculateNewUp();
};


#endif