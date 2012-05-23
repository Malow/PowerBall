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
#include "ChooseTeamMenu.h"

#define SIZE_HUD 20
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
			ChooseTeamMenu* mChooseTeamMenu;
			int				mTeam;
			Text* mHud[SIZE_HUD];
			bool			mQuitByMenu;
			Text*			mTimeElapsedText;
			
			static DWORD WINAPI	SelectTeamThread(void* param);
			static DWORD WINAPI	InGameMenuThread(void* param);
	public:
			GameMode();
			virtual ~GameMode();
			virtual void Initialize() = 0;
			virtual void Intro() = 0;
			virtual bool PlaySpecific() = 0;
			virtual void ShowStats() = 0;
			virtual void ShowHud() = 0;
			virtual bool checkWinConditions(float dt) = 0;
			virtual void AddBall();
			

			void QuitByMenu() { this->mQuitByMenu = true; }
	protected:
			void ClientKeyPress(float diff, const int index, char key); 
			void InputKeysPressedSelf(float diff, int index, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu);
			void SendKeyInputs(const int clientIndex, float diff);
			void HandleClientKeyInputs(const int clientIndex, float diff);
			void IsServer(float diff, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu);
			void IsClient(float diff, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu);
			bool PlayLan();
			bool PlayRoundLan(bool& roundsLeft, bool& zoomInPressed, bool& zoomOutPressed);
private:

		struct ThreadParam
		{
			InGameMenu* igm;
			bool resume;
			bool finished;
			ThreadParam(InGameMenu* igm, bool &res)
			{ 
				this->igm = igm; 
				this->resume = res; 
				finished = false;
			}
			~ThreadParam(){}
		};
};