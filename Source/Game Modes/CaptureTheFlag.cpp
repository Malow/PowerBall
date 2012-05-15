#include "CaptureTheFlag.h"
#include "..\Game Objects\FlagCTF.h"
CaptureTheFlag::CaptureTheFlag()
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

CaptureTheFlag::CaptureTheFlag(GraphicsEngine* ge, GameNetwork* net, ServerInfo server)
{
		this->mGe = ge;
		this->mNumberOfPlayers = 0;
		this->mNumberOfRounds = 3;
		this->mGameMode = server.GetGameMode();
		this->mNet = net;
		this->mServerInfo = server;
		this->mTimeElapsed = 0.0f;
	
}

CaptureTheFlag::~CaptureTheFlag()
{
		for(int i = 0; i < 5; i++)
		{
			this->mGe->DeleteLight(this->mLights[i]);
		}
		SAFE_DELETE(this->mIGM);
		SAFE_DELETE(this->mFriendlyFlag);
		SAFE_DELETE(this->mEnemyFlag);
}

void CaptureTheFlag::Initialize()
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
		D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,10,0);
		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 25, -10));
		mGe->GetCamera()->LookAt(centerPlatform);	
		this->mNet->SetStartForwardVectors(forwardVectors, 4);

		this->mNumberOfRounds = 3;
		this->mPlatform		= new Map("Media/CTFMap1.obj", centerPlatform);
		this->mPlatform->SetShrinkValue(0.0f);
		
		this->mEnemyFlag = new FlagCTF(mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(0, 20, 25)), D3DXVECTOR3(0, 20, 25));
		this->mFriendlyFlag = new FlagCTF(mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(0, 20, -25)), D3DXVECTOR3(0, 20, 25));
		/*D3DXVECTOR3 startPositions[4];
		startPositions[0] = D3DXVECTOR3(0,24.7f,-14);
		startPositions[1] = D3DXVECTOR3(0,24.7f,14);
		startPositions[2] = D3DXVECTOR3(0,24.7f,-12);
		startPositions[3] = D3DXVECTOR3(0,24.7f,12);*/
	
		this->mNet->SetStartPosistions(startPositions, 4);
		//D3DXVECTOR3 forwardVectors[4];
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
		
		this->mNet->SetForwardVectors(forwardVectors, 4);

		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 25, -10));
		mGe->GetCamera()->LookAt(centerPlatform);	
		this->mNumberOfRounds = 3;
		this->mPlatform		= new Map("Media/KOTHMap1.obj", centerPlatform);
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
		*/
		

		this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
		this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
		this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
		this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
		this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
		for(int i = 0; i < 5; i++)
			this->mLights[i]->SetIntensity(30.0f);
		this->mIGM	= new InGameMenu(this->mGe);
		
}

