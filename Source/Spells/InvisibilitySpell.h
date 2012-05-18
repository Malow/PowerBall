#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 03/05/2012
* 
* 
**/
#include "..\Spells\Spell.h"


class InvisibilitySpell : public Spell
{
private:
	backup mBackup;
public:
	InvisibilitySpell();
//	InvisibilitySpell(Ball* ball);
	InvisibilitySpell(PowerBall* ball);
	InvisibilitySpell(SoundEffect* soundEffect);
	virtual ~InvisibilitySpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { this->Restore();}
};