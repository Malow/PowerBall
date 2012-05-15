#include "KingOfTheHill.h"

KingOfTheHill::KingOfTheHill()
{
		this->mGe = NULL;
		this->mIGM = NULL;
		this->mBalls = NULL;
		this->mPlatform = NULL;
		this->mNumberOfPlayers = -1;
		this->mNumberOfRounds = -1;
		this->mGameMode = -1;
		this->mTimeElapsed = 0.0f;

		this->mChooseTeamMenu = NULL;
		this->mTeam = TEAM::NOTEAM;
}

KingOfTheHill::KingOfTheHill(GraphicsEngine* ge, GameNetwork* net, ServerInfo server)
{
		this->mGe = ge;
		this->mNumberOfPlayers = 0;
		this->mNumberOfRounds = 3;
		this->mGameMode = server.GetGameMode();
		this->mNet = net;
		this->mServerInfo = server;
		this->mTimeElapsed = 0.0f;

		this->mChooseTeamMenu = NULL;
}

KingOfTheHill::~KingOfTheHill()
{
		for(int i = 0; i < 5; i++)
		{
			this->mGe->DeleteLight(this->mLights[i]);
		}
		SAFE_DELETE(this->mIGM);

		SAFE_DELETE(this->mChooseTeamMenu);
}

void KingOfTheHill::Initialize()
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
		this->mNet->SetStartForwardVectors(forwardVectors, 4);
		
		D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,10,0);
		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 25, -10));
		mGe->GetCamera()->LookAt(centerPlatform);	
		this->mNumberOfRounds = 3;
		this->mPlatform		= new Map("Media/KOTHMap1.obj", centerPlatform);
		this->mPlatform->SetShrinkValue(0.0f);
		Vector3 hotZone = Vector3(0.0f,20.2f,0.0f);
		this->mPlatform->SetHotZonePosition(hotZone);
		float radius = 5.4f;
		this->mPlatform->SetHotZoneRadius(radius);
		
		
		this->mPlatform->SetMaxTimeInHotZone(30.0f);
		this->mPlatform->SetMaxTimeInHotZoneContinuously(10.0f);

		

		this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
		this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
		this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
		this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
		this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
		for(int i = 0; i < 5; i++)
			this->mLights[i]->SetIntensity(30.0f);
		this->mIGM	= new InGameMenu(this->mGe);
		
		this->mChooseTeamMenu = new ChooseTeamMenu(this->mGe);
}

