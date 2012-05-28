#pragma once
/* 
* Auther: Rikard Johansson
* Create Date: 05/04/2012
* 
* Flag class because I needed some extra variables. Like AtBase and maybe some more later.
*
* inheritance added by Jerry Rahmqvist 09/05/2012.
* 
* inheritance removed and staticmesh swapped with animatedmesh and other minor changes by Markus Tillman 26/05/2012.
**/

#include "stdafx.h"
#include "AnimatedMesh.h"

class FlagCTF
{
	private:
		float			mFlagRadius;
		bool			mAtBase;
		D3DXVECTOR3		mStartPosition;
		AnimatedMesh*	mAnimatedMesh;

	public:
		FlagCTF();
		FlagCTF(AnimatedMesh* mesh, D3DXVECTOR3 startPosition);
		virtual ~FlagCTF();
		
		float			GetFlagRadius() const	{ return this->mFlagRadius; }
		bool			GetAtBase()	const		{ return this->mAtBase; }
		AnimatedMesh*	GetMesh() const			{ return this->mAnimatedMesh; }
		
		void SetFlagRadius(float radius)		{ this->mFlagRadius = radius; }
		void SetAtBase(bool atBase)				{ this->mAtBase = atBase; }
		void SetPosition(D3DXVECTOR3 position)	{ this->mAnimatedMesh->SetPosition(position); }

		/*! Sets the game object to its startposition (at base). */
		void Reset();

	
};
