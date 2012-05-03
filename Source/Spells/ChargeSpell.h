#pragma once
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