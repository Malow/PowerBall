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

bool GameManager::Play(const int numPlayers, int lifes, int rounds)
{
	if(rounds < 1)
		return true;
	this->mGameMode = DM;
	this->mNumPlayers = numPlayers;
	this->mRounds = rounds;
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
	bool roundsLeft;
	if(this->mRounds >0)
		roundsLeft = true;
	else
		roundsLeft = false;
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
			for(int i = 0; i<this->mNumPlayers;i++)
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
	if(!quitByMenu && this->mRounds > 0)
	{
		int indexWinner = -1;
		int bestResault = 0;
		if(this->mNumPlayers >0)
		{
			bestResault = this->mBalls[0]->GetRoundsWon();
			indexWinner = 0;
		}
		bool draw = false;
		for(int i = 1; i<this->mNumPlayers;i++)
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
	//returns to menu after some win/draw screen.
	return true;
}
bool GameManager::PlayLAN(ServerInfo server)
{
	this->mGameMode = server.GetGameMode();
	this->mNumPlayers = 0;
	this->Initialize();
	bool running = true;
	bool zoomOutPressed = false;
	bool zoomInPressed = false;
	bool quitByMenu = false;
	this->mNet->Start(server);
	this->mGe->Update();
	int numAlivePlayers = 0;
	float warlockTimer = 0;
	Text* hudR1 = mGe->CreateText("",D3DXVECTOR2(20,20),2.0f,"Media/Fonts/1");
	string s;

	
	LARGE_INTEGER oldTick = LARGE_INTEGER();
	LARGE_INTEGER now =  LARGE_INTEGER();


	while(running)
	{
		float diff = mGe->Update(); //A problem when the user opens up ingame menu is that the diff after resume is incredibly high so it breaks game logic, game gotta continue in the background if network :P	

		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
			running = this->mIGM->Run();
		
		QueryPerformanceCounter(&now);
		LARGE_INTEGER proc_freq;
		::QueryPerformanceFrequency(&proc_freq);
		double frequency = proc_freq.QuadPart;

		//diff = 1000*((now.QuadPart - oldTick.QuadPart) / frequency); //2			WITH A VARIABLE DELTATIME THE BALL PHYSICS RESULT DIFFER IF MORE THAN TWO CLIENTS WITH DIFFERENT DELTA TIMES PROCESS EXACTLY THE SAME INPUT, SETTING A CONSTANT DELTATIME HOWEVER LEADS TO THE SAME PHYSICS RESULT (THOUGH WITH A HUGE DELAY DUE TO THE CLIENT IN THE BACKGROUND IS A ASSIGNED LESS CPU TIME (-> low FPS)). IS THERE SOMETHING IN BALL PHYSICS THAT SHOULD BE DEPENDANT ON DELTATIME THAT ISNT?//
		QueryPerformanceCounter(&oldTick);

		if(this->mNet->IsServer())
		{

			// will be moved to phisics simulation class
			for(int i = 0; i < this->mNumPlayers; i++)
			{
				if(i != this->mNet->GetIndex())
				{
					this->mBalls[i]->SetForwardVector(this->mNet->GetForwardVector(i));
					this->HandleClientKeyInputs(i, diff);
				}
				else
				{
					this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
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
				for(int i = 0; i < this->mNumPlayers; i++)
					this->mBalls[i]->UpdatePost();
	
			}
			
			for(int i = 0; i < this->mNumPlayers; i++)
				this->mBalls[i]->Update(diff); //split up due to the balls affecting each other, so cant send final position until all balls updated

			for(int i = 0; i < this->mNumPlayers; i++)
			{
				this->mNet->SetPos(this->mBalls[i]->GetPosition(), i);
				Vector3 vel = this->mBalls[i]->GetVelocity();
				this->mNet->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z),  i);
			}
		}
		else //is client
		{
			for(int i = 0; i < this->mNumPlayers; i++)
			{
				if(this->mNet->GetIndex() != i)
				{
					D3DXVECTOR3 rotVector = this->mNet->GetPos(i) - this->mBalls[i]->GetPosition();
					this->mBalls[i]->SetPosition(this->mNet->GetPos(i));
					this->mBalls[i]->Rotate(rotVector);
				}
			}
			if(this->mNet->GetIndex() < this->mNumPlayers)
			{
				int i = this->mNet->GetIndex();

				this->SendKeyInputs(i, diff);
				this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
				
				for(int c = 0; c < this->mNumPlayers; c++)
				{
					Ball* b1 = this->mBalls[c];
					for(int j = c+1; j < this->mNumPlayers; j++)
					{
						Ball* b2 = this->mBalls[j];
						if(b1->collisionWithSphereSimple(b2))
							b1->collisionSphereResponse(b2);

					}
				}

				Vector3 normalPlane;
				if(this->mBalls[i]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
					this->mBalls[i]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
				for(int i = 0; i < this->mNumPlayers; i++)
					this->mBalls[i]->UpdatePost();

				this->mBalls[i]->Update(diff);
				

				this->mNet->AddMovement(this->mBalls[i]);
				Vector3 temp = this->mBalls[i]->GetForwardVector();
				this->mNet->SetForwardVector(D3DXVECTOR3(temp.x, temp.y, temp.z), i);

			}
		}

		for(int i = 0; i < this->mNumPlayers; i++)
		{
			if(this->mBalls[i]->IsAlive())
				numAlivePlayers += 1;
		}
		mPlatform->Update(diff);

		
		if(!this->mNet->Update(this->mBalls, this->mNumPlayers, diff))
			running = false;

		if(this->mNet->GetNumPlayers() > this->mNumPlayers)
		{
			if(this->mGameMode == WARLOCK)
				this->AddBallWarlock();
			else
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
		if(this->mGameMode == WARLOCK)
		{
			float tmp = (600.0f - warlockTimer);
			tmp = floor(tmp * 10.0f) / 10.0f;
			s = "Timer: " + MaloW::convertNrToString(tmp);
			hudR1->SetText(s);
			if(!this->WarLock(diff, warlockTimer))
				running = false;
			
		}
	}
	mGe->DeleteText(hudR1);
	this->mNet->Close();
	//returns to menu after some win/draw screen.
	return true;
}

bool GameManager::PlayCredits()
{
	bool running = true;
	this->mGameMode = CREDITS;
	this->mNumPlayers = 1;
	this->Initialize();
	float diff;
	Text* hudR1 = mGe->CreateText("",D3DXVECTOR2(20,20),1.0f,"Media/Fonts/1");
	Text* hudR2 = mGe->CreateText("",D3DXVECTOR2(20,80),1.0f,"Media/Fonts/1");
	Text* hudR3 = mGe->CreateText("",D3DXVECTOR2(20,140),1.0f,"Media/Fonts/1");
	Text* hudR4 = mGe->CreateText("",D3DXVECTOR2(20,200),1.0f,"Media/Fonts/1");
	Text* hudR5 = mGe->CreateText("",D3DXVECTOR2(20,260),1.0f,"Media/Fonts/1");
	Text* hudR6 = mGe->CreateText("",D3DXVECTOR2(20,560),1.0f,"Media/Fonts/1");
	Text* hudR7 = mGe->CreateText("",D3DXVECTOR2(20,600),1.0f,"Media/Fonts/1");
	Text* hudR8 = mGe->CreateText("",D3DXVECTOR2(20,640),1.0f,"Media/Fonts/1");
	Text* hudR9 = mGe->CreateText("",D3DXVECTOR2(20,680),1.0f,"Media/Fonts/1");
	Text* hudR10 = mGe->CreateText("",D3DXVECTOR2(200,400),2.0f,"Media/Fonts/1");
	string s;
	s = "Credits: Random";
	hudR1->SetText(s);
	diff = mGe->Update();
	srand ( time(NULL) );
	float targetX = -10 + rand() % 21; // random angle [-10, 10] in x-axis
	float targetZ = -10 + rand() % 21; // random angle [-10, 10] in z-axis
	targetX = targetX*(PI/180.0f);
	targetZ = targetZ*(PI/180.0f);
	this->mPlatform->SetTargetAngleX(targetX);
	this->mPlatform->SetTargetAngleZ(targetZ);
	while(running)
	{
		
		diff = mGe->Update();
		if(this->mPlatform->IsTargetAngleReachedX())
		{
			targetX = -10 + rand() % 21;
			targetX = targetX*(PI/180.0f);
			this->mPlatform->SetTargetAngleX(targetX);
		}
		if(this->mPlatform->IsTargetAngleReachedZ())
		{
			targetZ = -10 + rand() % 21;
			targetZ = targetZ*(PI/180.0f);
			this->mPlatform->SetTargetAngleZ(targetZ);
		}
		
		
		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
			running = false;
		
		if(mGe->GetKeyListener()->IsPressed('W'))
			mBalls[0]->AddForceForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('S'))
			mBalls[0]->AddForceOppositeForwardDirection(diff);
		if(mGe->GetKeyListener()->IsPressed('A'))
			mBalls[0]->AddForceLeftOfForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('D'))
			mBalls[0]->AddForceRightOfForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsClicked(2))
			mBalls[0]->AddForce(Vector3(0,diff*(11.0f/6.0f),0));
		mBalls[0]->Update(diff);
		Vector3 normalPlane;
		if(this->mBalls[0]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
				this->mBalls[0]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
		mBalls[0]->UpdatePost();
		mPlatform->Update(diff);
		if(!this->mGe->isRunning())
			running = false;
		if(!mBalls[0]->IsAlive())
			running = false;
		
		
		
		s = "Target X = " + MaloW::convertNrToString(this->mPlatform->GetTargetAngleX()*(180.0f/PI));
		hudR2->SetText(s);
		s = "Current X = " + MaloW::convertNrToString(floor(this->mPlatform->GetAngleX()*(180.0f/PI)));
		hudR3->SetText(s);
		
		s = "Target Z = " + MaloW::convertNrToString(this->mPlatform->GetTargetAngleZ()*(180.0f/PI));
		hudR4->SetText(s);
		
		s = "Current Z = " + MaloW::convertNrToString(floor(this->mPlatform->GetAngleZ()*(180.0f/PI)));
		hudR5->SetText(s);
		
		s = "Position ball:";
		hudR6->SetText(s);
		s = "X = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().x)/10.0f) + " Y = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().y)/10.0f) + " Z = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().z)/10.0f);
		hudR7->SetText(s);
		s = "Position hotzone flag: ";
		hudR8->SetText(s);
		s =  "X = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().x)/10.0f) + " Y = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().y)/10.0f) + " Z = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().z)/10.0f);
		hudR9->SetText(s);
		Vector3 p = this->mPlatform->GetMesh()->GetPosition();

		if(this->mPlatform->IsInHotZone(this->mBalls[0]->GetPositionVector3(), this->mBalls[0]->GetRadius()))
		{
			hudR2->SetText("");
			hudR3->SetText("");
			hudR4->SetText("");
			hudR5->SetText("");
			hudR6->SetText("");
			hudR7->SetText("");
			hudR8->SetText("");
			hudR9->SetText("");
			running = false;
			s = "Mission Accomplished: you rock!";
			hudR10->SetText(s);
			while(diff < 4000)
				diff += mGe->Update();
			hudR10->SetText("");
		}
		

	}
	mGe->DeleteText(hudR1);
	mGe->DeleteText(hudR2);
	mGe->DeleteText(hudR3);
	mGe->DeleteText(hudR4);
	mGe->DeleteText(hudR5);
	mGe->DeleteText(hudR6);
	mGe->DeleteText(hudR7);
	mGe->DeleteText(hudR8);
	mGe->DeleteText(hudR9);
	mGe->DeleteText(hudR10);
	return true;
}

