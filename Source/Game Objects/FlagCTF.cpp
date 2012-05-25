#include "FlagCTF.h"

FlagCTF::FlagCTF()
{
	this->mAtBase = true;
	this->mMesh = NULL;
}
FlagCTF::FlagCTF(StaticMesh* mesh, D3DXVECTOR3 startPosition)
{
	this->mAtBase = true;
	this->mMesh = mesh;
	this->mStartPosition = startPosition;
}
FlagCTF::~FlagCTF()
{

}

void FlagCTF::SetToStartPosition()
{
	this->SetPosition(this->mStartPosition);
}

void FlagCTF::Reset()
{
	this->mMesh->SetPosition(this->mStartPosition);
	this->mAtBase = true;
}
/*
#include "FlagCTF.h"

FlagCTF::FlagCTF()
{
	this->mAtBase = true;
	this->mStartPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->mAnimatedMesh = NULL;
}

FlagCTF::FlagCTF(AnimatedMesh* mesh, D3DXVECTOR3 startPosition)
{
	this->mAtBase = true;
	this->mStartPosition = startPosition;
	this->mAnimatedMesh = mesh;
}
FlagCTF::~FlagCTF()
{
	
}

void FlagCTF::SetToStartPosition()
{
	this->mAnimatedMesh->SetPosition(this->mStartPosition);
}

void FlagCTF::Reset()
{
	this->mAnimatedMesh->SetPosition(this->mStartPosition);
	this->mAtBase = true;
}*/