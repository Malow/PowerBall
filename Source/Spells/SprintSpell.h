#pragma once

#include "..\Spells\Spell.h"


class SprintSpell : public Spell
{
private:
	backup mBackup;
public:
	SprintSpell();
	SprintSpell(Ball* ball);
	virtual ~SprintSpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { }
};