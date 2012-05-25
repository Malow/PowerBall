#include "Knockout.h"
#include "..\Physics\PhysicsEngine.h"

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
	for(int i = 0;i<6;i++)
		this->mGe->DeleteText(this->mHud[i]);
	for(int i = 0;i<this->mNumberOfPlayers; i++)
		this->mPe->RemoveBody(this->mBalls[i]);
	this->mPe->RemoveMap(this->mPlatform);
	SAFE_DELETE(this->mPe);
}

void Knockout::Initialize()
{
	D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,10,0);
	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 25, -10)); // y = 25
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
			this->mBalls[i] = new PowerBall("Media/Ball.obj", D3DXVECTOR3(0,25.0f,-5), GAMEMODE::DM);
				this->mBalls[i]->SetKnockoutMode();
				this->mBalls[i]->SetSound(true);
		}
		else
		{
				this->mBalls[i] = new PowerBall("Media/Ball.obj", D3DXVECTOR3(0,25.0f,5), GAMEMODE::DM);
				this->mBalls[i]->SetKnockoutMode();
				this->mBalls[i]->SetSound(true);
		}
	}

	this->mPe = new PhysicsEngine(this->mGe);
	this->mPe->AddBody(this->mBalls[0]);
	this->mPe->AddBody(this->mBalls[1]);
	this->mPe->AddMap(this->mPlatform);


	this->mHud[0] = mGe->CreateText("",D3DXVECTOR2(20,20),1.0f,"Media/Fonts/1");
	this->mHud[1] = mGe->CreateText("",D3DXVECTOR2(20,60),1.0f,"Media/Fonts/1");
	this->mHud[2] = mGe->CreateText("",D3DXVECTOR2(20,100),1.0f,"Media/Fonts/1");
	this->mHud[3] = mGe->CreateText("",D3DXVECTOR2(300,150),4.0f,"Media/Fonts/1");
	this->mHud[4] = mGe->CreateText("",D3DXVECTOR2(250,150),2.0f,"Media/Fonts/1");
	this->mHud[5] = mGe->CreateText("",D3DXVECTOR2(100,150),4.0f,"Media/Fonts/1");

	this->mTimeElapsedText = this->mGe->CreateText(	"", D3DXVECTOR2(15.0f, 10.0f), 1.0f, "Media/Fonts/1");
}

