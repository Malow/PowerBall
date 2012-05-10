#include "Spell.h"
#include "Ball.h"

Spell::Spell()
{
	this->mBall = NULL;
	this->mPowerBall = NULL;
	this->mIsInUse = false;
	this->mNeedCoolDown = false;
}

Spell::~Spell()
{
	
}

void Spell::Update(float dt)
{
	if(this->mIsInUse)
		this->mTimerCounterInUse += dt;
	if(this->mNeedCoolDown)
		this->mTimerCounterCoolDown += dt;	
}

void Spell::Ready()
{
	this->mTimeNeededToCoolDown = 0.0f;
	this->mNeedCoolDown = false;
}

bool Spell::SpellInUse()
{
	bool usingSpell = false;
	Spell** spells = this->mBall->GetSpells();
	for(int i = 0; i<this->mBall->GetNrOfSpells(); i++)
		if(spells[i]->mIsInUse)
			usingSpell = true;
	return usingSpell;
}