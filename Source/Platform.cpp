#include "Platform.h"

Platform::Platform(const string meshFilePath)
{
	//this->mMesh		= Engine.GetInstance()->CreateObject(meshFilePath);
	this->mShrink		= 0.0f;
	//float		mRotation;
	//float		mWobblynessiiness;
}

Platform::Platform(const string meshFilePath, float shrinkValue)
{
	//this->mMesh		= Engine.GetInstance()->CreateObject(meshFilePath);
	this->mShrink		= shrinkValue;
	//float		mRotation;
	//float		mWobblynessiiness;
}

Platform::~Platform()
{
	//Engine.GetInstance()->DeleteObject(this->mMesh);
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

void Platform::Update(const float dt)
{
	dt;
	//Engine.GetInstance()->ScaleObject(this->mMesh, this->mshrink*dt);
}
