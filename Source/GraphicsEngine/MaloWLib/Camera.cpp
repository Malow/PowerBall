#include "Camera.h"

Camera::Camera(HWND g_hWnd, GraphicsEngineParams params)
{
	this->forceBoundries = false;
	this->g_hWnd = g_hWnd;
	this->params = params;
	this->pos = D3DXVECTOR3(0, 0, 0);
	this->terrain = NULL;
	this->followTarget = NULL;
	
	this->angleX = 0;
	this->angleY = 0;
	
	this->speed = 1.0f;
	this->sensitivity = 1.0f;

	D3DXMatrixPerspectiveFovLH(&this->projection, (float)D3DX_PI * 0.45f, this->params.windowWidth / (float)this->params.windowHeight, 1.0f, 200.0f);
}

Camera::~Camera()
{

}

D3DXMATRIX Camera::GetViewMatrix()
{
	D3DXVECTOR3 at = this->pos + this->forward;
	D3DXMatrixLookAtLH(&view, &this->pos, &at, &this->up);
	return this->view; 
}

D3DXMATRIX Camera::GetProjectionMatrix()
{
	return this->projection; 
}

D3DXVECTOR3 Camera::getPosition()
{
	return this->pos;
}

float Camera::getPosX()
{
	return this->pos.x;
}

float Camera::getPosY()
{
	return this->pos.y;
}

float Camera::getPosZ()
{
	return this->pos.z;
}

void Camera::setPosition(D3DXVECTOR3 pos)
{
	this->pos = pos;
}

void Camera::setPosX(float x)
{
	this->pos.x = x;
}

void Camera::setPosY(float y)
{
	this->pos.y = y;
}

void Camera::setPosZ(float z)
{
	this->pos.z = z;
}


D3DXVECTOR3 Camera::getForward()
{
	return this->forward;
}

void Camera::setForward(D3DXVECTOR3 forward)
{
	this->forward = forward;
	this->forward = this->NormalizeVector(this->forward);
}

void Camera::LookAt(D3DXVECTOR3 target)
{
	this->forward = target - this->pos;
	this->forward = this->NormalizeVector(this->forward);

	this->angleX = -atan2(this->forward.x*0 - 1*this->forward.z, this->forward.x * 1 + this->forward.z * 0);
	this->angleY = asin(this->forward.y);
}

D3DXVECTOR3 Camera::getUpVector()
{
	return this->up;
}

void Camera::setUpVector(D3DXVECTOR3 up)
{
	this->up = up;
}

void Camera::moveToTerrain()
{
	if(this->terrain)
		this->pos.y = this->terrain->getYPositionAt(this->pos.x, this->pos.z) + this->DistanceFromTarget;
}

void Camera::move(D3DXVECTOR3 moveBy)
{
	this->pos += moveBy;
	this->moveToTerrain();
}

D3DXVECTOR3 Camera::NormalizeVector(D3DXVECTOR3 vec)
{
	float length = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
	vec.x /= length;
	vec.y /= length;
	vec.z /= length;
	return vec;
}

void Camera::MoveToFollowPosition()
{
	if(this->followTarget)
	{
		this->pos = this->followTarget->GetPosition() - D3DXVECTOR3(0, -15, this->DistanceFromTarget);

	}
}

void Camera::update(float delta)
{
	if(GetForegroundWindow() == this->g_hWnd)
		updateSpecific(delta);
	
	this->moveToTerrain();
	this->MoveToFollowPosition();
	if(this->forceBoundries)
	{
		if(this->pos.x < this->minBoundries.x)
			this->pos.x = this->minBoundries.x;
		else if(this->pos.x > this->maxBoundries.x)
			this->pos.x = this->maxBoundries.x;

		if(this->pos.y < this->minBoundries.y)
			this->pos.y = this->minBoundries.y;
		else if(this->pos.y > this->maxBoundries.y)
			this->pos.y = this->maxBoundries.y;

		if(this->pos.z < this->minBoundries.z)
			this->pos.z = this->minBoundries.z;
		else if(this->pos.z > this->maxBoundries.z)
			this->pos.z = this->maxBoundries.z;
	}
}

void Camera::SetBoundries(D3DXVECTOR3 minBoundries, D3DXVECTOR3 maxBoundries)
{
	this->minBoundries = minBoundries;
	this->maxBoundries = maxBoundries;
	this->forceBoundries = true;
}

void Camera::DisableBoundries()
{
	this->forceBoundries = false;
}