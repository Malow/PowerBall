#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 09/05/2012
* 
* This Class is the abstract class for the game modes.
*
**/

#include "GraphicsEngine.h"
#include "..\Game Objects\Map.h"
#include "..\Game Objects\PowerBall.h"
#include "..\InGameMenu.h"
#include "..\Network\GameNetwork.h"

class GameMode
{
	protected:
			GraphicsEngine* mGe;
			InGameMenu*		mIGM;
			int				mGameMode;
			GameNetwork*	mNet;
			ServerInfo mServerInfo;
			Map*			mPlatform;
			PowerBall**		mBalls; 
			int				mNumberOfPlayers;
			int				mNumberOfRounds;
			Light*			mLights[5];
			float			mTimeElapsed;
	public:
			GameMode();
			virtual ~GameMode();
			virtual void Initialize() = 0;
			virtual void Intro() = 0;
			virtual void Play() = 0;
			virtual void ShowStats() = 0;
			virtual bool checkWinConditions(float dt) = 0;

	protected:
			void InputKnockout(float diff, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& roundsLeft, bool& quitByMenu);
			void ClientKeyPress(float diff, const int index, char key); 
			void InputKeysPressedSelf(float diff, int index, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu);
			void SendKeyInputs(const int clientIndex, float diff);
			void HandleClientKeyInputs(const int clientIndex, float diff);
			
};