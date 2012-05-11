#include "ChargeSpell.h"
//#include "Ball.h"
#include "..\Game Objects\PowerBall.h"

ChargeSpell::ChargeSpell()
{
	//this->mBall = NULL;
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 15.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 1.0f;
	this->mIsInUse = false;
}

/*ChargeSpell::ChargeSpell(Ball* ball)
{
	this->mBall = ball;
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 15.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 1.0f;
	this->mIsInUse = false;
}*/

ChargeSpell::ChargeSpell(PowerBall* ball)
{
	//this->mBall = NULL;
	this->mPowerBall = ball;
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
		this->mBackup.maxSpeed = this->mPowerBall->GetMaxVelocity();
		
		/* new behaviour for ball */
		this->mPowerBall->SetMaxVelocity(this->mPowerBall->GetMaxVelocity()*5.0f);
		this->mPowerBall->SetSteering(false);
		this->mPowerBall->SetVelocity(this->mPowerBall->GetForwardVector()*this->mPowerBall->GetMaxVelocity());

		this->mIsInUse = true;
	}

}
void ChargeSpell::Restore()
{
	if(this->mIsInUse)
	{
		this->mPowerBall->SetMaxVelocity(this->mBackup.maxSpeed);
		this->mPowerBall->SetSteering(true);
		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}