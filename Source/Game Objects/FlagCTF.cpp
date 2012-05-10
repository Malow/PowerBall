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