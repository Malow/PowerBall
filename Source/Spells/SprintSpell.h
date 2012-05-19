#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 03/05/2012
* 
* 
**/
#include "..\Spells\Spell.h"


class SprintSpell : public Spell
{
private:
	backup mBackup;
public:
	SprintSpell();
	//SprintSpell(Ball* ball);
	SprintSpell(PowerBall* ball);
	SprintSpell(SoundEffect* soundEffect);
	virtual ~SprintSpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { }
};