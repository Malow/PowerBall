#include "GameManager.h"
//#include "GraphicsEngine\MaloWLib\TRDCamera.h"


GameManager::GameManager(GraphicsEngine* ge)
{
	this->mNumPlayers	= 0;
	this->mRounds		= 0;
	this->mPlatform		= NULL;
	this->mBalls		= NULL;
	this->mGe			= ge;
	this->mNet			= NULL;
	this->mIGM			= NULL;
	this->mGameMode		= NULL;
	this->mEnemyFlag	= NULL;
	this->mFriendlyFlag	= NULL;
	counter				= 0.0f;
	this->mNet			= new GameNetwork();
}
GameManager::~GameManager()
{
	// removing the dynamically allocated memory
	SAFE_DELETE(this->mPlatform);
	for (int i = 0; i < this->mNumPlayers; i++) 
	{
		SAFE_DELETE(this->mBalls[i]);
    }
	SAFE_DELETE_ARRAY(this->mBalls);
	SAFE_DELETE(this->mNet);
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
	SAFE_DELETE(this->mIGM);
	if((this->mEnemyFlag))
		this->mGe->DeleteStaticMesh(this->mEnemyFlag->GetMesh());
	if((this->mFriendlyFlag))
		this->mGe->DeleteStaticMesh(this->mFriendlyFlag->GetMesh());
		
	if(this->mEnemyFlag)
		SAFE_DELETE(this->mEnemyFlag);
	if(this->mFriendlyFlag)
		SAFE_DELETE(this->mFriendlyFlag);
}

