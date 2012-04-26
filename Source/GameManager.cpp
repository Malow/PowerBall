#include "GameManager.h"

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
	counter = 0.0f;
	this->mNet = new GameNetwork();
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
	this->Initialize();
	bool running = true;
	
	counter = 0;
	while(running)
	{
		int numAlivePlayers = 0;
				
		float diff = mGe->Update();	
		counter += diff;

		

		/* the move of ball 1 and ball 2 should be done using a 
		*  timer that perhaps is set to only add force every 
		*  intervall. so we don't get a jumpy motion of the 
		*  balls :)
		*
		*  Use the settings in ball to change sensitivity on moving the ball
		*/
		// move ball 1
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

		if(mGe->GetKeyListener()->IsPressed('P'))
			mGe->GetCamera()->moveForward(diff);
		if(mGe->GetKeyListener()->IsClicked(1))
			mGe->GetCamera()->moveBackward(diff);

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
			running = this->mIGM->Run();
		

		for(int i = 0; i < this->mNumPlayers; i++)
		{
			this->mBalls[i]->Update(diff, this->mPlatform);

			if(this->mBalls[i]->IsAlive())
				numAlivePlayers += 1;
			
		}
		
		// will be moved to phisics simulation class
		for(int i = 0; i < this->mNumPlayers; i++)
		{
			Ball* b1 = this->mBalls[i];
			for(int j = i+1; j < this->mNumPlayers; j++)
			{
				Ball* b2 = this->mBalls[j];
				if(b1->collisionWithSphereSimple(b2))
					b1->collisionSphereResponse(b2, diff);

			}
			// check ball[i] against platform
			Vector3 normalPlane;
			if(b1->collisionWithPlatformSimple(this->mPlatform,normalPlane))
			{
				
				b1->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
			}
		}
		
		mPlatform->Update(diff);

		if(numAlivePlayers <= 1)
			running = false;
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
				this->mBalls[i]->Update(diff, this->mPlatform);
			// will be moved to phisics simulation class
			for(int i = 0; i < this->mNumPlayers; i++)
			{
				if(this->mPlatform->IsOnPlatform(mBalls[i]->GetPosition().x, mBalls[i]->GetPosition().z))
				{
					if(i != this->mNet->GetIndex())
					{
						int flip = 1;
						if(this->mNet->GetStartPos(i) > 0)
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
							b1->collisionSphereResponse(b2, diff);

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
				if(this->mNet->GetStartPos(this->mNet->GetIndex()) > 0)
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
					this->mBalls[i]->Update(diff, this->mPlatform);

				for(int i = 0; i < this->mNumPlayers; i++)
				{
					Ball* b1 = this->mBalls[i];
					for(int j = i+1; j < this->mNumPlayers; j++)
					{
						Ball* b2 = this->mBalls[j];
						if(b1->collisionWithSphereSimple(b2))
							b1->collisionSphereResponse(b2, diff);

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
		if(this->mGameMode == CTF)
			if(!this->CaptureTheFlag())
				running = false;
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

	D3DXVECTOR3 startPositions[4];
	startPositions[0] = D3DXVECTOR3(0,24.7f,-20);
	startPositions[1] = D3DXVECTOR3(0,24.7f,20);
	startPositions[2] = D3DXVECTOR3(-5,24.7f,-20);
	startPositions[3] = D3DXVECTOR3(-5,24.7f,20);
	
	this->mNet->SetStartPosistions(startPositions, 4);

	/**
	* King of the hill
	**/
	if(this->mGameMode == KOTH || this->mGameMode == KOTH2)
	{
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
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,-5));
			else
				this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,30.0f,5));
		}
	}
	// wait until everything is loaded and then drop the balls from hight above
	mGe->LoadingScreen("Media/LoadingScreenBG.png", "Media/LoadingScreenPB.png");	// Changed by MaloW
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
		this->mBalls[0]->AddItem(this->mEnemyFlag->GetMesh());
		this->mEnemyFlag->SetAtBase(false);
	}
	if(!this->mEnemyFlag->GetAtBase())
	{
		D3DXVECTOR3 distBetweenFlags = D3DXVECTOR3(this->mEnemyFlag->GetMesh()->GetPosition() - this->mFriendlyFlag->GetMesh()->GetPosition());
		if(D3DXVec3Length(&distBetweenFlags) < (this->mBalls[0]->GetRadius()) && this->mFriendlyFlag->GetAtBase())
		{
			this->mBalls[0]->ResetInventory();
			this->mEnemyFlag->Reset();
			this->mEnemyFlag->SetAtBase(true);
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
			this->mBalls[1]->AddItem(this->mFriendlyFlag->GetMesh());
			this->mFriendlyFlag->SetAtBase(false);
		}
		if(!this->mFriendlyFlag->GetAtBase())
		{
			D3DXVECTOR3 distBetweenFlags = D3DXVECTOR3(this->mFriendlyFlag->GetMesh()->GetPosition() - this->mEnemyFlag->GetMesh()->GetPosition());
			if(D3DXVec3Length(&distBetweenFlags) < (this->mBalls[1]->GetRadius()) && this->mEnemyFlag->GetAtBase())
			{
				this->mBalls[1]->ResetInventory();
				this->mFriendlyFlag->Reset();
				this->mFriendlyFlag->SetAtBase(true);
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
	float numberOfPlayers = this->mNumPlayers;
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