void Knockout::Intro()
{
	Text*	intro = mGe->CreateText("Knockout",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
	mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
	intro->SetText("");
	mGe->DeleteText(intro);
}

bool Knockout::PlaySpecific()
{	
	
	bool roundsLeft = true;
	this->mRoundsPlayed = 0;
	this->mGe->Update();
	this->mIndexBallLeft = -1;
	this->mWinnerActivated = false;
	this->mPe->Initialize();

	while(roundsLeft && this->mGe->isRunning())
	{
		
		this->PlayRound(roundsLeft); 
		this->mRoundsPlayed++;

		if(this->mRoundsPlayed == this->mNumberOfRounds)
			roundsLeft = false;
		else
		{
			this->mPlatform->Reset();
			for(int i = 0; i<this->mNumberOfPlayers;i++)
				this->mBalls[i]->SetToStartPosition();
		}
	
	}

	if(this->mQuitByMenu || !roundsLeft || !this->mGe->isRunning())
		return true;
	else
		return false;
	
}

void Knockout::ShowStats()
{
	float diff = mGe->Update();
	if(!this->mQuitByMenu && this->mNumberOfRounds > 0)
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

bool Knockout::checkWinConditions(float dt)
{
	
	int numberAlivePlayersThisRound = 0;
	for(int i = 0;i<this->mNumberOfPlayers;i++)
	{
		if(this->mBalls[i]->IsAlive())
		{
			this->mIndexBallLeft = i;
			numberAlivePlayersThisRound += 1;
		}
	}
		
	if(numberAlivePlayersThisRound == 1)
	{
		if(!this->mWinnerActivated)
		{
			this->mWinnerActivated = true;
			this->mBalls[this->mIndexBallLeft]->ActivateWinTimer();
		}
		if(this->mBalls[this->mIndexBallLeft]->GetWinTimer() > 2.0f)
		{
			this->mBalls[this->mIndexBallLeft]->AddWonRound();
			this->mBalls[this->mIndexBallLeft]->RestetWinTimer();
			
			// winner, show winner text for 2 seconds
			string s = "Winner: Player " + MaloW::convertNrToString(this->mIndexBallLeft+1);
			this->mHud[4]->SetText(s);
			while(dt < 2000)
				dt += mGe->Update();
			this->mHud[4]->SetText("");
			this->mWinnerActivated = false;
			return true;
		}
	}
	else if(this->mWinnerActivated)
	{
		// draw, show draw text for 2 seconds
		string s = "Draw";
		this->mHud[4]->SetText(s);
		while(dt < 2000)
			dt += mGe->Update();
		this->mHud[4]->SetText("");
		this->mBalls[this->mIndexBallLeft]->RestetWinTimer();
		this->mWinnerActivated = false;
		return true;
	}
	else if(numberAlivePlayersThisRound == 0 )
	{
		// is if both the balls are below y = -6 in the same update
		// draw, show draw text for 2 seconds
		string s = "Draw in the same update!";
		this->mHud[4]->SetText(s);
		while(dt < 2000)
			dt += mGe->Update();
		this->mHud[4]->SetText("");
		return true;
	}
	return false;
}

void Knockout::ShowHud()
{
	string s;
	s = "Player 1: " + this->mBalls[0]->GetRoundsWonStr();
	this->mHud[0]->SetText(s);
	s = "Player 2: " + this->mBalls[1]->GetRoundsWonStr();
	this->mHud[1]->SetText(s);
	s = "Round: " + MaloW::convertNrToString(this->mRoundsPlayed+1) + " of " + MaloW::convertNrToString(this->mNumberOfRounds);
	this->mHud[2]->SetText(s);
	
	//show time elapsed
	float tmp = floor(this->mTimeElapsed * 10.0f) / 10.0f;
	this->mTimeElapsedText->SetText(MaloW::convertNrToString(tmp));
}

void Knockout::PlayRound(bool& roundsLeft)
{
	float diff;
	bool running = true;
	
	while(running)
	{
		diff = mGe->Update();
		
		this->mPe->Simulate();
		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
		{
			this->HideHud();
			roundsLeft = running = this->mIGM->Run();
			this->mQuitByMenu = !running;
		}
		/*
		this->mTimeElapsed += diff * 0.001f;

		this->InputKnockout(diff, running, roundsLeft);
		for(int i = 0; i < this->mNumberOfPlayers; i++)
			this->mBalls[i]->Update(diff);
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
		*/

		if(!this->mGe->isRunning())
		{
			roundsLeft = running = false;
			this->mQuitByMenu = !running;
		}
		this->ShowHud();
		if(this->checkWinConditions(diff))
			running = false;
	}

	this->mPe->ResetTimers();
}

void Knockout::InputKnockout(float diff, bool& running, bool& roundsLeft)
{
	// move ball 1
	if(mGe->GetKeyListener()->IsPressed('W'))
		mBalls[0]->AddForceForwardDirection(diff);	
	if(mGe->GetKeyListener()->IsPressed('S'))
		mBalls[0]->AddForceOppositeForwardDirection(diff);
	if(mGe->GetKeyListener()->IsPressed('A'))
		mBalls[0]->AddForceLeftOfForwardDirection(diff);	
	if(mGe->GetKeyListener()->IsPressed('D'))
		mBalls[0]->AddForceRightOfForwardDirection(diff);	
	
	// move ball 2
	if(mGe->GetKeyListener()->IsPressed(VK_LEFT))
		mBalls[1]->AddForceLeftOfForwardDirection(diff);	
	if(mGe->GetKeyListener()->IsPressed(VK_RIGHT))
		mBalls[1]->AddForceRightOfForwardDirection(diff);	
	if(mGe->GetKeyListener()->IsPressed(VK_UP))
		mBalls[1]->AddForceForwardDirection(diff);	
	if(mGe->GetKeyListener()->IsPressed(VK_DOWN))
		mBalls[1]->AddForceOppositeForwardDirection(diff);

	if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
	{
		this->HideHud();
		roundsLeft = running = this->mIGM->Run();
		this->mQuitByMenu = !running;
	}
}

void Knockout::HideHud()
{
	for(int i = 0;i<6;i++)
		this->mHud[i]->SetText("");
	this->mTimeElapsedText->SetText("");
}