bool GameManager::Play(const int numPlayers)
{
	this->mGameMode = DM;
	this->mNumPlayers = numPlayers;
	this->mRounds = 3;
	int roundsPlayed = 0;
	this->Initialize();
	bool running = true;
	bool zoomOutPressed = false;
	bool zoomInPressed = false;
	counter = 0;
	
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
			s = "Player1: Rounds won: " + this->mBalls[0]->GetRoundsWonStr();
			hudR1->SetText(s);
			s = "Player2: Rounds won: " + this->mBalls[1]->GetRoundsWonStr();
			hudR2->SetText(s);
			s = "Round: " + MaloW::convertNrToString(roundsPlayed) + " of " + MaloW::convertNrToString(this->mRounds);
			hudR3->SetText(s);
			this->InputKnockout(diff,zoomOutPressed,zoomInPressed, running, roundsLeft, quitByMenu);
			
			for(int i = 0; i < this->mNumPlayers; i++)
			{
				this->mBalls[i]->Update(diff);
				if(this->mBalls[i]->IsAlive())
				{
					indexBallLeft = i;
					numberAlivePlayersThisRound += 1;
				}
			}
		
			for(int i = 0; i < this->mNumPlayers; i++)
			{
				Ball* b1 = this->mBalls[i];
				for(int j = i+1; j < this->mNumPlayers; j++)
				{
					Ball* b2 = this->mBalls[j];
					if(b1->collisionWithSphereSimple(b2))
						b1->collisionSphereResponse(b2);
				}
				
				Vector3 normalPlane;
				if(b1->collisionWithPlatformSimple(this->mPlatform,normalPlane))
					b1->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
				
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

		}
		if(roundsPlayed == this->mRounds)
			roundsLeft = false;
		else
		{
			this->mPlatform->Reset();
			for(int i = 0; i<this->mNumPlayers;i++)
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
	if(!quitByMenu)
	{
		int indexWinner = -1;
		int bestResault = 0;
		if(this->mNumPlayers >0)
		{
			bestResault = this->mBalls[0]->GetRoundsWon();
			indexWinner = 0;
		}
		for(int i = 1; i<this->mNumPlayers;i++)
		{
			if(this->mBalls[i]->GetRoundsWon() > bestResault)
			{
					bestResault = this->mBalls[i]->GetRoundsWon();
					indexWinner = i;
			}
		}
		
		
		string win = "Winner: Player " + MaloW::convertNrToString(indexWinner+1) + " with " + MaloW::convertNrToString(bestResault) + " won";
		Text* winner = mGe->CreateText(win,D3DXVECTOR2(250,300),2.5f,"Media/Fonts/1");
		diff = mGe->Update();
		while(diff < 2000)
			diff += mGe->Update();
	
		mGe->DeleteText(winner);
	}
	//returns to menu after some win/draw screen.
	return true;
}
bool GameManager::PlayLAN(ServerInfo server)
{
	this->mGameMode = server.GetGameMode();
	this->mNumPlayers = 0;
	this->Initialize();
	bool running = true;

	this->mNet->Start(server);
	this->mGe->Update();
	
	
	while(running)
	{
		int numAlivePlayers = 0;
		float diff = mGe->Update();	

		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
			running = this->mIGM->Run();
		

		if(!this->mNet->Update(this->mBalls, this->mNumPlayers, diff))
			running = false;

		if(this->mNet->IsServer())
		{
			for(int i = 0; i < this->mNumPlayers; i++)
				this->mBalls[i]->Update(diff);
			// will be moved to phisics simulation class
			for(int i = 0; i < this->mNumPlayers; i++)
			{
				if(this->mPlatform->IsOnPlatform(mBalls[i]->GetPosition().x, mBalls[i]->GetPosition().z))
				{
					if(i != this->mNet->GetIndex())
					{
						int flip = 1;
						if(this->mNet->GetStartPos(i).z > 0)
							flip = -1;
						if(this->mNet->IsKeyPressed('A', i))
							mBalls[i]->AddForce(Vector3(-diff * flip,0,0));	
						if(this->mNet->IsKeyPressed('D', i))
							mBalls[i]->AddForce(Vector3(diff * flip,0,0));
						if(this->mNet->IsKeyPressed('W', i))
							mBalls[i]->AddForce(Vector3(0,0,diff * flip));	
						if(this->mNet->IsKeyPressed('S', i))
							mBalls[i]->AddForce(Vector3(0,0,-diff * flip));
						if(this->mNet->IsKeyPressed(VK_SPACE, i))
							mBalls[i]->AddForce(Vector3(0, diff,0));
					}
					else
					{
						if(mGe->GetKeyListener()->IsPressed('A'))
							mBalls[i]->AddForce(Vector3(-diff,0,0));	
						if(mGe->GetKeyListener()->IsPressed('D'))
							mBalls[i]->AddForce(Vector3(diff,0,0));
						if(mGe->GetKeyListener()->IsPressed('W'))
							mBalls[i]->AddForce(Vector3(0,0,diff));	
						if(mGe->GetKeyListener()->IsPressed('S'))
							mBalls[i]->AddForce(Vector3(0,0,-diff));

						if(mGe->GetKeyListener()->IsPressed(VK_SPACE))
							mBalls[i]->AddForce(Vector3(0,diff,0));
					}	
					Ball* b1 = this->mBalls[i];
					for(int j = i+1; j < this->mNumPlayers; j++)
					{
						Ball* b2 = this->mBalls[j];
						if(b1->collisionWithSphereSimple(b2))
							b1->collisionSphereResponse(b2);

					}
					Vector3 normalPlane;
					if(b1->collisionWithPlatformSimple(this->mPlatform,normalPlane))
						b1->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
				}

				
				


				
				//if(this->mBalls[i]->GetPosition().y < 14.7f && this->mPlatform->IsOnPlatform(this->mBalls[i]->GetPosition().x, this->mBalls[i]->GetPosition().z))
					//this->mBalls[i]->SetPosition(this->mBalls[i]->GetPosition().x, 14.7f, this->mBalls[i]->GetPosition().z);

			}
			for(int i = 0; i < this->mNumPlayers; i++)
			{
				this->mNet->SetPos(this->mBalls[i]->GetPosition(), i);
				Vector3 vel = this->mBalls[i]->GetVelocity();
				this->mNet->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z),  i);
			}
		}
		else 
		{
			this->mNet->AddKeyInput('A', mGe->GetKeyListener()->IsPressed('A'));
			this->mNet->AddKeyInput('D', mGe->GetKeyListener()->IsPressed('D'));
			this->mNet->AddKeyInput('W', mGe->GetKeyListener()->IsPressed('W'));
			this->mNet->AddKeyInput('S', mGe->GetKeyListener()->IsPressed('S'));
			this->mNet->AddKeyInput(VK_SPACE, mGe->GetKeyListener()->IsPressed(VK_SPACE));

			if(this->mNet->GetIndex() < this->mNumPlayers)
			{
				
				int flip = 1;
				if(this->mNet->GetStartPos(this->mNet->GetIndex()).z > 0)
					flip = -1;
				if(mGe->GetKeyListener()->IsPressed('A'))
					mBalls[this->mNet->GetIndex()]->AddForce(Vector3(-diff*flip,0,0));	
				if(mGe->GetKeyListener()->IsPressed('D'))
					mBalls[this->mNet->GetIndex()]->AddForce(Vector3(diff*flip,0,0));
				if(mGe->GetKeyListener()->IsPressed('W'))
					mBalls[this->mNet->GetIndex()]->AddForce(Vector3(0,0,diff*flip));	
				if(mGe->GetKeyListener()->IsPressed('S'))
					mBalls[this->mNet->GetIndex()]->AddForce(Vector3(0,0,-diff*flip));

				/*this->mBalls[this->mNet->GetIndex()]->Update(diff, this->mPlatform);

				Ball* b1 = this->mBalls[this->mNet->GetIndex()];

				for(int i = 0; i < this->mNumPlayers; i++)
				{
					if(i != this->mNet->GetIndex())
					{
						Ball* b2 = this->mBalls[i];
						if(b1->collisionWithSphereSimple(b2))
							b1->collisionSphereResponse(b2, diff);
					}

				}

				Vector3 normalPlane;
				if(b1->collisionWithPlatformSimple(this->mPlatform,normalPlane))
					b1->collisionPlatformResponse(this->mPlatform, normalPlane, diff);*/
				for(int i = 0; i < this->mNumPlayers; i++)
					this->mBalls[i]->Update(diff);

				for(int i = 0; i < this->mNumPlayers; i++)
				{
					Ball* b1 = this->mBalls[i];
					for(int j = i+1; j < this->mNumPlayers; j++)
					{
						Ball* b2 = this->mBalls[j];
						if(b1->collisionWithSphereSimple(b2))
							b1->collisionSphereResponse(b2);

					}
					Vector3 normalPlane;
					if(b1->collisionWithPlatformSimple(this->mPlatform,normalPlane))
						b1->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
				}
			}
		}
		for(int i = 0; i < this->mNumPlayers; i++)
		{
			if(this->mBalls[i]->IsAlive())
				numAlivePlayers += 1;
		}
		mPlatform->Update(diff);



		if(this->mNet->GetNumPlayers() > this->mNumPlayers)
		{
			this->AddBall();
			numAlivePlayers++;
		}

		if(this->mNet->IsServer())
		if((numAlivePlayers == 1 && this->mNet->GetNumPlayers() > 1) || numAlivePlayers < 1)
		{
			running = false;
		}
		if(!this->mGe->isRunning())
			running = false;
		if(this->mGameMode == CTF)
		{
			if(this->mNet->IsServer())
			{
				if(!this->CaptureTheFlag())
					running = false;
			}
			else
			{
				this->mEnemyFlag->GetMesh()->SetPosition(this->mNet->GetFlagPos(0));
				this->mFriendlyFlag->GetMesh()->SetPosition(this->mNet->GetFlagPos(1));
			}
		}
		if(this->mGameMode == KOTH || this->mGameMode == KOTH2)
			if(!this->KingOfTheHill(diff))
				running = false;
	}
	this->mNet->Close();
	//returns to menu after some win/draw screen.
	return true;
}
void GameManager::Initialize()
{
	D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,10,0);
	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 30, -15));
	mGe->GetCamera()->LookAt(centerPlatform);
	//Image* testImg = mGe->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
	this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
	this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
	this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
	this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
	for(int i = 0; i < 5; i++)
		this->mLights[i]->SetIntensity(30.0f);

	this->mIGM			= new InGameMenu(this->mGe);

	//replace with MAP->GETSTARTPOSITIONS() ?
	//
	/*this->mNet->SetStartPos(D3DXVECTOR3(0,24.7f,-20), 0);
	this->mNet->SetStartPos(D3DXVECTOR3(0,24.7f,20), 1);
	this->mNet->SetStartPos(D3DXVECTOR3(-5,24.7f,-20), 2);
	this->mNet->SetStartPos(D3DXVECTOR3(-5,24.7f,20), 3);*/

	/**
	* King of the hill
	**/
	if(this->mGameMode == KOTH || this->mGameMode == KOTH2)
	{
		D3DXVECTOR3 startPositions[4];
		startPositions[0] = D3DXVECTOR3(0,24.7f,-14);
		startPositions[1] = D3DXVECTOR3(0,24.7f,14);
		startPositions[2] = D3DXVECTOR3(0,24.7f,-12);
		startPositions[3] = D3DXVECTOR3(0,24.7f,12);
	
		this->mNet->SetStartPosistions(startPositions, 4);

		this->mRounds = 3;
		this->mPlatform		= new Platform("Media/KOTHMap1.obj", centerPlatform);
		this->mPlatform->SetShrinkValue(0.0f);
		Vector3 hotZone = Vector3(0.0f,20.2f,0.0f);
		this->mPlatform->SetHotZonePosition(hotZone);
		float radius = 5.4f;
		this->mPlatform->SetHotZoneRadius(radius);
		float maxTime;
		if(this->mGameMode == KOTH)
			maxTime = 10.0f;
		else
			maxTime = 30.0f;
		this->mPlatform->SetMaxTimeInHotZone(maxTime);

		this->mBalls		= new Ball*[this->mNumPlayers];

		for(int i = 0; i < this->mNumPlayers; i++)
		{
			if( i == 0)
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,-14));
			else
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,14));
		}
		
	}
	/**
	* Capture The Flag
	**/
	else if(this->mGameMode == CTF)
	{
		D3DXVECTOR3 startPositions[4];
		startPositions[0] = D3DXVECTOR3(0,24.7f,-20);
		startPositions[1] = D3DXVECTOR3(0,24.7f,20);
		startPositions[2] = D3DXVECTOR3(-5,24.7f,-20);
		startPositions[3] = D3DXVECTOR3(-5,24.7f,20);
	
		this->mNet->SetStartPosistions(startPositions, 4);

		this->mRounds = 3;
		this->mPlatform		= new Platform("Media/CTFMap1.obj", centerPlatform);
		this->mPlatform->SetShrinkValue(0.0f);
		this->mBalls		= new Ball*[this->mNumPlayers];

		for(int i = 0; i < this->mNumPlayers; i++)
		{
			if( i == 0)
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,-10));
			else
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,10));
		}
		this->mEnemyFlag = new Flag(mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(0, 20, 25)), D3DXVECTOR3(0, 20, 25));
		this->mFriendlyFlag = new Flag(mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(0, 20, -25)), D3DXVECTOR3(0, 20, 25));
	}
	/**
	* DeathMatch
	**/
	else if(this->mGameMode == DM)
	{
		this->mPlatform		= new Platform("Media/Cylinder.obj", centerPlatform);
		this->mBalls		= new Ball*[this->mNumPlayers];
		for(int i = 0; i < this->mNumPlayers; i++)
		{
			if( i == 0)
			{
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,25.0f,-5));
				this->mBalls[i]->SetKnockoutMode();
			}
			else
			{
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,25.0f,5));
				this->mBalls[i]->SetKnockoutMode();
			}
		}
	}
	/* WarLock
	**/
	else if(this->mGameMode == TESTW)
	{
		
		D3DXVECTOR3 startPositions[4];
		startPositions[0] = D3DXVECTOR3(0,30.0f,-10);
		startPositions[1] = D3DXVECTOR3(0,30.0f,10);
		startPositions[2] = D3DXVECTOR3(-10,30.0f,0);
		startPositions[3] = D3DXVECTOR3(10,30.0f,0); 
		
		this->mPlatform		= new Platform("Media/Cylinder.obj", centerPlatform);
		this->mPlatform->SetScale(Vector3(3,3,3));
		this->mBalls		= new Ball*[this->mNumPlayers];
		this->mPlatform->SetShrinkValue(0.0f);
		
		for(int i = 0; i < this->mNumPlayers; i++)
		{
			if(i == 0)
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,-10.0f));
			else if(i == 1)
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,10.0f));
			else if(i == 2)
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(10.0f,30.0f,0.0f));
			else if(i == 3)
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(-10.0f,30.0f,0.0f));
		}
		this->mBalls[0]->AddSpell(new ChargeSpell());
		this->mBalls[0]->AddSpell(new SprintSpell());
		this->mBalls[0]->AddSpell(new HardenSpell());
		this->mBalls[0]->AddSpell(new InvisibilitySpell());
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[0]);
		
	}
	// wait until everything is loaded and then drop the balls from hight above
	mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
	/*
	float diff = mGe->Update();
	while(diff < 1000)
		diff += mGe->Update();
	*/
}
void GameManager::AddBall()
{
	
	int old = this->mNumPlayers;
	this->mNumPlayers = this->mNet->GetNumPlayers();
	Ball** temp = new Ball*[this->mNumPlayers];
	for(int i = 0; i < old; i++)
	{
		temp[i] = this->mBalls[i];
	}
	for(int i = old; i < this->mNumPlayers; i++)
	{
		temp[i] = new Ball("Media/Ball.obj", this->mNet->GetStartPos(i));
	}
	delete[] this->mBalls;
	this->mBalls = temp;

	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 40, this->mNet->GetStartPos(this->mNet->GetIndex()).z * 1.5f));
	mGe->GetCamera()->LookAt(D3DXVECTOR3(0,10,0));
}
bool GameManager::CaptureTheFlag()
{
	D3DXVECTOR3 BallToFlag = D3DXVECTOR3((this->mEnemyFlag->GetMesh()->GetPosition() + D3DXVECTOR3(0, this->mBalls[0]->GetRadius(), 0))- this->mBalls[0]->GetPosition());

	if(D3DXVec3Length(&BallToFlag) < (this->mBalls[0]->GetRadius()))
	{
		this->mBalls[0]->AddFlag(this->mEnemyFlag);
		this->mEnemyFlag->SetAtBase(false);
	}
	if(!this->mEnemyFlag->GetAtBase())
	{
		D3DXVECTOR3 distBetweenFlags = D3DXVECTOR3(this->mEnemyFlag->GetMesh()->GetPosition() - this->mFriendlyFlag->GetMesh()->GetPosition());
		if(D3DXVec3Length(&distBetweenFlags) < (this->mBalls[0]->GetRadius()) && this->mFriendlyFlag->GetAtBase())
		{
			this->mBalls[0]->ResetFlag();
			this->mEnemyFlag->Reset();
			this->mRounds--;
			if(this->mRounds <= 0)
				return false;
		}
	}


	//just copied temporarily to try
	if(this->mNumPlayers > 1)
	{
		D3DXVECTOR3 BallToFlag = D3DXVECTOR3((this->mFriendlyFlag->GetMesh()->GetPosition() + D3DXVECTOR3(0, this->mBalls[1]->GetRadius(), 0))- this->mBalls[1]->GetPosition());

		if(D3DXVec3Length(&BallToFlag) < (this->mBalls[1]->GetRadius()))
		{
			this->mBalls[1]->AddFlag(this->mFriendlyFlag);
			this->mFriendlyFlag->SetAtBase(false);
		}
		if(!this->mFriendlyFlag->GetAtBase())
		{
			D3DXVECTOR3 distBetweenFlags = D3DXVECTOR3(this->mFriendlyFlag->GetMesh()->GetPosition() - this->mEnemyFlag->GetMesh()->GetPosition());
			if(D3DXVec3Length(&distBetweenFlags) < (this->mBalls[1]->GetRadius()) && this->mEnemyFlag->GetAtBase())
			{
				this->mBalls[1]->ResetFlag();
				this->mFriendlyFlag->Reset();
				this->mRounds--;
				if(this->mRounds <= 0)
					return false;
			}
		}
	}
	this->mNet->SetFlagPos(this->mFriendlyFlag->GetMesh()->GetPosition(), 0);
	this->mNet->SetFlagPos(this->mEnemyFlag->GetMesh()->GetPosition(), 1);
	return true;
}
bool GameManager::KingOfTheHill(float dt)
{
	float newdt = dt/1000.0f;
	int numberOfPlayers = this->mNumPlayers;
	int numberInHotZone = 0;
	int ballIndex = 0;
	for(int i = 0; i<numberOfPlayers; i++)
	{
		if(this->mPlatform->IsInHotZone(this->mBalls[i]->GetPositionVector3(), this->mBalls[i]->GetRadius()))
		{
			numberInHotZone++;
			ballIndex = i;
		}
	}
	if(numberInHotZone == 1)
	{
		this->mBalls[ballIndex]->AddTimeInHotZone(newdt);
		if( this->mBalls[ballIndex]->GetTimeInHotZone() >= this->mPlatform->GetMaxTimeInHotZone())
		{
			this->mRounds--;
			for(int i = 0; i<numberOfPlayers; i++)
			{
				this->mBalls[i]->ResetTime();
				this->mBalls[i]->SetPositionToStartPosition();
				this->mNet->SetPos(this->mBalls[i]->GetPosition(), i);
				Vector3 vel = Vector3(0,0,0);
				this->mNet->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z),  i);
			}										
			if(this->mRounds <= 0)
					return false;

		}
	}
	else
	{
		if( this->mGameMode == KOTH)
			for(int i = 0; i<numberOfPlayers; i++)
				this->mBalls[i]->ResetTime();
	}
	return true;
}

