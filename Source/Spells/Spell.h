#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 03/05/2012
* 
* 
**/
#include "GraphicsEngine\MaloWLib\Vector.h"
#include "..\SoundEffect.h"
using namespace MaloW;

//class Ball;
class PowerBall;
class Spell
{
	protected:
		//Ball* mBall;
		PowerBall* mPowerBall;
		float mTimeNeededToCoolDown;
		float mTimerCounterCoolDown;
		bool mNeedCoolDown;
		float mTimerCounterInUse;
		float mMaxTimeUse;
		bool mIsInUse;
		bool mCharging;
		struct  backup
		{
			Vector3 velocity;
			Vector3 acceleration;
			Vector3 normalContact;
			float maxSpeed;
			float forcePress;
			float mass;
			float e;
		};
		bool mKeyUp;
		SoundEffect* mSoundEffect;
	public:
		Spell();
		Spell(SoundEffect* soundEffect);
		virtual ~Spell();
		void Update(float dt);
		virtual void UpdateSpecial(float dt) = 0;
		virtual void Use() = 0;
		virtual void Restore() = 0;
		void KeyUp();
		void Ready();
		virtual void InformCollision() = 0;
		//void SetBall(Ball* ball) { this->mBall = ball; }
		void SetPowerBall(PowerBall* ball) { this->mPowerBall = ball; }
		bool SpellInUse();
		float GetCoolDownTimeLeft() const { return (this->mTimeNeededToCoolDown - this->mTimerCounterCoolDown); }
		bool InUse() const { return this->mIsInUse; }
		bool NeedCoolDown() const { return this->mNeedCoolDown; }
		bool IsCharging() const { return this->mCharging; }
		float GetTimeNeededToCoolDown() const { return this->mTimeNeededToCoolDown; }
		float GetTimerCounterCoolDown() const { return this->mTimerCounterCoolDown; }
		float GetTimerCounterInUse() const { return this->mTimerCounterInUse; }
		float GetMaxTimeUse() const { return this->mMaxTimeUse; }

		/* Add a sound effect associated with the spell. */
		void SetSoundEffect(SoundEffect* soundEffect) { this->mSoundEffect = soundEffect; }

};