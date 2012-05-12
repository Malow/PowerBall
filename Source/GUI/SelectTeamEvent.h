#pragma once
/**
* Author: Rikard Johansson
* Create Date: 12/04/2012
* This event is a about picking team.
**/
#include "GUIEvent.h"

class SelectTeamEvent : public GUIEvent
{
private:
	int mTeam;
public:
	SelectTeamEvent(int team);
	virtual ~SelectTeamEvent();

	int GetTeam(){ return this->mTeam; }
};