bool GameManager::WarLock(float dt)
{
	float newdt = dt/1000.0f;
	return true;
}

void GameManager::InputKnockout(float diff, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& roundsLeft, bool& quitByMenu )
{
	if(mGe->GetEngineParameters().CamType == RTS)
		{
			if(mGe->GetKeyListener()->IsPressed('A'))
				mBalls[0]->AddForce(Vector3(- diff,0,0));	
			if(mGe->GetKeyListener()->IsPressed('D'))
				mBalls[0]->AddForce(Vector3(diff,0,0));
			if(mGe->GetKeyListener()->IsPressed('W'))
				mBalls[0]->AddForce(Vector3(0,0,diff));	
			if(mGe->GetKeyListener()->IsPressed('S'))
				mBalls[0]->AddForce(Vector3(0,0,-diff));
			if(mGe->GetKeyListener()->IsClicked(2))
				mBalls[0]->AddForce(Vector3(0,diff*(11.0f/6.0f),0));
		}
		else if(mGe->GetEngineParameters().CamType == TRD)
		{
			if(mGe->GetKeyListener()->IsPressed('W'))
				mBalls[0]->AddForceForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsPressed('S'))
				mBalls[0]->AddForceOppositeForwardDirection(diff);
			if(mGe->GetKeyListener()->IsPressed('Q'))
				mBalls[0]->RotateForwardLeft(diff);
			if(mGe->GetKeyListener()->IsPressed('E'))
				mBalls[0]->RotateForwardRight(diff);
			if(mGe->GetKeyListener()->IsClicked(2))
				mBalls[0]->AddForce(Vector3(0,diff*(11.0f/6.0f),0));
			if(mGe->GetKeyListener()->IsPressed('A'))
				mBalls[0]->AddForceLeftOfForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsPressed('D'))
				mBalls[0]->AddForceRightOfForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsPressed('1'))
				mBalls[0]->UseSpell(1);
			if(mGe->GetKeyListener()->IsPressed('2'))
				mBalls[0]->UseSpell(2);
			if(mGe->GetKeyListener()->IsPressed('3'))
				mBalls[0]->UseSpell(3);
			if(mGe->GetKeyListener()->IsPressed('Z') && !zoomOutPressed)
			{
				mBalls[0]->ZoomOut();
				zoomOutPressed = true;
			}
			else if(!mGe->GetKeyListener()->IsPressed('Z'))
				zoomOutPressed = false;
			if(mGe->GetKeyListener()->IsPressed('C') && !zoomInPressed)
			{
				mBalls[0]->ZoomIn();
				zoomInPressed = true;
			}
			else if(!mGe->GetKeyListener()->IsPressed('C'))
				zoomInPressed = false;
		}
		

		// move ball 2
		if(mGe->GetKeyListener()->IsPressed('H'))
			mBalls[1]->AddForce(Vector3(-diff,0,0));	
		if(mGe->GetKeyListener()->IsPressed('K'))
			mBalls[1]->AddForce(Vector3(diff,0,0));
		if(mGe->GetKeyListener()->IsPressed('U'))
			mBalls[1]->AddForce(Vector3(0,0,diff));	
		if(mGe->GetKeyListener()->IsPressed('J'))
			mBalls[1]->AddForce(Vector3(0,0,-diff));

		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
		{
			roundsLeft = running = this->mIGM->Run();
			quitByMenu = !running;
		}

}