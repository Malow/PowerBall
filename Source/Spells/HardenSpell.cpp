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
	this->mHardenMesh = GetGraphicsEngine()->CreateStaticMesh("Media/HardenedBall.obj", D3DXVECTOR3(300,300,3000));
	

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
	this->mHardenMesh = GetGraphicsEngine()->CreateStaticMesh("Media/HardenBall.obj", D3DXVECTOR3(300,300,3000));
	
}
HardenSpell::HardenSpell(SoundEffect* soundEffect) : Spell(soundEffect)
{
	this->mPowerBall = NULL;
	this->mTimeNeededToCoolDown = 1.0f; // 15.0f
	this->mTimerCounterCoolDown = 0.0f;
	this->mNeedCoolDown = false;
	this->mTimerCounterInUse = 0.0f;
	this->mMaxTimeUse = 3.0f;
	this->mIsInUse = false;
	this->mHardenMesh = GetGraphicsEngine()->CreateStaticMesh("Media/HardenedBall.obj", D3DXVECTOR3(300,300,3000));
	
}

HardenSpell::~HardenSpell()
{
	GetGraphicsEngine()->DeleteStaticMesh(this->mHardenMesh);
}

void HardenSpell::UpdateSpecial(float dt)
{

	if(this->mTimerCounterInUse > this->mMaxTimeUse)
		this->Restore();
	if(this->mTimerCounterCoolDown > this->mTimeNeededToCoolDown)
		Spell::Ready();
	Spell::Update(dt);


	/* to make sure the stone is following the mesh of the ball */ 
	if(this->mIsInUse)
		this->mHardenMesh->SetPosition(this->mPowerBall->GetMesh()->GetPosition());
	
}

void HardenSpell::Use()
{
	if(!this->mNeedCoolDown && !this->mIsInUse && !this->SpellInUse())
	{
		/* switch mesh here */

		/* backup ball info */
		this->mBackup.mass = this->mPowerBall->GetMass();
		
		
		/* new behaviour for ball */
		this->mHardenMesh->SetPosition(this->mPowerBall->GetMesh()->GetPosition());
		this->mPowerBall->SetSteering(false);
		this->mPowerBall->SetMass(10000.0f);
		this->mPowerBall->SetVelocity(Vector3(0,0,0));

		this->mIsInUse = true;

		//play sound effect, if any
		if(this->mSoundEffect)
		{
			this->mSoundEffect->Play();
		}
	}

}
void HardenSpell::Restore()
{
	if(this->mIsInUse)
	{
		/* restore mesh here */
		this->mHardenMesh->SetPosition(D3DXVECTOR3(300,300,3000));
		this->mPowerBall->SetSteering(true);
		this->mPowerBall->SetMass(this->mBackup.mass);

		this->mIsInUse = false;
		this->mTimerCounterInUse = 0.0f;
		this->mNeedCoolDown = true;
	}
}