bool GameManager::PlayCredits2()
{
	bool running = true;
	this->mGameMode = CREDITS2;
	this->mNumPlayers = 1;
	this->Initialize();
	float diff;
	Text* hudR1 = mGe->CreateText("",D3DXVECTOR2(20,20),1.0f,"Media/Fonts/1");
	Text* hudR2 = mGe->CreateText("",D3DXVECTOR2(20,140),1.0f,"Media/Fonts/1");
	Text* hudR3 = mGe->CreateText("",D3DXVECTOR2(20,180),1.0f,"Media/Fonts/1");
	Text* hudR4 = mGe->CreateText("",D3DXVECTOR2(20,500),1.0f,"Media/Fonts/1");
	Text* hudR5 = mGe->CreateText("",D3DXVECTOR2(20,540),1.0f,"Media/Fonts/1");
	Text* hudR6 = mGe->CreateText("",D3DXVECTOR2(20,580),1.0f,"Media/Fonts/1");
	Text* hudR7 = mGe->CreateText("",D3DXVECTOR2(20,620),1.0f,"Media/Fonts/1");
	Text* hudR8 = mGe->CreateText("",D3DXVECTOR2(20,660),1.0f,"Media/Fonts/1");
	Text* hudR9 = mGe->CreateText("",D3DXVECTOR2(20,700),1.0f,"Media/Fonts/1");
	Text* hudR10 = mGe->CreateText("",D3DXVECTOR2(200,400),2.0f,"Media/Fonts/1");
	string s;
	s = "Credits: OldStyle";
	hudR1->SetText(s);
	this->mPlatform->SetMaxAngleX(10.0f*(PI/180.0f));
	this->mPlatform->SetMaxAngleZ(10.0f*(PI/180.0f));
	diff = mGe->Update();
	
	while(running)
	{
		
		diff = mGe->Update();
			
		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
			running = false;
		
		if(mGe->GetKeyListener()->IsPressed('W'))
			this->mPlatform->RotateX(diff);
		if(mGe->GetKeyListener()->IsPressed('S'))
			this->mPlatform->RotateX(-diff);
		if(mGe->GetKeyListener()->IsPressed('A'))
			this->mPlatform->RotateZ(diff);
		if(mGe->GetKeyListener()->IsPressed('D'))
			this->mPlatform->RotateZ(-diff);
		if(mGe->GetKeyListener()->IsClicked(2))
			mBalls[0]->AddForce(Vector3(0,diff*(11.0f/6.0f),0));
		mBalls[0]->Update(diff);
		Vector3 normalPlane;
		if(this->mBalls[0]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
				this->mBalls[0]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
		mBalls[0]->UpdatePost();
		mPlatform->Update(diff);
		if(!this->mGe->isRunning())
			running = false;
		
		
		
		s = "X = " + MaloW::convertNrToString(floor(this->mPlatform->GetAngleX()*(180.0f/PI)));
		hudR2->SetText(s);
		s = "Z = " + MaloW::convertNrToString(floor(this->mPlatform->GetAngleZ()*(180.0f/PI)));
		hudR3->SetText(s);
		
		
		s = "Position ball:";
		hudR4->SetText(s);
		s = "X = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().x)/10.0f) + " Y = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().y)/10.0f) + " Z = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().z)/10.0f);
		hudR5->SetText(s);
		s = "Position hotzone flag: ";
		hudR6->SetText(s);
		s =  "X = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().x)/10.0f) + " Y = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().y)/10.0f) + " Z = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().z)/10.0f);
		hudR7->SetText(s);
		//Vector3 distVec = this->mPlatform->GetHotZonePosition() - this-mBalls[0]->GetPositionVector3();
		//float dist = distVec.GetLength();
		//s = "Distance to flag: " +  MaloW::convertNrToString(floor(10.0f*dist)/10.0f);
		
		if(this->mPlatform->IsInHotZone(this->mBalls[0]->GetPositionVector3(), this->mBalls[0]->GetRadius()))
		{
			hudR2->SetText("");
			hudR3->SetText("");
			hudR4->SetText("");
			hudR5->SetText("");
			hudR6->SetText("");
			hudR7->SetText("");
			hudR8->SetText("");
			hudR9->SetText("");
			running = false;
			s = "Mission Accomplished: you rock!";
			hudR10->SetText(s);
			while(diff < 4000)
				diff += mGe->Update();
			hudR10->SetText("");
		}
	}
		mGe->DeleteText(hudR1);
		mGe->DeleteText(hudR2);
		mGe->DeleteText(hudR3);
		mGe->DeleteText(hudR4);
		mGe->DeleteText(hudR5);
		mGe->DeleteText(hudR6);
		mGe->DeleteText(hudR7);
		mGe->DeleteText(hudR8);

		return true;
	}

