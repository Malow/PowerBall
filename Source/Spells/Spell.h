#pragma once

#include "GraphicsEngine\MaloWLib\Vector.h"
using namespace MaloW;

class Ball;
class Spell
{
	protected:
		Ball* mBall;
		float mTimeNeededToCoolDown;
		float mTimerCounterCoolDown;
		bool mNeedCoolDown;
		float mTimerCounterInUse;
		float mMaxTimeUse;
		bool mIsInUse;
		struct  backup
		{
			Vector3 velocity;
			float maxSpeed;
			float forcePress;
			float mass;
			float e;
		};
	public:
		Spell();
		virtual ~Spell();
		void Update(float dt);
		virtual void UpdateSpecial(float dt) = 0;
		virtual void Use() = 0;
		virtual void Restore() = 0;
		void Ready();
		virtual void InformCollision() = 0;
		void SetBall(Ball* ball) { this->mBall = ball; }
		bool SpellInUse();
};