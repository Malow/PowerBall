#include "InvisibilitySpell.h"
//#include "Ball.h"
#include "..\Game Objects\PowerBall.h"

InvisibilitySpell::InvisibilitySpell()
{
	//this->mBall = NULL;
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 30.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 5.0f;
	this->mIsInUse = false;
}

/*invisibilitySpell::InvisibilitySpell(Ball* ball)
{
	this->mBall = ball;
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 30.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 5.0f;
	this->mIsInUse = false;
}*/

InvisibilitySpell::InvisibilitySpell(PowerBall* ball)
{
	//this->mBall = NULL;
	this->mPowerBall = ball;
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
		/* set mesh to use invisibility effect */
		this->mPowerBall->UseInvisibilityEffect(true);
		
		/* backup ball info */
		//this->mBackup.e = this->mBall->GetRestitution();
		this->mBackup.mass = this->mPowerBall->GetMass();
		
		/* new behaviour for ball */
		//this->mBall->SetRestitution(0.5f);
		this->mPowerBall->SetMass(this->mPowerBall->GetMass());

		this->mIsInUse = true;
	}

}
void InvisibilitySpell::Restore()
{
	if(this->mIsInUse)
	{
		this->mPowerBall->UseInvisibilityEffect(false);
		//this->mBall->SetRestitution(this->mBackup.e);
		this->mPowerBall->SetMass(mBackup.mass);
		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}