void GameManager::Initialize()
{
	D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,10,0);
	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 25, -10));
	mGe->GetCamera()->LookAt(centerPlatform);
	//Image* testImg = mGe->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
	this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
	this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
	this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
	this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
	for(int i = 0; i < 5; i++)
		this->mLights[i]->SetIntensity(30.0f);

	this->mIGM	= new InGameMenu(this->mGe);

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
		D3DXVECTOR3 forwardVectors[4];
		for(int i = 0; i<4;i++)
		{
			if(i%2 == 0)
				forwardVectors[i] = D3DXVECTOR3(0,0,1);
			else
				forwardVectors[i] = D3DXVECTOR3(0,0,-1);
		}
		/* this if we want all to use TRD-Camera and forward vector at center
		D3DXVECTOR3 center = D3DXVECTOR3(0,0,0);
		for(int i = 0; i<4;i++)
		{
			D3DXVECTOR3 startPosXZ = D3DXVECTOR3(startPositions[i].x, 0, startPositions[i].z);
			D3DXVECTOR3 temp = center - startPosXZ;
			D3DXVECTOR3 tempN;
			D3DXVec3Normalize(&tempN, &temp);
			forwardVectors[i] = tempN;
		}
		*/
		this->mNet->SetForwardVectors(forwardVectors, 4);

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

		D3DXVECTOR3 forwardVectors[4];
		for(int i = 0; i<4;i++)
		{
			if(i%2 == 0)
				forwardVectors[i] = D3DXVECTOR3(0,0,1);
			else
				forwardVectors[i] = D3DXVECTOR3(0,0,-1);
		}
		/* this if we want all to use TRD-Camera and forward vector at center
		D3DXVECTOR3 center = D3DXVECTOR3(0,0,0);
		for(int i = 0; i<4;i++)
		{
			D3DXVECTOR3 startPosXZ = D3DXVECTOR3(startPositions[i].x, 0, startPositions[i].z);
			D3DXVECTOR3 temp = center - startPosXZ;
			D3DXVECTOR3 tempN;
			D3DXVec3Normalize(&tempN, &temp);
			forwardVectors[i] = tempN;
		}
		*/
		this->mNet->SetForwardVectors(forwardVectors, 4);

		this->mRounds = 3;
		this->mPlatform		= new Platform("Media/CTFMap1.obj", centerPlatform);
		this->mPlatform->SetShrinkValue(0.0f);
		/*
		this->mBalls		= new Ball*[this->mNumPlayers];

		for(int i = 0; i < this->mNumPlayers; i++)
		{
			if( i == 0)
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,-10));
			else
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,10));
		}
		*/
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
				this->mBalls[i]->SetSound(true);
			}
			else
			{
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,25.0f,5));
				this->mBalls[i]->SetKnockoutMode();
				this->mBalls[i]->SetSound(true);
			}
		}
		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[0]);
		*/
	}
	/**
	* WarLock
	**/
	else if(this->mGameMode == WARLOCK)
	{
		
		D3DXVECTOR3 startPositions[4];
		startPositions[0] = D3DXVECTOR3(0,30.0f,-10);
		startPositions[1] = D3DXVECTOR3(0,30.0f,10);
		startPositions[2] = D3DXVECTOR3(-10,30.0f,0);
		startPositions[3] = D3DXVECTOR3(10,30.0f,0); 
		this->mNet->SetStartPosistions(startPositions, 4);

		D3DXVECTOR3 forwardVectors[4];
		D3DXVECTOR3 center = D3DXVECTOR3(0,0,0);
		for(int i = 0; i<4;i++)
		{
			D3DXVECTOR3 startPosXZ = D3DXVECTOR3(startPositions[i].x, 0, startPositions[i].z);
			D3DXVECTOR3 temp = center - startPosXZ;
			D3DXVECTOR3 tempN;
			D3DXVec3Normalize(&tempN, &temp);
			forwardVectors[i] = tempN;
		}
		this->mNet->SetForwardVectors(forwardVectors, 4);
		this->mPlatform		= new Platform("Media/Cylinder.obj", centerPlatform);
		this->mPlatform->SetScale(Vector3(3,3,3));
		this->mBalls		= new Ball*[this->mNumPlayers];
		this->mPlatform->SetShrinkValue(0.0f);
		/*
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
		*/
		
	}
	else if(this->mGameMode == CREDITS)
	{
		//mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 35, -5));
		//mGe->GetCamera()->LookAt(centerPlatform);
		centerPlatform = D3DXVECTOR3(0,20,0);
		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 45, 0));
		mGe->GetCamera()->LookAt(centerPlatform);
		mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,0,1));
		this->mPlatform		= new Platform("Media/MazeMap.obj", centerPlatform);
		this->mPlatform->SetRotate(true);
		StaticMesh* fla = this->mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(12,22.5f,11));
		this->mPlatform->SetMeshHotZone(fla);
		this->mPlatform->SetHotZonePosition(Vector3(12,22.5f,11));
		this->mPlatform->SetHotZoneRadius(2.0f);
		this->mPlatform->SetShrinkValue(0.0f);
		this->mPlatform->SetRestition(0.2f);
		this->mBalls		= new Ball*[this->mNumPlayers];
		//this->mBalls[0] = new Ball("Media/Ball.obj", D3DXVECTOR3(10,20,-11));
		this->mBalls[0] = new Ball("Media/Ball.obj", D3DXVECTOR3(10,24,-11));
		this->mBalls[0]->SetForwardVector(Vector3(0,0,1).GetD3DVec());
		this->mBalls[0]->SetKnockoutMode();
		this->mBalls[0]->SetForcePressed(this->mBalls[0]->GetForcePressed()/15.0f);
		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[0]);
		*/
	}
	else if(this->mGameMode == CREDITS2)
	{
		centerPlatform = D3DXVECTOR3(0,20,0);
		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 45, 0));
		mGe->GetCamera()->LookAt(centerPlatform);
		mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,0,1));
		this->mPlatform		= new Platform("Media/MazeMap.obj", centerPlatform);
		
		//this->mPlatform		= new Platform("Media/Cylinder.obj", centerPlatform);
		this->mPlatform->SetShrinkValue(0.0f);
		this->mPlatform->SetRestition(0.0f);
		StaticMesh* fla = this->mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(12,22.5f,11));
		this->mPlatform->SetMeshHotZone(fla);
		this->mPlatform->SetHotZonePosition(Vector3(12,23.5f,11));
		this->mPlatform->SetHotZoneRadius(2.5f);
		this->mBalls		= new Ball*[this->mNumPlayers];
		this->mBalls[0] = new Ball("Media/Ball.obj", D3DXVECTOR3(10,24,-11));
		
		this->mBalls[0]->SetForwardVector(Vector3(0,0,1).GetD3DVec());
		this->mBalls[0]->SetKnockoutMode();
		this->mBalls[0]->SetAcceleration(Vector3(0,-40,0));

		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[0]);
		*/
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
		temp[i]->SetForwardVector(this->mNet->GetForwardVector(i));
	}
	delete[] this->mBalls;
	this->mBalls = temp;

	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 40, this->mNet->GetStartPos(this->mNet->GetIndex()).z * 1.5f));
	mGe->GetCamera()->LookAt(D3DXVECTOR3(0,10,0));
}

