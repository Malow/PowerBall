#include "GameHandler.h"
#include "Game Modes\Knockout.h"
#include "Game Modes\Maze.h"
#include "Game Modes\Maze2.h"
#include "Game Modes\Warlock.h"
#include "Game Modes\KingOfTheHill.h"
#include "Game Modes\CaptureTheFlag.h"

GameHandler::GameHandler()
{
	this->mGameMode = NULL;
	this->mGe = NULL;
	this->mNet = NULL;
	this->mEndGameMenu = NULL;
}

GameHandler::GameHandler(GraphicsEngine* ge)
{
	this->mGameMode = NULL;
	this->mGe = ge;
	this->mNet = NULL;
	this->mEndGameMenu = new EndGameMenu(ge);
}

GameHandler::~GameHandler()
{
	SAFE_DELETE(this->mGameMode);
	this->mGe = NULL;
	SAFE_DELETE(this->mNet);
	SAFE_DELETE(this->mEndGameMenu);
}
bool GameHandler::CreateGame(int gameMode, ServerInfo server)
{
	if(gameMode == CTF)
	{
		this->CreateCaptureTheFlag(server);
		return true;
	}
	if(gameMode == KOTH)
	{
		this->CreateKingOfTheHill(server);
		return true;
	}
	if(gameMode == WARLOCK)
	{
		this->CreateWarlockGame(server);
		return true;
	}
	if(gameMode == CREDITS)
	{
		this->CreateMazeGame();
		return true;
	}
	if(gameMode == CREDITS2)
	{
		this->CreateMazeGame2();
		return true;
	}
	if(gameMode == DM)
	{
		this->CreateKnockoutGame(2, 3);
		return true;
	}
	
	return false;
}
bool GameHandler::CreateKnockoutGame(int numberOfPlayers, int numberOfRounds)
{
	if(numberOfRounds <= 0 || numberOfPlayers <= 0)
		return false;
	this->mGameMode = new Knockout(this->mGe, numberOfPlayers, numberOfRounds);
	return true;
}

bool GameHandler::CreateWarlockGame(ServerInfo server)
{
	this->mGameMode = new Warlock(this->mGe, this->mNet,server);
	return true;
}

bool GameHandler::CreateKingOfTheHill(ServerInfo server)
{
	this->mGameMode = new KingOfTheHill(this->mGe, this->mNet,server);
	return true;
}

bool GameHandler::CreateCaptureTheFlag(ServerInfo server)
{
	this->mGameMode = new CaptureTheFlag(this->mGe, this->mNet,server);
	return true;
}
bool GameHandler::CreateMazeGame()
{
	this->mGameMode = new Maze(this->mGe);
	return true;
}

bool GameHandler::CreateMazeGame2()
{
	this->mGameMode = new Maze2(this->mGe);
	return true;
}

bool GameHandler::Start()
{
	if(!this->mNet)
		this->mNet = new GameNetwork();
	bool quitByMenu = false;
	ENDGAMEMENU val = ENDGAMEMENU::PLAYAGAIN;

	if(this->mNet->IsServer())
	{
		
		this->mGameMode->Initialize();
		this->mGameMode->Intro();
		
		do
		{
			quitByMenu = this->mGameMode->PlaySpecific();
			this->mGameMode->ShowStats();
			if(!quitByMenu)
			{
				CreateThread(0, 0, &EndMenuThread, (void*)(new EndMenuThreadParam(this->mEndGameMenu, val, this->mGameMode)), 0, 0);
			}
			else val = ENDGAMEMENU::QUITGAME;

		}while(val == ENDGAMEMENU::PLAYAGAIN);

		this->DeleteCreatedGame();

	}
	else //client
	{	
			
			this->mGameMode->Initialize();
			this->mGameMode->Intro();
			
			while(this->mNet->IsRunning() && !quitByMenu && val == ENDGAMEMENU::PLAYAGAIN)
			{
				quitByMenu = this->mGameMode->PlaySpecific();
				this->mGameMode->ShowStats();
				
				/*if(!quitByMenu)
					CreateThread(0, 0, &EndMenuThread, (void*)new EndMenuThreadParam(this->mEndGameMenu, val, this->mGameMode), 0, 0);
				else val = ENDGAMEMENU::QUITGAME;*/

			}
			this->DeleteCreatedGame();
	}
	this->mNet->Close();
	SAFE_DELETE(this->mNet);
	return true;
}

bool GameHandler::DeleteCreatedGame()
{
	if(this->mGameMode)
	{
		SAFE_DELETE(this->mGameMode);
		return true;
	}
	else
		return false;
	
}

GameNetwork* GameHandler::GetLanPointer()
{
	if(!this->mNet)
		this->mNet = new GameNetwork();
	return this->mNet;
}


DWORD WINAPI GameHandler::EndMenuThread(void* param)
{
	EndMenuThreadParam* emtp = (EndMenuThreadParam*) param;
	emtp->val = ENDGAMEMENU(emtp->egm->Run());
	if(emtp->val == ENDGAMEMENU::QUITGAME)
		emtp->gm->QuitByMenu();
	delete emtp;
	return 0;
}