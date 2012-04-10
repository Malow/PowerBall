#include "Platform.h"

Platform::Platform(const string meshFilePath, D3DXVECTOR3 position)
{
	this->mMesh		     = GetGraphicsEngine()->CreateMesh(meshFilePath, position); //D3DXVECTOR3(10, 10, 10)
	this->mShrink	     = 0.01f;
	this->mRadius	     = 10.0f;
	this->mScaledRadius  = this->mRadius;
	
}
Platform::~Platform()
{
	GetGraphicsEngine()->DeleteMesh(this->mMesh);
}

float Platform::GetY(const float x, const float z) const
{
	x;
	z;
	//either get vertex height from engine
	//or calc the distance of x,z to the center of platform and compare it to radius of platform. (this requires to have a specified Y-value of the platform)


	return -1.0f;
}

float Platform::GetYInterpolated(const float x, const float z) const
{
	x;
	z;
	//perform bilinear interpolation (this function is only needed if the platform surface have height differences.
	return -1.0f;
}
Vector3 Platform::GetPositionXZ() const
{
	D3DXVECTOR3 temp = this->mMesh->GetPosition();
	Vector3 temp2 = Vector3(temp.x,0,temp.z);
	return temp2;
}
void Platform::Update(const float dt)
{
	
	float fraction = 1.0f-this->mShrink*dt;
	this->mScaledRadius *= fraction;//this->mScaledRadius/this->mRadius;
	this->mMesh->Scale(D3DXVECTOR3(fraction,1,fraction));
}
bool Platform::IsOnPlatform(const float x, const float z) const
{
	D3DXVECTOR3 pos = this->mMesh->GetPosition();

	//calculate the distance from the XZ-center of the platform to the XZ-position
	float distance = sqrt(pow(pos.x - x, 2) + pow(pos.z - z, 2));

	bool isOn = false;
	if(distance < this->mScaledRadius)
		isOn = true;

	return isOn;
}