void GameManager::AddBallWarlock()
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
		temp[i]->SetForwardVector(this->mNet->GetForwardVector(i));
		temp[i]->AddSpell(new ChargeSpell());
		temp[i]->AddSpell(new SprintSpell());
		temp[i]->AddSpell(new HardenSpell());
		temp[i]->AddSpell(new InvisibilitySpell());
	}
	delete[] this->mBalls;
	this->mBalls = temp;
	
	if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[this->mNet->GetIndex()]);
	else
	{
		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 40, this->mNet->GetStartPos(this->mNet->GetIndex()).z * 1.5f));
		mGe->GetCamera()->LookAt(D3DXVECTOR3(0,10,0));
	}
	
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

bool GameManager::WarLock(float dt, float& warlockTimer)
{
	float newdt = dt/1000.0f;
	/* will be implemented when we have the rules, for now just play around in 1 minutes then gameover */
	warlockTimer += newdt;
	if(warlockTimer > 600.0f)
		return false;
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

void GameManager::ClientKeyPress(float diff, const int index, char key)
{

	if(key == 'A')
		mBalls[index]->AddForceLeftOfForwardDirection(diff);
	if(key == 'D')
		mBalls[index]->AddForceRightOfForwardDirection(diff);
	/* wTF */
	if(key == 'W')
		mBalls[index]->AddForceForwardDirection(diff);
	
	
	if(key == 'S')
		mBalls[index]->AddForceOppositeForwardDirection(diff);
	
	if(key == 'Q')
		mBalls[index]->RotateForwardLeft(diff);
	if(key == 'E')
		mBalls[index]->RotateForwardRight(diff);
	if(key == '1')
		mBalls[index]->UseSpell(1);
	if(key == '2')
		mBalls[index]->UseSpell(2);
	if(key == '3')
		mBalls[index]->UseSpell(3);
	if(key == '4')
		mBalls[index]->UseSpell(4);
	if(key == VK_SPACE)
		mBalls[index]->AddForce(Vector3(0, diff,0));
	
}

void GameManager::InputKeysPressedSelf(float diff, int index, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu)
{
	if(mGe->GetEngineParameters().CamType == TRD)
	{
		if(mGe->GetKeyListener()->IsPressed('A'))
			mBalls[index]->AddForceLeftOfForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('W'))
			mBalls[index]->AddForceForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('S'))
			mBalls[index]->AddForceOppositeForwardDirection(diff);
		if(mGe->GetKeyListener()->IsPressed('Q'))
			mBalls[index]->RotateForwardLeft(diff);
		if(mGe->GetKeyListener()->IsPressed('E'))
			mBalls[index]->RotateForwardRight(diff);
		if(mGe->GetKeyListener()->IsPressed('D'))
			mBalls[index]->AddForceRightOfForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('1'))
			mBalls[index]->UseSpell(1);
		if(mGe->GetKeyListener()->IsPressed('2'))
			mBalls[index]->UseSpell(2);
		if(mGe->GetKeyListener()->IsPressed('3'))
			mBalls[index]->UseSpell(3);
		if(mGe->GetKeyListener()->IsPressed('4'))
			mBalls[index]->UseSpell(4);
		if(mGe->GetKeyListener()->IsPressed('Z') && !zoomOutPressed)
		{
			mBalls[index]->ZoomOut();
			zoomOutPressed = true;
		}
		else if(!mGe->GetKeyListener()->IsPressed('Z'))
			zoomOutPressed = false;
		if(mGe->GetKeyListener()->IsPressed('C') && !zoomInPressed)
		{
			mBalls[index]->ZoomIn();
			zoomInPressed = true;
		}
		else if(!mGe->GetKeyListener()->IsPressed('C'))
			zoomInPressed = false;
	}
	else
	{

	}
	if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
	{
		running = this->mIGM->Run();
		quitByMenu = !running;
	}
}