void KingOfTheHill::Intro()
{
		Text*	intro = mGe->CreateText("King of the Hill",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

void KingOfTheHill::Play()
{	
		bool zoomOutPressed = false;
		bool zoomInPressed = false;
		this->mNet->Start(this->mServerInfo);
		this->mGe->Update();
		bool roundsLeft = true;
		int roundsPlayed = 0;

		//choose team before starting the game
		this->mTeam = this->mChooseTeamMenu->Run();
		//this->mBalls[this->mNet->GetIndex]->SetTeamColor(team);**

		while(roundsLeft && this->mGe->isRunning())
		{

			this->PlayRound(roundsLeft, zoomInPressed, zoomOutPressed); 
			roundsPlayed++;;
			if(roundsPlayed == this->mNumberOfRounds)
				roundsLeft = false;
		}
		
		/*
		bool running = true;
		bool quitByMenu = false;
		int numAlivePlayers = 0;
		float KingOfTheHillTimer = 0;
		Text* hudR1 = mGe->CreateText("",D3DXVECTOR2(20,20),2.0f,"Media/Fonts/1");
		string s;
		Text* hudR20 = mGe->CreateText("",D3DXVECTOR2(10,750),1.0f,"Media/Fonts/1");
		Text* hudR21 = mGe->CreateText("",D3DXVECTOR2(10,790),1.0f,"Media/Fonts/1");
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
						}
						else
						{
							this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
						}	

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
						//for(int i = 0; i < this->mNumberOfPlayers; i++)
							//this->mBalls[i]->UpdatePost();
	
					}
					for(int i = 0; i < this->mNumberOfPlayers; i++)
					{					
						this->mBalls[i]->UpdatePost();
						bool clientBall = true;
						if(i == this->mNet->GetIndex())
							clientBall = false;

						this->mBalls[i]->Update(diff, clientBall); //split up due to the balls affecting each other, so cant send final position until all balls updated
						clientBall = true;

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
							this->mBalls[i]->Rotate(rotVector);
						}
					}
					if(this->mNet->GetIndex() < this->mNumberOfPlayers)
					{
						int i = this->mNet->GetIndex();

						this->SendKeyInputs(i, diff);
						this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
				
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

						this->mBalls[i]->Update(diff);
				

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
				Vector3 n = this->mBalls[0]->GetNormalContact();
				string t = " x: " + MaloW::convertNrToString(floor(10*n.x)/10.0f) + " y: " + MaloW::convertNrToString(floor(10*n.y)/10.0) + " z: " + MaloW::convertNrToString(floor(10*n.z)/10.0);
				if(this->mBalls[0]->GetHasContact())
					s = "Contact: True";
			
				else
					s = "Contact: False";
			
				hudR20->SetText(s);
				s =  "Normal: " + t;
				hudR21->SetText(s);
				if(this->checkWinConditionsTeam(diff))
					running = false;
				float newdt = diff/1000.0f;
				
				this->mTimeElapsed += newdt;
				if(this->mTimeElapsed > 600.0f)
					running = false;
		}
		mGe->DeleteText(hudR1);
		mGe->DeleteText(hudR20);
		mGe->DeleteText(hudR21);
		*/
		this->mNet->Close();
}

void KingOfTheHill::PlayRound(bool& roundsLeft, bool& zoomInPressed, bool& zoomOutPressed)
{
		bool running = true;
		bool quitByMenu = false;
		int numAlivePlayers = 0;
		float KingOfTheHillTimer = 0;
		Text* hudR1 = mGe->CreateText("",D3DXVECTOR2(20,20),2.0f,"Media/Fonts/1");
		string s;
		Text* hudR20 = mGe->CreateText("",D3DXVECTOR2(10,750),1.0f,"Media/Fonts/1");
		Text* hudR21 = mGe->CreateText("",D3DXVECTOR2(10,790),1.0f,"Media/Fonts/1");
		Text* hudR30 = mGe->CreateText("",D3DXVECTOR2(800,20),1.0f,"Media/Fonts/1");
		Text* hudR31 = mGe->CreateText("",D3DXVECTOR2(1000,20),1.0f,"Media/Fonts/1");
		LARGE_INTEGER oldTick = LARGE_INTEGER();
		LARGE_INTEGER now =  LARGE_INTEGER();
		while(running && this->mGe->isRunning())
		{
				float diff = mGe->Update(); //A problem when the user opens up ingame menu is that the diff after resume is incredibly high so it breaks game logic, game gotta continue in the background if network :P	

				if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
					roundsLeft = running = this->mIGM->Run();
		
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
						}
						else
						{
							this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
						}	

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
						//for(int i = 0; i < this->mNumberOfPlayers; i++)
							//this->mBalls[i]->UpdatePost();
	
					}
					for(int i = 0; i < this->mNumberOfPlayers; i++)
					{					
						this->mBalls[i]->UpdatePost();
						bool clientBall = true;
						if(i == this->mNet->GetIndex())
							clientBall = false;

						this->mBalls[i]->Update(diff, clientBall); //split up due to the balls affecting each other, so cant send final position until all balls updated
						clientBall = true;

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
							this->mBalls[i]->Rotate(rotVector);
						}
					}
					if(this->mNet->GetIndex() < this->mNumberOfPlayers)
					{
						int i = this->mNet->GetIndex();

						this->SendKeyInputs(i, diff);
						this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
				
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

						this->mBalls[i]->Update(diff);
				

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
				
				float tmp = (600.0f - this->mTimeElapsed);
				tmp = floor(tmp * 10.0f) / 10.0f;
				s = "Timer: " + MaloW::convertNrToString(tmp);
				hudR1->SetText(s);
				Vector3 n = this->mBalls[this->mNet->GetIndex()]->GetNormalContact();
				string t = " x: " + MaloW::convertNrToString(floor(10*n.x)/10.0f) + " y: " + MaloW::convertNrToString(floor(10*n.y)/10.0) + " z: " + MaloW::convertNrToString(floor(10*n.z)/10.0);
				if(this->mBalls[this->mNet->GetIndex()]->GetHasContact())
					s = "Contact: True";
			
				else
					s = "Contact: False";
			
				hudR20->SetText(s);
				s =  "Normal: " + t;
				hudR21->SetText(s);
				s = "T1: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetTimeInHotZone())/10.0f);
				hudR30->SetText(s);
				s = "T2: " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetTimeInHotZoneContinuously())/10.0f);
				hudR31->SetText(s);
				if(this->checkWinConditions(diff))
					running = false;
				float newdt = diff/1000.0f;
				
				this->mTimeElapsed += newdt;
				if(this->mTimeElapsed > 600.0f)
					running = false;
		}
		mGe->DeleteText(hudR1);
		mGe->DeleteText(hudR20);
		mGe->DeleteText(hudR21);
		mGe->DeleteText(hudR30);
		mGe->DeleteText(hudR31);
}

