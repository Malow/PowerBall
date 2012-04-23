#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 05/04/2012
* 
* Flag class because I needed some extra variables. Like AtBase and maybe some more later.
**/

#include "GraphicsEngine\GraphicsEngine.h"

class Flag
{
private:
	StaticMesh* mMesh;
	bool mAtBase;
	D3DXVECTOR3 mStartPosition;
public:
	Flag();
	Flag(StaticMesh* mesh, D3DXVECTOR3 startPosition);
	~Flag();

	void SetAtBase(bool atBase){ this->mAtBase = atBase; }
	bool GetAtBase(){ return this->mAtBase; }

	void SetStartPosition(D3DXVECTOR3 startPosition){ this->mStartPosition = startPosition; }
	D3DXVECTOR3 GetStartPosition(){ return this->mStartPosition; }

	void Reset();

	void SetMesh(StaticMesh* mesh){ this->mMesh = mesh; }
	StaticMesh* GetMesh(){ return this->mMesh; }
};