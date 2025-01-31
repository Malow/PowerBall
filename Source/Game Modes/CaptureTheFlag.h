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
#include "..\Network\GameNetwork.h"


class CaptureTheFlag : public GameMode
{
	private:
			FlagCTF*	mEnemyFlag;		
			FlagCTF*	mFriendlyFlag;	
			FlagCTF*	mRedFlag;		
			FlagCTF*	mBlueFlag;	
			int			mRedScore;
			int			mBlueScore;
			Text*		mRedScoreText;
			Text*		mBlueScoreText;
			Text*		mIntermediateText;


	public:
			CaptureTheFlag();
			CaptureTheFlag(GraphicsEngine* ge, GameNetwork* net, ServerInfo server);
			virtual ~CaptureTheFlag();
			virtual void Initialize();
			virtual void Intro();
			virtual bool PlaySpecific();
			virtual void ShowStats();
			virtual void ShowHud();
			virtual void HideHud();
			virtual bool checkWinConditions(float dt);
			virtual void AddBall();

	
};