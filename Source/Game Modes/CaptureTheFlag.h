#pragma once
/**
* Authors: Jerry Rahmqvist
* Network: Didrik Axelsson
* Logic: Rikard Johansson 
* Create Date: 09/05/2012
* 
* This Class is responible for all things in the Capture the Flag game 
* from start to end.
*
**/

#include "..\Game Modes\GameMode.h"
#include "..\GameNetwork.h"


class CaptureTheFlag : public GameMode
{
	private:
			FlagCTF* mEnemyFlag;
			FlagCTF* mFriendlyFlag;
	public:
			CaptureTheFlag();
			CaptureTheFlag(GraphicsEngine* ge, GameNetwork* net, ServerInfo server);
			virtual ~CaptureTheFlag();
			virtual void Initialize();
			virtual void Intro();
			virtual void Play();
			virtual void ShowStats();
			virtual bool checkWinConditions(float dt);
			void AddBall();

	
};