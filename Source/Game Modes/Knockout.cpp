#include "Knockout.h"

Knockout::Knockout()
{
	this->mGe = NULL;
	this->mIGM = NULL;
	this->mBalls = NULL;
	this->mPlatform = NULL;
	this->mNumberOfPlayers = -1;
	this->mNumberOfRounds = -1;
	this->mGameMode = -1;
	this->mTimeElapsed = 0.0f;

}

Knockout::Knockout(GraphicsEngine* ge, int numberOfPlayers, int numberOfRounds)
{
	this->mGe = ge;
	this->mNumberOfPlayers = numberOfPlayers;
	this->mNumberOfRounds = numberOfRounds;
	this->mGameMode = DM;
	this->mTimeElapsed = 0.0f;
}

Knockout::~Knockout()
{
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
	SAFE_DELETE(this->mIGM);
}

void Knockout::Initialize()
{
	D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,10,0);
	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 25, -10));
	mGe->GetCamera()->LookAt(centerPlatform);	
	this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
	this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
	this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
	this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
	this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
	for(int i = 0; i < 5; i++)
		this->mLights[i]->SetIntensity(30.0f);
	this->mIGM	= new InGameMenu(this->mGe);
	this->mPlatform		= new Map("Media/Cylinder.obj", centerPlatform);
	this->mBalls		= new PowerBall*[this->mNumberOfPlayers];
	for(int i = 0; i < this->mNumberOfPlayers; i++)
	{
		if( i == 0)
		{
				this->mBalls[i] = new PowerBall("Media/Ball.obj", D3DXVECTOR3(0,25.0f,-5));
				this->mBalls[i]->SetKnockoutMode();
				this->mBalls[i]->SetSound(true);
		}
		else
		{
				this->mBalls[i] = new PowerBall("Media/Ball.obj", D3DXVECTOR3(0,25.0f,5));
				this->mBalls[i]->SetKnockoutMode();
				this->mBalls[i]->SetSound(true);
		}
	}
}

