#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 09/05/2012
* 
* This Class is responible for all things in the Knockout game 
* from start to end.
*
**/

#include "..\Game Modes\GameMode.h"


class Knockout : public GameMode
{
	private:
			bool mWinnerActivated;
			int mRoundsPlayed;
			int mIndexBallLeft;
	public:
			Knockout();
			Knockout(GraphicsEngine* ge, int numberOfPlayers, int numberOfRounds);
			virtual ~Knockout();
			virtual void Initialize();
			virtual void Intro();
			virtual void PlaySpecific();
			virtual void ShowStats();
			virtual void ShowHud();
			virtual bool checkWinConditions(float dt);

};