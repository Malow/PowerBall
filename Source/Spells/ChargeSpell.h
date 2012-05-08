#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 03/05/2012
* 
* 
**/
#include "..\Spells\Spell.h"


class ChargeSpell : public Spell
{
private:
	backup mBackup;
public:
	ChargeSpell();
	virtual ~ChargeSpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { this->Restore();}

};