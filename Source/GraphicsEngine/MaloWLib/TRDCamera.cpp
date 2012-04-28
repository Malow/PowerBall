#include "TRDCamera.h"
#include "Vector.h"
using namespace MaloW;
#include "..\..\Ball.h"
TRDCamera::TRDCamera(HWND g_hWnd, GraphicsEngineParams params) : Camera(g_hWnd, params)
{
	this->up = D3DXVECTOR3(0, 1, 0);
	this->DistanceFromTarget = 75.0f;
	this->forward = D3DXVECTOR3(0, -2, 1);
	this->forward = this->NormalizeVector(this->forward);
	//this->mFollowThisMeshPosition = NULL;
	//this->mBallForwardVector = NULL;
	this->mTargetVector = D3DXVECTOR3(0, 0, 0);
	this->mBallToFollow = NULL;
	//this->pos = D3DXVECTOR3(0, 0, 0);
	this->mIsClicked = false;
	CursorControl cc;
	cc.SetVisibility(false);
}

TRDCamera::~TRDCamera()
{
	
}

void TRDCamera::updateSpecific(float delta)
{
	if(this->mBallToFollow)
	{
		D3DXVECTOR3 forwardBall = this->mBallToFollow->GetForwardVector().GetD3DVec();
		this->mTargetVector = (D3DXVECTOR3(0,-1,0) + forwardBall)*this->mBallToFollow->GetDistanceToCam();
		this->pos = this->mBallToFollow->GetPosition() - this->mTargetVector;
		this->forward = this->NormalizeVector(this->mTargetVector);
		
		/*
		D3DXVECTOR3 temp = *this->mBallForwardVector;
		this->mTargetVector = D3DXVECTOR3(0,-5,0) + temp*5;
		this->pos = *this->mFollowThisMeshPosition - this->mTargetVector;
		this->forward = this->NormalizeVector(this->mTargetVector);
		*/
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(this->g_hWnd, &p);
		int dx = 0;
		int dy = 0;
		if(GetGraphicsEngine()->GetKeyListener()->IsClicked(1))
		{
			if(!this->mIsClicked)
			{
				this->mIsClicked = true;
				this->mOldPos = p;
			}
			else
			{
				dx = p.x - this->mOldPos.x;
				dy = p.y - this->mOldPos.y;
				this->mOldPos = p;
				this->mBallToFollow->RotateRight(dx);
			}
			
		}
		else
		{
			if(this->mIsClicked)
			{
				this->mIsClicked = false;
				POINT newPos = p;
				dx = newPos.x - this->mOldPos.x;
				dy = newPos.y - this->mOldPos.y;
			}
		
		}
	}
	
}

void TRDCamera::moveForward(float diff)
{
	
}

void TRDCamera::moveBackward(float diff)
{
	
}

void TRDCamera::moveLeft(float diff)
{
	
}

void TRDCamera::moveRight(float diff)
{
	
}

void TRDCamera::setBallToFollow(Ball* ball)
{
	this->mBallToFollow = ball;
}

Ball* TRDCamera::getBallToFollow()
{
	 return this->mBallToFollow;
}