void CaptureTheFlag::Intro()
{
		Text*	intro = mGe->CreateText("CaptureTheFlag",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

void CaptureTheFlag::Play()
{	
		bool running = true;
		bool zoomOutPressed = false;
		bool zoomInPressed = false;
		bool quitByMenu = false;
		this->mNet->Start(this->mServerInfo);
		this->mGe->Update();
		int numAlivePlayers = 0;
		float CaptureTheFlagTimer = 0;
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
				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					if(i != this->mNet->GetIndex())
					{
						this->HandleClientKeyInputs(i, diff);
						this->mBalls[i]->Update(diff, true);
					}
					else
					{
						this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
						this->mBalls[i]->Update(diff, false);
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

				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					this->mBalls[i]->UpdatePost();
				}

				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					this->mNet->SetPos(this->mBalls[i]->GetPosition(), i);
					Vector3 vel = this->mBalls[i]->GetVelocity();
					this->mNet->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z),  i);
				}
			}
			else //is client
			{
				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					if(this->mNet->GetIndex() != i)
					{
						D3DXVECTOR3 rotVector = this->mNet->GetPos(i) - this->mBalls[i]->GetPosition();
						this->mBalls[i]->SetPosition(this->mNet->GetPos(i));
						this->mBalls[i]->SetTempPosition(this->mNet->GetPos(i));
						this->mBalls[i]->SetVelocity(this->mNet->GetVel(i));
						this->mBalls[i]->Rotate(rotVector);
					}
				}
				if(this->mNet->GetIndex() < this->mNumberOfPlayers)
				{
					int i = this->mNet->GetIndex();

					this->SendKeyInputs(i, diff);
					this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
				

					this->mBalls[i]->Update(diff);


					for(int c = 0; c < this->mNumberOfPlayers; c++)
					{
						PowerBall* b1 = this->mBalls[c];
						for(int j = c+1; j < this->mNumberOfPlayers; j++)
						{
							PowerBall* b2 = this->mBalls[j];
							if(b1->collisionWithSphereSimple(b2))
								b1->collisionSphereResponse(b2);

						}
					}

					Vector3 normalPlane;
					if(this->mBalls[i]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
						this->mBalls[i]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);


					this->mBalls[i]->UpdatePost();
				

					this->mNet->AddMovementPowerBall(this->mBalls[i]);

				}
			}


			for(int i = 0; i < this->mNumberOfPlayers; i++)
			{
				if(this->mBalls[i]->IsAlive())
					numAlivePlayers += 1;
			}
			mPlatform->Update(diff);

		
			if(!this->mNet->UpdatePowerBall(this->mBalls, this->mNumberOfPlayers, diff))
				running = false;

			if(this->mNet->GetNumPlayers() > this->mNumberOfPlayers)
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
			float tmp = (600.0f - this->mTimeElapsed);
			tmp = floor(tmp * 10.0f) / 10.0f;
			s = "Timer: " + MaloW::convertNrToString(tmp);
			hudR1->SetText(s);
			if(this->mNet->IsServer())
			if(this->checkWinConditions(diff))
				running = false;
			float newdt = diff/1000.0f;
			/* will be implemented when we have the rules, for now just play around in 600 seconds then gameover */
			this->mTimeElapsed += newdt;
			if(this->mTimeElapsed > 600.0f)
				running = false;
			
		}
		mGe->DeleteText(hudR1);
		this->mNet->Close();
}

void CaptureTheFlag::ShowStats()
{

}

bool CaptureTheFlag::checkWinConditions(float dt)
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
			this->mNumberOfRounds--;
			if(this->mNumberOfRounds <= 0)
				return true;
		}
	}


	//just copied temporarily to try
	if(this->mNumberOfPlayers > 1)
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
				this->mNumberOfRounds--;
				if(this->mNumberOfRounds <= 0)
					return true;
			}
		}
	}
	this->mNet->SetFlagPos(this->mFriendlyFlag->GetMesh()->GetPosition(), 0);
	this->mNet->SetFlagPos(this->mEnemyFlag->GetMesh()->GetPosition(), 1);
	return false;
}

void CaptureTheFlag::AddBall()
{
	
	int old = this->mNumberOfPlayers;
	this->mNumberOfPlayers = this->mNet->GetNumPlayers();
	PowerBall** temp = new PowerBall*[this->mNumberOfPlayers];
	for(int i = 0; i < old; i++)
	{
		temp[i] = this->mBalls[i];
	}
	for(int i = old; i < this->mNumberOfPlayers; i++)
	{
		temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetStartPos(i));
		temp[i]->SetForwardVector(this->mNet->GetForwardVector(i));
	}
	delete[] this->mBalls;
	this->mBalls = temp;		

	if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setPowerBallToFollow(this->mBalls[this->mNet->GetIndex()]);
	else
	{
		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 40, this->mNet->GetStartPos(this->mNet->GetIndex()).z * 1.5f));
		mGe->GetCamera()->LookAt(D3DXVECTOR3(0,10,0));
	}


}