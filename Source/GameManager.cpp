#include "GameManager.h"

GameManager::GameManager(GraphicsEngine* ge)
{
	this->mNumPlayers	= 0;
	this->mPlatform		= NULL;
	this->mBalls		= NULL;
}
GameManager::~GameManager()
{
	//SAFE_DELETE(this->mPlatform)
	//SAFE_DELETE(this->mBalls) //for loop
}

bool GameManager::Play(const int numPlayers)
{
	this->mNumPlayers = numPlayers;
	this->Initialize();
	bool running = true;

	while(running)
	{
		int numAlivePlayers = 0;
		this->mPlatform->Update(0); //Timer->GetDt() or something

		for(int i = 0; i < this->mNumPlayers; i++)
		{
			this->mBalls[i]->Update(0);

			if(this->mBalls[i]->IsAlive())
				numAlivePlayers += 1;
		}

		if(numAlivePlayers <= 1)
			running = false;
	}

	//returns to menu after some win/draw screen.
	return true;
}

void GameManager::Initialize()
{
	//SAFE_DELETE(this->mPlatform)
	//SAFE_DELETE(this->mBalls) //for loop
	this->mPlatform		= new Platform("platform.obj", 0.0f);
	this->mBalls		= new Ball*[this->mNumPlayers];

	for(int i = 0; i < this->mNumPlayers; i++)
	{
		this->mBalls[i] = new Ball("ball.obj", 1.0f, 1.0f, 1.0f);
	}
}

