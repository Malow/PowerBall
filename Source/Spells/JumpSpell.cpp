#include "JumpSpell.h"
#include "..\Game Objects\PowerBall.h"

JumpSpell::JumpSpell()
{
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 10.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 0.1f;
	this->mIsInUse = false;
	this->mCharging = false;
}


JumpSpell::JumpSpell(PowerBall* ball)
{

	this->mPowerBall = ball;
	this->mTimeNeededToCoolDown = 10.0f;
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 0.1f;
	this->mIsInUse = false;
}

JumpSpell::~JumpSpell()
{
	
}

void JumpSpell::UpdateSpecial(float dt)
{

	Vector3 distance = this->mPowerBall->GetPositionVector3() - this->mJumpPosition;
	Vector3 projectionNormalJump = this->mBackup.normalContact*(distance.GetDotProduct(this->mBackup.normalContact));
	float length = projectionNormalJump.GetLength();
	if(length >= 2.0f)
	{
		this->Restore();
	}
	if(this->mTimerCounterCoolDown > this->mTimeNeededToCoolDown)
		Spell::Ready();
	Spell::Update(dt);
}

void JumpSpell::Use()
{
	if(!this->mNeedCoolDown && !this->mIsInUse && !this->SpellInUse())
	{
		
		/* backup ball info */
		this->mBackup.acceleration = this->mPowerBall->GetAcceleration();
		
		/* new behaviour for ball */
		Vector3 normal = this->mPowerBall->GetNormalContact();
		this->mBackup.normalContact = normal;
		/* the speed of the jump is a litle bit faster than the "gravity speed" */
		normal = normal*(this->mBackup.acceleration.GetLength()*4.0f); 
		this->mPowerBall->SetAcceleration(normal);
		
		/*	saving information to know when the ball has jumped far enough from the position 
		*	where the jump was started 
		*/
		this->mJumpPosition = this->mPowerBall->GetPositionVector3();
		this->mIsInUse = true;
	}

}
void JumpSpell::Restore()
{
	if(this->mIsInUse)
	{

		this->mPowerBall->SetAcceleration(this->mBackup.acceleration);
		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}