void KingOfTheHill::ShowStats()
{

}

bool KingOfTheHill::checkWinConditions(float dt)
{
		float newdt = dt/1000.0f;
		int numberOfPlayers = this->mNumberOfPlayers;
		int numberInHotZone = 0;
		int ballIndex = 0;
		int* arrayIndexs = new int[numberOfPlayers];
		bool returnValue = false;
		/* checking which balls that are in the hot zone. */
		for(int i = 0; i<numberOfPlayers; i++)
		{
			if(this->mPlatform->IsInHotZone(this->mBalls[i]->GetPositionVector3(), this->mBalls[i]->GetRadius()))
			{
				/* saving the index of the balls that are in the hot zone and how many they are. */
				arrayIndexs[numberInHotZone] = i;
				numberInHotZone++;
			}
		}

		/* checking so they belong to the same team. */
		int numberRed = 0;
		int numberBlue = 0;
		int numberNone = 0;
		for(int i = 0; i<numberInHotZone; i++)
		{
			ballIndex = arrayIndexs[i];
			if(this->mBalls[ballIndex]->GetTeamColor() == TEAM::BLUETEAM)
				numberBlue++;
			if(this->mBalls[ballIndex]->GetTeamColor() == TEAM::REDTEAM)
				numberRed++;
			if(this->mBalls[ballIndex]->GetTeamColor() == TEAM::NOTEAM) // this is for free 4 all
				numberNone++;
		}
		/* if all balls belong to the same team and they are more that zero in the hotzone then update the time. */
		if((numberInHotZone >0) && (numberInHotZone == numberBlue || numberInHotZone == numberRed || numberNone == numberInHotZone ))  
		{
			/* add time to the continuously timer. */
			this->mPlatform->AddTimeInHotZoneContinuously(newdt);
			
			/* add time to the individual timers for the balls (non continuously). */
			for(int i = 0; i<numberInHotZone; i++)
			{
				ballIndex = arrayIndexs[i];
				this->mBalls[ballIndex]->AddTimeInHotZone(newdt);
			}
			
			/* check all the times for the ball if any ball has more than the time set by the map information. */
			for(int i = 0; i<numberInHotZone; i++)
			{
				ballIndex = arrayIndexs[i];
				if(this->mBalls[ballIndex]->GetTimeInHotZone() >= this->mPlatform->GetMaxTimeInHotZone())
				{
					/* here the round is over. */
					for(int i = 0; i<numberOfPlayers; i++)
					{
						this->mBalls[i]->ResetTime();
						this->mBalls[i]->SetToStartPosition();
						this->mBalls[i]->SetForwardVector(this->mNet->GetStartForwardVector(i));
						this->mNet->SetPos(this->mBalls[i]->GetPosition(), i);
						Vector3 vel = Vector3(0,0,0);
						this->mNet->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z),  i);
						this->mNet->SetForwardVector(this->mNet->GetStartForwardVector(i), i);
						this->mPlatform->ResetHotZone();
					}
					returnValue = true;
				}
			}

			/* check if the continuously time for the balls. */
			if(this->mPlatform->GetTimeInHotZoneContinuously() >= this->mPlatform->GetMaxTimeInHotZoneContinuously())
			{
				/* here the round is over. */
				for(int i = 0; i<numberOfPlayers; i++)
				{
					this->mBalls[i]->ResetTime();
					this->mBalls[i]->SetToStartPosition();
					this->mBalls[i]->SetForwardVector(this->mNet->GetStartForwardVector(i));
					this->mNet->SetPos(this->mBalls[i]->GetPosition(), i);
					Vector3 vel = Vector3(0,0,0);
					this->mNet->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z),  i);
					this->mNet->SetForwardVector(this->mNet->GetStartForwardVector(i), i);
					this->mPlatform->ResetHotZone();
				}			
				returnValue = true;
			}

		}
		else
		{
			this->mPlatform->ResetHotZone();
			returnValue = false;
		}
		delete arrayIndexs;
		return returnValue;
		
}

void KingOfTheHill::AddBall()
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
		temp[i]->SetForwardVector(this->mNet->GetStartForwardVector(i));
		temp[i]->SetStartForwardVector(this->mNet->GetStartForwardVector(i));
		if(this->mNet->GetIndex() == i)
			temp[i]->SetTeamColor(this->mTeam);
		
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