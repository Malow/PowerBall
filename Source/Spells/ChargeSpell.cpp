#include "ChargeSpell.h"
#include "Ball.h"

ChargeSpell::ChargeSpell()
{
	this->mBall = NULL;
	this->mTimeNeededToCoolDown = 15.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 1.0f;
	this->mIsInUse = false;
}

ChargeSpell::~ChargeSpell()
{
	
}

void ChargeSpell::UpdateSpecial(float dt)
{
	if(this->mTimerCounterInUse > this->mMaxTimeUse)
		this->Restore();
	if(this->mTimerCounterCoolDown > this->mTimeNeededToCoolDown)
		Spell::Ready();
	Spell::Update(dt);
}

void ChargeSpell::Use()
{
	if(!this->mNeedCoolDown && !this->mIsInUse && !this->SpellInUse())
	{
		/* backup ball info */
		this->mBackup.maxSpeed = this->mBall->GetMaxVelocity();
		
		/* new behaviour for ball */
		this->mBall->SetMaxVelocity(this->mBall->GetMaxVelocity()*5.0f);
		this->mBall->SetSteering(false);
		this->mBall->SetVelocity(this->mBall->GetForwardVector()*this->mBall->GetMaxVelocity());

		this->mIsInUse = true;
	}

}
void ChargeSpell::Restore()
{
	if(this->mIsInUse)
	{
		this->mBall->SetMaxVelocity(this->mBackup.maxSpeed);
		this->mBall->SetSteering(true);
		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}