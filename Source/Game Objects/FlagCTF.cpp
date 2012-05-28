#include "FlagCTF.h"

FlagCTF::FlagCTF()
{
	this->mFlagRadius = 0.0f;
	this->mAtBase = true;
	this->mStartPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->mAnimatedMesh = NULL;
}

FlagCTF::FlagCTF(AnimatedMesh* mesh, D3DXVECTOR3 startPosition)
{
	this->mFlagRadius = 1.0f;
	this->mAtBase = true;
	this->mStartPosition = startPosition;
	this->mAnimatedMesh = mesh;
}
FlagCTF::~FlagCTF()
{
	//SAFE_DELETE(this->mAnimatedMesh); //handled by CaptureTheFlag.cpp
}

void FlagCTF::Reset()
{
	this->mAnimatedMesh->SetPosition(this->mStartPosition);
	this->mAtBase = true;
}