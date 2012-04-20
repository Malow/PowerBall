#include "RTSCamera.h"

RTSCamera::RTSCamera(HWND g_hWnd, GraphicsEngineParams params) : Camera(g_hWnd, params)
{
	this->up = D3DXVECTOR3(0, 1, 0);
	this->DistanceFromTarget = 75.0f;
	this->forward = D3DXVECTOR3(0, -2, 1);
	this->forward = this->NormalizeVector(this->forward);

	CursorControl cc;
	cc.SetVisibility(true);
}

RTSCamera::~RTSCamera()
{
	
}

void RTSCamera::updateSpecific(float delta)
{
	POINT p;
	if(GetCursorPos(&p))
	{
		if(ScreenToClient(this->g_hWnd, &p))
		{
			/*
			float diffX = (this->params.windowWidth/2) - p.x;
			float diffY = (this->params.windowHeight/2) - p.y;

			
			this->angleX += diffX * (this->sensitivity * 0.001f);
			this->angleY += diffY * (this->sensitivity * 0.001f);

			if(angleY > PI/2)
				angleY = PI/2;
			if(angleY < -PI/2)
				angleY = -PI/2;

			this->forward.x = cos(angleX);
			this->forward.z = sin(angleX);
			this->forward.y = sin(angleY);

			
			float length = sqrt(pow(this->forward.x, 2) + pow(this->forward.y, 2) + pow(this->forward.z, 2));
			this->forward.x /= length;
			this->forward.y /= length;
			this->forward.z /= length;
			
			
			POINT np;
			np.x = this->params.windowWidth/2;
			np.y = this->params.windowHeight/2;
			if(ClientToScreen(this->g_hWnd, &np))
			{
				SetCursorPos(np.x, np.y);
			}
			*/
		}
	}
}

void RTSCamera::moveForward(float diff)
{
	this->pos.z += ((float)diff/100) * this->speed;
}

void RTSCamera::moveBackward(float diff)
{
	this->pos.z -= ((float)diff/100) * this->speed;
}

void RTSCamera::moveLeft(float diff)
{
	this->pos.x -= ((float)diff/100) * this->speed;
}

void RTSCamera::moveRight(float diff)
{
	this->pos.x += ((float)diff/100) * this->speed;
}