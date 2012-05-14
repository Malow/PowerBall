#include "Spell.h"
//#include "Ball.h"
#include "..\Game Objects\PowerBall.h"

Spell::Spell()
{
	//this->mBall = NULL;
	this->mPowerBall = NULL;
	this->mIsInUse = false;
	this->mNeedCoolDown = false;
	this->mCharging = false;
	this->mKeyUp = true;
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

void Spell::KeyUp()
{
	this->mKeyUp = true;
}

void Spell::Ready()
{
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
}

bool Spell::SpellInUse()
{
	bool usingSpell = false;
	Spell** spells = this->mPowerBall->GetSpells();
	for(int i = 0; i<this->mPowerBall->GetNrOfSpells(); i++)
		if(spells[i]->mIsInUse || spells[i]->mCharging )
			usingSpell = true;
	return usingSpell;
}