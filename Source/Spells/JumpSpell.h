#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 11/05/2012
* 
* 
**/
#include "..\Spells\Spell.h"


class JumpSpell : public Spell
{
private:
	backup mBackup;
	Vector3 mJumpPosition;

public:
	JumpSpell();
	JumpSpell(PowerBall* ball);
	virtual ~JumpSpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { }
};