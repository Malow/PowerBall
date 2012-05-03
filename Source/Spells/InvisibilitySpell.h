#pragma once
#include "..\Spells\Spell.h"


class InvisibilitySpell : public Spell
{
private:
	backup mBackup;
public:
	InvisibilitySpell();
	virtual ~InvisibilitySpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { this->Restore();}
};