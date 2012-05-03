#pragma once
#include "..\Spells\Spell.h"


class HardenSpell : public Spell
{
private:
	backup mBackup;
public:
	HardenSpell();
	virtual ~HardenSpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { }
};