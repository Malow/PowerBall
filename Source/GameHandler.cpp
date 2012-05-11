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
}

GameHandler::GameHandler(GraphicsEngine* ge)
{
	this->mGameMode = NULL;
	this->mGe = ge;
	this->mNet = NULL;
}

GameHandler::~GameHandler()
{
	SAFE_DELETE(this->mGameMode);
	this->mGe = NULL;
	SAFE_DELETE(this->mNet);
}
bool GameHandler::CreateGame(int gameMode, ServerInfo server)
{
	if(gameMode == CTF)
		this->CreateCaptureTheFlag(server);
	if(gameMode == KOTH)
		this->CreateKingOfTheHill(server);
	if(gameMode == WARLOCK)
		this->CreateWarlockGame(server);
	if(gameMode == CREDITS)
		this->CreateMazeGame();
	if(gameMode == CREDITS2)
		this->CreateMazeGame2();
	if(gameMode == DM)
		this->CreateKnockoutGame(2, 3);
	
	return true;
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
	this->mGameMode->Initialize();
	this->mGameMode->Intro();
	this->mGameMode->Play();
	this->mGameMode->ShowStats();
	this->DeleteCreatedGame();
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