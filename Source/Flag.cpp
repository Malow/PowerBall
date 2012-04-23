#include "Flag.h"

Flag::Flag()
{
	this->mAtBase = true;
	this->mMesh = NULL;
}
Flag::Flag(Mesh* mesh, D3DXVECTOR3 startPosition)
{
	this->mAtBase = true;
	this->mMesh = mesh;
	this->mStartPosition = startPosition;
}
Flag::~Flag()
{

}

void Flag::Reset()
{
	this->mMesh->SetPosition(this->mStartPosition);
}