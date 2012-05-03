#include "InvisibilitySpell.h"
#include "Ball.h"

InvisibilitySpell::InvisibilitySpell()
{
	this->mBall = NULL;
	this->mTimeNeededToCoolDown = 30.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 5.0f;
	this->mIsInUse = false;
}

InvisibilitySpell::~InvisibilitySpell()
{
	
}

void InvisibilitySpell::UpdateSpecial(float dt)
{
	if(this->mTimerCounterInUse > this->mMaxTimeUse)
		this->Restore();
	if(this->mTimerCounterCoolDown > this->mTimeNeededToCoolDown)
		Spell::Ready();
	Spell::Update(dt);
}

void InvisibilitySpell::Use()
{
	if(!this->mNeedCoolDown && !this->mIsInUse && !this->SpellInUse())
	{
		/* switch mesh here to an almost transparent mesh */
		
		
		/* backup ball info */
		//this->mBackup.e = this->mBall->GetRestitution();
		this->mBackup.mass = this->mBall->GetMass();
		
		/* new behaviour for ball */
		//this->mBall->SetRestitution(0.5f);
		this->mBall->SetMass(this->mBall->GetMass());

		this->mIsInUse = true;
	}

}
void InvisibilitySpell::Restore()
{
	if(this->mIsInUse)
	{
		//this->mBall->SetRestitution(this->mBackup.e);
		this->mBall->SetMass(mBackup.mass);
		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}