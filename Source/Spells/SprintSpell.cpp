#include "SprintSpell.h"
//#include "Ball.h"
#include "..\Game Objects\PowerBall.h"

SprintSpell::SprintSpell()
{
	//this->mBall = NULL;
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 20.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 5.0f;
	this->mIsInUse = false;
}

/*SprintSpell::SprintSpell(Ball* ball)
{
	this->mBall = ball;
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 20.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 5.0f;
	this->mIsInUse = false;
}*/

SprintSpell::SprintSpell(PowerBall* ball)
{
	//this->mBall = NULL;
	this->mPowerBall = ball;
	this->mTimeNeededToCoolDown = 20.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 5.0f;
	this->mIsInUse = false;
}
SprintSpell::SprintSpell(SoundEffect* soundEffect) : Spell(soundEffect)
{
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 20.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 5.0f;
	this->mIsInUse = false;
}
SprintSpell::~SprintSpell()
{
	
}

void SprintSpell::UpdateSpecial(float dt)
{
	if(this->mTimerCounterInUse > this->mMaxTimeUse)
		this->Restore();
	if(this->mTimerCounterCoolDown > this->mTimeNeededToCoolDown)
		this->Ready();
	Spell::Update(dt);
}

void SprintSpell::Use()
{
	if(!this->mNeedCoolDown && !this->mIsInUse && !this->SpellInUse())
	{
		/* backup ball info */
		this->mBackup.maxSpeed = this->mPowerBall->GetMaxVelocity();
		this->mBackup.forcePress = this->mPowerBall->GetForcePressed();
		
		/* new behaviour for ball */
		this->mPowerBall->SetMaxVelocity(this->mPowerBall->GetMaxVelocity() * 2.0f);
		this->mPowerBall->SetForcePressed(this->mPowerBall->GetForcePressed() * 2.0f);

		this->mIsInUse = true;

		//play sound, if any
		if(this->mSoundEffect)
		{
			this->mSoundEffect->Play();
		}
	}

}
void SprintSpell::Restore()
{
	if(this->mIsInUse)
	{
		this->mPowerBall->SetMaxVelocity(this->mBackup.maxSpeed);
		this->mPowerBall->SetForcePressed(this->mBackup.forcePress);
		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}