void GameManager::SendKeyInputs(const int clientIndex, float diff)
{
	char keyDowns[5] = {0};
	char keysToCheck[11] = {'A', 'D', 'W', 'S', 'Q', 'E', VK_SPACE, '1', '2', '3', '4'};
	int numKeys = 0;
	for(int i = 0; i < 11; i++)
	{
		if(mGe->GetKeyListener()->IsPressed(keysToCheck[i]) && numKeys < 5)
			keyDowns[numKeys++] = keysToCheck[i];
	}
				
	if(numKeys == 0)
		keyDowns[numKeys++] = '?'; //"idle"-key

	this->mNet->AddKeyInput(clientIndex, keyDowns, numKeys, diff);
	
}
void GameManager::HandleClientKeyInputs(const int clientIndex, float diff)
{
	
	int flip = 1;
	if(this->mNet->GetStartPos(clientIndex).z > 0)
		flip = -1;
	//keep reading client inputs until the sum of all DT has exceeded server DT (->not allowed to move any more)
	KeyInput* command = this->mNet->GetNextCommand(clientIndex);
	float duration = 0.0f;
	if(command != NULL)
	{
		duration = command->dt;
		while(duration <=  diff && command != NULL)
		{
			for(int c = 0; c < command->numKeys; c++)
			{
				this->ClientKeyPress(command->dt, clientIndex, command->keys[c]);
			}
			this->mNet->SetExecTime(this->mNet->GetExecTime(clientIndex) + command->dt, clientIndex);
			this->mNet->PopCommand(clientIndex);


			command = this->mNet->GetNextCommand(clientIndex);
			if(command != NULL)
				duration += command->dt;
								
		}
		if(duration > diff && command != NULL)
		{
			duration -= command->dt;
									
			for(int c = 0; c < command->numKeys; c++)
			{
				//ADD A CHECK HERE SO THAT THE SAME KEY CANT APPEAR MORE THAN ONCE IN THE ARRAY (COULD CHEAT THE SYSTEM THIS WAY)
				
				this->ClientKeyPress((diff - duration), clientIndex, command->keys[c]);
			}

			command->dt -= (diff - duration);
								
			this->mNet->SetExecTime(this->mNet->GetExecTime(clientIndex) + (diff - duration), clientIndex);
		}
	}
}