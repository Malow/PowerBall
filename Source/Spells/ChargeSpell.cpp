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
	this->mTimeHoldButton = 0.0f;
	this->mMaxChargingTime = 3.0f;
	this->mCharging = false;
	this->mChargingDone = false;
	
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
	this->mTimeHoldButton = 0.0f;
	this->mMaxChargingTime = 3.0f;
	this->mCharging = false;
	this->mChargingDone = false;
}



ChargeSpell::~ChargeSpell()
{
	
}

void ChargeSpell::UpdateSpecial(float dt)
{
	if(this->mCharging)
	{
		Vector3 normal = this->mPowerBall->GetNormalContact();
		Vector3 direction = this->mPowerBall->GetForwardVector();
		Vector3 rotateAround = normal.GetCrossProduct(direction);
		rotateAround.normalize();
		this->mPowerBall->GetMesh()->RotateAxis(rotateAround.GetD3DVec(), (PI/2)*this->mTimeHoldButton*this->mTimeHoldButton*dt);
		
		if(this->mTimeHoldButton <= this->mMaxChargingTime)
		{
			this->mTimeHoldButton += dt;	
			Vector3 acc = this->mPowerBall->GetAcceleration();
			acc += acc*dt*0.1f;
			this->mPowerBall->SetAcceleration(acc);
		}
		Vector3 newPos = this->mPowerBall->GetPositionVector3() + this->mBackup.normalContact*dt;
		this->mPowerBall->SetPosition(newPos);
		
		if(!GetGraphicsEngine()->GetKeyListener()->IsPressed('1'))
		{

			if(this->mTimeHoldButton <= 1)
				this->mTimeHoldButton = 0.5*this->mMaxChargingTime;
			/* change back to old behaviour */
			this->mPowerBall->SetAcceleration(this->mBackup.acceleration);
			/* setting the Camera to follow the position changes again. */
			((TRDCamera*)GetGraphicsEngine()->GetCamera())->enablePositionChanges();
			this->mChargingDone = true;
			this->mCharging = false;
			this->Use();
		}

	}
	if(this->mTimerCounterInUse > this->mMaxTimeUse)
		this->Restore();
	if(this->mTimerCounterCoolDown > this->mTimeNeededToCoolDown)
		Spell::Ready();
	Spell::Update(dt);
}

void ChargeSpell::Use()
{
	if(!this->mNeedCoolDown && !this->mIsInUse && !this->SpellInUse() && !this->mCharging)
	{
		if(this->mChargingDone)
		{
			

			/* backup ball info */
			this->mBackup.maxSpeed = this->mPowerBall->GetMaxVelocity();
			/* new behaviour for ball */
			this->mPowerBall->SetMaxVelocity(this->mPowerBall->GetMaxVelocity()*5.0f*(this->mTimeHoldButton/this->mMaxChargingTime));
			this->mPowerBall->SetSteering(false);
			this->mPowerBall->SetVelocity(this->mPowerBall->GetForwardVector()*this->mPowerBall->GetMaxVelocity());
		
			this->mIsInUse = true;
			this->mChargingDone = false;
			this->mTimeHoldButton = 0.0f;
		}
		else
		{
			this->mCharging = true;
			/* backing up ball for behaviour during the charging (ball is rotating) and has more gravity and is lifted in normal direction */

			/* backup ball info */
			this->mBackup.acceleration = this->mPowerBall->GetAcceleration();
		
			/* new behaviour for ball */
			this->mBackup.normalContact = this->mPowerBall->GetNormalContact();

			/* faster "gravity" speed  = more jumpy */
			Vector3 acc = this->mPowerBall->GetAcceleration();
			acc = acc * 3.0f;
			this->mPowerBall->SetAcceleration(acc);

			/* setting the Camera to not follow the position changes, otherwise the cam will jump. */
			((TRDCamera*)GetGraphicsEngine()->GetCamera())->disablePositionChanges();
			
		}
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