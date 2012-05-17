#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 03/05/2012
* 
* 
**/
#include "..\Spells\Spell.h"
#include "..\GraphicsEngine\StaticMesh.h"

class HardenSpell : public Spell
{
private:
	backup mBackup;
	StaticMesh* mHardenMesh;
public:
	HardenSpell();
	//HardenSpell(Ball* ball);
	HardenSpell(PowerBall* ball);
	virtual ~HardenSpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { }

};