void Knockout::Intro()
{
	Text*	intro = mGe->CreateText("Knockout",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
	mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
	intro->SetText("");
	mGe->DeleteText(intro);
}

void Knockout::Play()
{	
	int roundsPlayed = 0;
	bool running = true;
	bool zoomOutPressed = false;
	bool zoomInPressed = false;
	
	Text* hudR1 = mGe->CreateText("",D3DXVECTOR2(20,20),1.0f,"Media/Fonts/1");
	Text* hudR2 = mGe->CreateText("",D3DXVECTOR2(20,60),1.0f,"Media/Fonts/1");
	Text* hudR3 = mGe->CreateText("",D3DXVECTOR2(20,100),1.0f,"Media/Fonts/1");
	Text* hudR4 = mGe->CreateText("",D3DXVECTOR2(300,150),4.0f,"Media/Fonts/1");
	Text* hudR5 = mGe->CreateText("",D3DXVECTOR2(250,150),4.0f,"Media/Fonts/1");
	Text* hudR6 = mGe->CreateText("",D3DXVECTOR2(100,150),4.0f,"Media/Fonts/1");
	string s;
	int indexBallLeft = -1;
	bool roundsLeft = true;
	float diff;
	bool quitByMenu = false;
	bool winnerActivated = false;
	while(roundsLeft)
	{
		int numberAlivePlayersThisRound;
		roundsPlayed++;
		while(running)
		{
			numberAlivePlayersThisRound = 0;
			diff = mGe->Update();	
			s = "Player 1: " + this->mBalls[0]->GetRoundsWonStr();
			hudR1->SetText(s);
			s = "Player 2: " + this->mBalls[1]->GetRoundsWonStr();
			hudR2->SetText(s);
			s = "Round: " + MaloW::convertNrToString(roundsPlayed) + " of " + MaloW::convertNrToString(this->mNumberOfRounds);
			hudR3->SetText(s);
			this->InputKnockout(diff,zoomOutPressed,zoomInPressed, running, roundsLeft, quitByMenu);
			
			for(int i = 0; i < this->mNumberOfPlayers; i++)
			{
				this->mBalls[i]->Update(diff);
				if(this->mBalls[i]->IsAlive())
				{
					indexBallLeft = i;
					numberAlivePlayersThisRound += 1;
				}
			}
		
			for(int i = 0; i < this->mNumberOfPlayers; i++)
			{
				PowerBall* b1 = this->mBalls[i];
				for(int j = i+1; j < this->mNumberOfPlayers; j++)
				{
					PowerBall* b2 = this->mBalls[j];
					if(b1->collisionWithSphereSimple(b2))
						b1->collisionSphereResponse(b2);
				}
				
				Vector3 normalPlane;
				if(b1->collisionWithPlatformSimple(this->mPlatform,normalPlane))
					b1->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
				
			}
			for(int i = 0; i<this->mNumberOfPlayers;i++)
			{
				this->mBalls[i]->UpdatePost();
			}
			mPlatform->Update(diff);
			if(!this->mGe->isRunning())
			{
				roundsLeft = running = false;
				quitByMenu = !running;
			}
			if(numberAlivePlayersThisRound == 1)
			{
				if(!winnerActivated)
				{
					winnerActivated = true;
					this->mBalls[indexBallLeft]->ActivateWinTimer();
				}
				if(this->mBalls[indexBallLeft]->GetWinTimer() > 1.0f)
				{
					this->mBalls[indexBallLeft]->AddWonRound();
					this->mBalls[indexBallLeft]->RestetWinTimer();
					running = false;
					// winner, show winner text for 2 seconds
					s = "Winner: Player " + MaloW::convertNrToString(indexBallLeft+1);
					hudR5->SetText(s);
					while(diff < 2000)
						diff += mGe->Update();
					hudR5->SetText("");
					winnerActivated = false;
				}
			}
			else if(winnerActivated)
			{
				// draw, show draw text for 2 seconds
				s = "Draw";
				hudR4->SetText(s);
				while(diff < 2000)
						diff += mGe->Update();
				hudR4->SetText("");
				this->mBalls[indexBallLeft]->RestetWinTimer();
				running = false;
				winnerActivated = false;
			}
			else if(numberAlivePlayersThisRound == 0 )
			{
				// is if both the balls are below y = -6 in the same update
				// draw, show draw text for 2 seconds
				s = "Draw in the same update!";
				hudR6->SetText(s);
				while(diff < 2000)
						diff += mGe->Update();
				hudR6->SetText("");
				running = false;
			}
			if(this->checkWinConditions(diff))
				running = false;
		}
		if(roundsPlayed == this->mNumberOfRounds)
			roundsLeft = false;
		else
		{
			this->mPlatform->Reset();
			for(int i = 0; i<this->mNumberOfPlayers;i++)
				this->mBalls[i]->SetToStartPosition();
			running = true;
		}
	}
	mGe->DeleteText(hudR1);
	mGe->DeleteText(hudR2);
	mGe->DeleteText(hudR3);
	mGe->DeleteText(hudR4);
	mGe->DeleteText(hudR5);
	mGe->DeleteText(hudR6);
	if(!quitByMenu && this->mNumberOfRounds > 0)
	{
		int indexWinner = -1;
		int bestResault = 0;
		if(this->mNumberOfPlayers >0)
		{
			bestResault = this->mBalls[0]->GetRoundsWon();
			indexWinner = 0;
		}
		bool draw = false;
		for(int i = 1; i<this->mNumberOfPlayers;i++)
		{
			if(this->mBalls[i]->GetRoundsWon() >= bestResault)
			{
				if(this->mBalls[i]->GetRoundsWon() == bestResault)
					draw = true;
				bestResault = this->mBalls[i]->GetRoundsWon();
				indexWinner = i;
			}
		}
		if(!draw)
		{
			string win = "Winner: Player " + MaloW::convertNrToString(indexWinner+1) + " with " + MaloW::convertNrToString(bestResault) + " won";
			Text* winner = mGe->CreateText(win,D3DXVECTOR2(250,300),2.5f,"Media/Fonts/1");
			diff = mGe->Update();
			while(diff < 2000)
				diff += mGe->Update();
			mGe->DeleteText(winner);
		}
		else
		{
			string win = "No winner in this Game";
			Text* winner = mGe->CreateText(win,D3DXVECTOR2(250,300),2.0f,"Media/Fonts/1");
			diff = mGe->Update();
			while(diff < 2000)
				diff += mGe->Update();
			mGe->DeleteText(winner);
		}
	}
	
}

void Knockout::ShowStats()
{

}

bool Knockout::checkWinConditions(float dt)
{
	return false;
}