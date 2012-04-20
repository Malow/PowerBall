#include "MorphAnimation.h"

//private
void MorphAnimation::Expand()
{
	this->mCapacity *= this->mCapacity;
	Mesh** tmp = new Mesh*[this->mCapacity];
	for(UINT i = 0; i < this->mNrOfKeyFrames; i++)
	{
		tmp[i] = this->mKeyFrames[i];
	}
	SAFE_DELETE_ARRAY(this->mKeyFrames);
	this->mKeyFrames = tmp;
	tmp = NULL;
}

//public
MorphAnimation::MorphAnimation()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;
		
	this->mSpeedMultiplier = 1.0f;	
	this->mTime = 0.0f;				
	this->mNrOfKeyFrames = 0;
	this->mCapacity = 2;
	this->mKeyFrames = new Mesh*[this->mCapacity];
	for(UINT i = 0; i < this->mCapacity; i++)
	{
		this->mKeyFrames[i] = NULL;
	}
	this->mInterpolatedMesh = NULL;
}
MorphAnimation::~MorphAnimation()
{
	for(UINT i = 0; i < this->mNrOfKeyFrames; i++)
	{
		SAFE_DELETE(this->mKeyFrames[i]);
	}
	SAFE_DELETE_ARRAY(this->mKeyFrames);
	SAFE_DELETE(this->mInterpolatedMesh);
}

//get
float MorphAnimation::GetSpeedMultiplier() const
{
	return this->mSpeedMultiplier;
}
float MorphAnimation::GetTime() const
{
	return this->mTime;
}			
UINT MorphAnimation::GetNrOfKeyFrames() const
{
	return this->mNrOfKeyFrames;
}
UINT MorphAnimation::GetCapacity() const
{
	return this->mCapacity;
}
Mesh* MorphAnimation::GetInterpolatedMesh()
{
	//pseudocode:**
	//work out animation step from time
	//get previous and next key frames
	//get vertices from key frames
	//interpolate to get current mesh
	//	use previous & next key frame
	//	transform time along the interval between previous & next key frame to range [0,1] as weight w
	//	linear interpolation: lerp(k0.positions, k1.positions, w);
	//		 x + s * (y - x)
	//draw this mesh (note: new world pos)

	
	//compute the indices for the keyframes to interpolate
	UINT firstIndex = (int)((this->mNrOfKeyFrames - 1) * this->mTime);

	//get previous and next keyframes (vertex buffers)
	UINT size = (UINT)this->mKeyFrames[firstIndex]->GetStrips()->size();
	for(UINT i = 0; i < size; i++)
	{
		//**
		this->mKeyFrames[firstIndex]->GetStrips()->get(i)->getVerts();
	}
}

//set
void MorphAnimation::SetSpeedMultiplier(float speedMultiplier)
{
	this->mSpeedMultiplier = speedMultiplier;
}

//other
void MorphAnimation::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->gDevice = device;
	this->gDeviceContext = deviceContext;
}
void MorphAnimation::Update(float deltaTime)
{
	this->mTime += deltaTime * 0.001f * this->mSpeedMultiplier; //seconds**
}


