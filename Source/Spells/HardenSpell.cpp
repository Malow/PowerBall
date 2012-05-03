#include "HardenSpell.h"
#include "Ball.h"

HardenSpell::HardenSpell()
{
	this->mBall = NULL;
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
		this->mBackup.mass = this->mBall->GetMass();
		
		/* new behaviour for ball */
		this->mBall->SetSteering(false);
		this->mBall->SetMass(10000.0f);
		this->mBall->SetVelocity(Vector3(0,0,0));

		this->mIsInUse = true;
	}

}
void HardenSpell::Restore()
{
	if(this->mIsInUse)
	{
		/* restore mesh here */

		this->mBall->SetSteering(true);
		this->mBall->SetMass(this->mBackup.mass);
		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}