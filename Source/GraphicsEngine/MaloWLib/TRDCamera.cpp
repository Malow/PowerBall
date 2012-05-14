#include "TRDCamera.h"
#include "Vector.h"
using namespace MaloW;
//#include "..\..\Ball.h"
#include "..\..\Game Objects\PowerBall.h"

TRDCamera::TRDCamera(HWND g_hWnd, GraphicsEngineParams params) : Camera(g_hWnd, params)
{
	this->up = D3DXVECTOR3(0, 1, 0);	// dummy vector, will calculate a new up with gram-schmidt orthogonalization process
	this->forward = D3DXVECTOR3(0, 0, 1);	// dummy vector, will be set when a ball is assigned 
	this->forward = this->NormalizeVector(this->forward);
	this->mTargetVector = D3DXVECTOR3(0, 0, 0);
	this->mOldDistanceBall = D3DXVECTOR3(0, 0, 0);
	//this->mBallToFollow = NULL;
	this->mPowerBallToFollow = NULL;
	this->mIsClicked = false;
	this->mDisablePositionChanges = false;
}

TRDCamera::~TRDCamera()
{
	
}

void TRDCamera::updateSpecific(float delta)
{/*
	if(this->mBallToFollow)
	{
		D3DXVECTOR3 forwardBall = this->mBallToFollow->GetForwardVector().GetD3DVec();
		this->mTargetVector = (D3DXVECTOR3(0,-1,0) + forwardBall)*this->mBallToFollow->GetDistanceToCam();
		this->pos = this->mBallToFollow->GetPosition() - this->mTargetVector;
		this->forward = this->NormalizeVector(this->mTargetVector);
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
				
				/* add this line if you not want to keep the mouse position after click 
				// this->mOldPos = p;
				
				this->mBallToFollow->RotateForwardRight(dx);
				/* remove the 3 lines below if you not want to keep the mouse position after click 
				POINT newP = this->mOldPos;
				ClientToScreen(this->g_hWnd, &newP);
				SetCursorPos(newP.x, newP.y);
				
			}
			
		}
		else
		{
			if(this->mIsClicked)
				this->mIsClicked = false;
		}
		/* remove 2 lines below if you want to show the mouse pointer after a click 
		CursorControl cc;
		cc.SetVisibility(!this->mIsClicked);
		
	}
	*/
	if(this->mPowerBallToFollow)
	{
		if(!this->mDisablePositionChanges)
		{
			D3DXVECTOR3 forwardBall = this->mPowerBallToFollow->GetForwardVector().GetD3DVec();
			this->mTargetVector = (D3DXVECTOR3(0,-1,0) + forwardBall)*this->mPowerBallToFollow->GetDistanceToCam();
			this->pos = this->mPowerBallToFollow->GetPosition() - this->mTargetVector;
			this->forward = this->NormalizeVector(this->mTargetVector);
			this->mOldDistanceBall = this->mPowerBallToFollow->GetPosition();
		}
		else
		{
			D3DXVECTOR3 forwardBall = this->mPowerBallToFollow->GetForwardVector().GetD3DVec();
			this->mTargetVector = (D3DXVECTOR3(0,-1,0) + forwardBall)*this->mPowerBallToFollow->GetDistanceToCam();
			this->pos = this->mOldDistanceBall - this->mTargetVector;
			this->forward = this->NormalizeVector(this->mTargetVector);
		}
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
				
				/* add this line if you not want to keep the mouse position after click */
				// this->mOldPos = p;
				
				this->mPowerBallToFollow->RotateForwardRight(dx);
				/* remove the 3 lines below if you not want to keep the mouse position after click */
				POINT newP = this->mOldPos;
				ClientToScreen(this->g_hWnd, &newP);
				SetCursorPos(newP.x, newP.y);
				
			}
			
		}
		else
		{
			if(this->mIsClicked)
				this->mIsClicked = false;
		}
		/* remove 2 lines below if you want to show the mouse pointer after a click */
		CursorControl cc;
		cc.SetVisibility(!this->mIsClicked);
		
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

//void TRDCamera::setBallToFollow(Ball* ball)
//{
//	this->mBallToFollow = ball;
//}

void TRDCamera::setPowerBallToFollow(PowerBall* ball)
{
	this->mPowerBallToFollow = ball;
}

//Ball* TRDCamera::getBallToFollow()
//{
	// return this->mBallToFollow;
//}

PowerBall* TRDCamera::getPowerBallToFollow()
{
	return this->mPowerBallToFollow;
}
void TRDCamera::calculateNewUp()
{
	/* Gram-Schmidt process */
	
	/* Start vectors */
	D3DXVECTOR3 v1 = this->NormalizeVector(this->forward);
	D3DXVECTOR3 v2 = this->NormalizeVector(this->up);
	
	/* Process */
	D3DXVECTOR3 w1 = v1;
	D3DXVECTOR3 w2 = v2 - D3DXVec3Dot(&v1,&v2)*v2;
	
	/* Some of the above we don't really need to calc, but to be clear how the process work its shown. */
	this->forward = w1;
	this->up = this->NormalizeVector(w2);
	D3DXVECTOR3 left;
	D3DXVec3Cross(&left, &this->up,&this->forward);
	
}

void TRDCamera::disablePositionChanges()
{
	this->mDisablePositionChanges = true;
}

void TRDCamera::enablePositionChanges()
{
	this->mDisablePositionChanges = false;
}