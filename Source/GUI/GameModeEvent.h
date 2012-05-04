#pragma once

#include "GUIEvent.h"
#include "..\stdafx.h"

class GameModeEvent : public GUIEvent
{
private:
	int mGameMode;
public:
	GameModeEvent();
	GameModeEvent(int GameMode);
	virtual ~GameModeEvent();
	int GetGameMode(){ if(this->mGameMode) return this->mGameMode; else return NONE; }
	void SetGameMode(int GameMode){ this->mGameMode = GameMode; }
};