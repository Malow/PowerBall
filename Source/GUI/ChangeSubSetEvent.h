#pragma once
/**
* Author: Rikard Johansson
* Create Date: 04/04/2012
*
* This event will be returned when you are going into a new menu
**/

#include "GUIEvent.h"

class ChangeSubSetEvent : public GUIEvent
{
private:
	int mSet;
	int mGameMode;
public:
	ChangeSubSetEvent();
	ChangeSubSetEvent(int set, int GameMode);
	virtual ~ChangeSubSetEvent();

	/*! Changes the value that explane what set that is going to be active*/
	void SetSet(int set){ this->mSet = set; }
	/*! Returns the Set value*/
	int GetSet(){ return this->mSet; }

	int GetGameMode(){ if(this->mGameMode) return this->mGameMode; else return NONE; }
	void SetGameMode(int GameMode){ this->mGameMode = GameMode; }
};