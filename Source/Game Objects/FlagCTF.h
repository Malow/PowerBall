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



/*#pragma once
* Outdated:
* Auther: Rikard Johansson
* Create Date: 05/04/2012
* 
* Flag class because I needed some extra variables. Like AtBase and maybe some more later.
*
* inheritance added by Jerry Rahmqvist 09/05/2012.
**/
/*
#include "stdafx.h"
#include "AnimatedMesh.h"

class FlagCTF
{
	private:
		bool			mAtBase;
		D3DXVECTOR3		mStartPosition;
		AnimatedMesh*	mAnimatedMesh;

	public:
		FlagCTF();
		FlagCTF(AnimatedMesh* mesh, D3DXVECTOR3 startPosition);
		virtual ~FlagCTF();
		
		bool			GetAtBase()	const	{ return this->mAtBase; }
		AnimatedMesh*	GetMesh() const		{ return this->mAnimatedMesh; }
		
		void SetAtBase(bool atBase){ this->mAtBase = atBase; }
		void SetPosition(D3DXVECTOR3 position) { this->mAnimatedMesh->SetPosition(position); }

		/*! Sets the game object to its startposition (at base). 
		void Reset();

	
};
*/