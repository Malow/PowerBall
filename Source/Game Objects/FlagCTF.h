#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 05/04/2012
* 
* Flag class because I needed some extra variables. Like AtBase and maybe some more later.
*
* inheritance added by Jerry Rahmqvist 09/05/2012.
**/

#include "..\Game Objects\GameObject.h"


class FlagCTF : public GameObject
{
private:
	bool mAtBase;
	D3DXVECTOR3 mStartPosition;
public:
	FlagCTF();
	FlagCTF(StaticMesh* mesh, D3DXVECTOR3 startPosition);
	virtual ~FlagCTF();
	/*! Sets the game object to its startposition. */
	virtual void SetToStartPosition();
	void SetAtBase(bool atBase){ this->mAtBase = atBase; }
	bool GetAtBase(){ return this->mAtBase; }
	
	void Reset();

	
};