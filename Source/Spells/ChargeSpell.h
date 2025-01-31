#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 03/05/2012
* 
* 
**/
#include "..\Spells\Spell.h"


class ChargeSpell : public Spell
{
private:
	backup mBackup;
	bool mChargingDone;
	float mTimeHoldButton;
	float mMaxChargingTime;
	
public:
	ChargeSpell();
	//ChargeSpell(Ball* ball);
	ChargeSpell(PowerBall* ball);
	ChargeSpell(SoundEffect* soundEffect);
	virtual ~ChargeSpell();
	virtual void UpdateSpecial(float dt);
	virtual void Use();
	virtual void Restore();
	virtual void InformCollision() { this->Restore();}
	float GetMaxTimeCharging() const;
	float GetTimerCharging() const;
};