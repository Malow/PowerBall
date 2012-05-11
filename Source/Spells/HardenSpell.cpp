#include "HardenSpell.h"
//#include "Ball.h"
#include "..\Game Objects\PowerBall.h"

HardenSpell::HardenSpell()
{
	//this->mBall = NULL;
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 15.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 3.0f;
	this->mIsInUse = false;
}

//HardenSpell::HardenSpell(Ball* ball)
//{
//	this->mBall = ball;
//	this->mPowerBall = NULL;
//	this->mTimeNeededToCoolDown = 15.0f;
//	this->mTimerCounterCoolDown = 0.0f;
//	this->mNeedCoolDown = false;
//	this->mTimerCounterInUse = 0.0f;
//	this->mMaxTimeUse = 3.0f;
//	this->mIsInUse = false;
//}

HardenSpell::HardenSpell(PowerBall* ball)
{
	//this->mBall = NULL;
	this->mPowerBall = ball;
	this->mTimeNeededToCoolDown = 15.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 3.0f;
	this->mIsInUse = false;
}

HardenSpell::~HardenSpell()
{
	
}

void HardenSpell::UpdateSpecial(float dt)
{
	if(this->mTimerCounterInUse > this->mMaxTimeUse)
		this->Restore();
	if(this->mTimerCounterCoolDown > this->mTimeNeededToCoolDown)
		Spell::Ready();
	Spell::Update(dt);
}

void HardenSpell::Use()
{
	if(!this->mNeedCoolDown && !this->mIsInUse && !this->SpellInUse())
	{
		/* switch mesh here */

		/* backup ball info */
		this->mBackup.mass = this->mPowerBall->GetMass();
		
		/* new behaviour for ball */
		this->mPowerBall->SetSteering(false);
		this->mPowerBall->SetMass(10000.0f);
		this->mPowerBall->SetVelocity(Vector3(0,0,0));

		this->mIsInUse = true;
	}

}
void HardenSpell::Restore()
{
	if(this->mIsInUse)
	{
		/* restore mesh here */

		this->mPowerBall->SetSteering(true);
		this->mPowerBall->SetMass(this->mBackup.mass);
		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}