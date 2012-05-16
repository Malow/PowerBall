#include "Warlock.h"

Warlock::Warlock()
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

Warlock::Warlock(GraphicsEngine* ge, GameNetwork* net, ServerInfo server)
{
		this->mGe = ge;
		this->mNumberOfPlayers = 0;
		this->mNumberOfRounds = 3;
		this->mGameMode = server.GetGameMode();
		this->mNet = net;
		this->mServerInfo = server;
		this->mTimeElapsed = 0.0f;
		
	
}

Warlock::~Warlock()
{
		for(int i = 0; i < 5; i++)
		{
			this->mGe->DeleteLight(this->mLights[i]);
		}
		SAFE_DELETE(this->mIGM);

}

void Warlock::Initialize()
{
		D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,4,0);
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
		this->mNet->SetStartForwardVectors(forwardVectors, 4);
		this->mPlatform		= new Map("Media/WarlockMap.obj", centerPlatform);
		
		this->mBalls		= new PowerBall*[this->mNumberOfPlayers];
		this->mPlatform->SetShrinkValue(0.0f);
		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setPowerBallToFollow(this->mBalls[0]);
			*/
		WARLOCKInfo* gmi = (WARLOCKInfo*)this->mServerInfo.GetGameModeInfo();
		this->mNumberOfRounds = gmi->GetNumRounds();
}

void Warlock::Intro()
{
		Text*	intro = mGe->CreateText("Warlock",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

void Warlock::Play()
{	
		bool running = true;
		bool zoomOutPressed = false;
		bool zoomInPressed = false;
		bool quitByMenu = false;
		this->mNet->Start(this->mServerInfo);
		this->mGe->Update();
		int numAlivePlayers = 0;
		float warlockTimer = 0;
		
		//MsgHandler::GetInstance().JoinTeam((TEAM)this->mTeam);
		
		this->hudR1 = mGe->CreateText("",D3DXVECTOR2(20,20),2.0f,"Media/Fonts/1");
		this->hudR2 = mGe->CreateText("",D3DXVECTOR2(20,90),1.0f,"Media/Fonts/1");
		this->hudR3 = mGe->CreateText("",D3DXVECTOR2(20,140),1.0f,"Media/Fonts/1");
		this->hudR4 = mGe->CreateText("",D3DXVECTOR2(20,190),1.0f,"Media/Fonts/1");
		this->hudR5 = mGe->CreateText("",D3DXVECTOR2(20,240),1.0f,"Media/Fonts/1");
		this->hudR6 = mGe->CreateText("",D3DXVECTOR2(20,290),1.0f,"Media/Fonts/1");
		this->hudR7 = mGe->CreateText("",D3DXVECTOR2(20,800),1.0f,"Media/Fonts/1");
		this->hudR8 = mGe->CreateText("",D3DXVECTOR2(260,800),1.0f,"Media/Fonts/1");
		this->hudR9 = mGe->CreateText("",D3DXVECTOR2(500,800),1.0f,"Media/Fonts/1");
		this->hudR10 = mGe->CreateText("",D3DXVECTOR2(740,800),1.0f,"Media/Fonts/1");
		this->hudR11 = mGe->CreateText("",D3DXVECTOR2(980,800),1.0f,"Media/Fonts/1");

		this->hudR12 = mGe->CreateText("",D3DXVECTOR2(70,740),1.0f,"Media/Fonts/1");
		this->hudR13 = mGe->CreateText("",D3DXVECTOR2(310,740),1.0f,"Media/Fonts/1");
		this->hudR14 = mGe->CreateText("",D3DXVECTOR2(550,740),1.0f,"Media/Fonts/1");
		this->hudR15 = mGe->CreateText("",D3DXVECTOR2(790,740),1.0f,"Media/Fonts/1");
		this->hudR16 = mGe->CreateText("",D3DXVECTOR2(1030,740),1.0f,"Media/Fonts/1");

	
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
			
			for(int i = 0; i < this->mNumberOfPlayers; i++)
			{
				if(this->mBalls[i]->GetTeamColor() != this->mNet->GetBall(i)->GetTeam()) //causes lag otherwise re-setting the color every frame if its alrdy set.
					this->mBalls[i]->SetTeamColor(this->mNet->GetBall(i)->GetTeam());
			}
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
					this->mNet->GetBall(i)->SetPos(this->mBalls[i]->GetPosition());
					Vector3 vel = this->mBalls[i]->GetVelocity();
					this->mNet->GetBall(i)->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z));
				}
			}
			else //is client
			{
				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					if(this->mNet->GetIndex() != i)
					{
						D3DXVECTOR3 rotVector = this->mNet->GetBall(i)->GetPos() - this->mBalls[i]->GetPosition();
						this->mBalls[i]->SetPosition(this->mNet->GetBall(i)->GetPos());
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
				

					this->mNet->GetBall(i)->AddMovementPowerBall(this->mBalls[i]);

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
			
			this->ShowHud();

			if(this->checkWinConditions(diff))
				running = false;
			
		}
		mGe->DeleteText(this->hudR1);
		mGe->DeleteText(this->hudR2);
		mGe->DeleteText(this->hudR3);
		mGe->DeleteText(this->hudR4);
		mGe->DeleteText(this->hudR5);
		mGe->DeleteText(this->hudR6);
		mGe->DeleteText(this->hudR7);
		mGe->DeleteText(this->hudR8);
		mGe->DeleteText(this->hudR9);
		mGe->DeleteText(this->hudR10);
		mGe->DeleteText(this->hudR11);
		
		mGe->DeleteText(this->hudR12);
		mGe->DeleteText(this->hudR13);
		mGe->DeleteText(this->hudR14);
		mGe->DeleteText(this->hudR15);
		mGe->DeleteText(this->hudR16);
		this->mNet->Close();
}

void Warlock::ShowStats()
{

}

bool Warlock::checkWinConditions(float dt)
{
		float newdt = dt/1000.0f;
		/* will be implemented when we have the rules, for now just play around in 600 seconds then gameover */
		this->mTimeElapsed += newdt;
		if(this->mTimeElapsed > 600.0f)
			return true;
		return false;
}

void Warlock::ShowHud()
{
			string s;
			
			float tmp = (600.0f - this->mTimeElapsed);
			tmp = floor(tmp * 10.0f) / 10.0f;
			s = "Timer: " + MaloW::convertNrToString(tmp);
			this->hudR1->SetText(s);
			s = "Speed: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetVelocity().GetLength())/10.0f) + " m/s";
			this->hudR2->SetText(s);
			float y = floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetVelocity().y)/10.0f;
			if(y > -0.2f && y <0.2f)
				y = 0.0f;
			s = "Vector: ";
			this->hudR3->SetText(s);
			s = "x: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetVelocity().x)/10.0f);
			this->hudR4->SetText(s);
			s = "y: " + MaloW::convertNrToString(y);
			this->hudR5->SetText(s);
			s = "z: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetVelocity().z)/10.0f);
			this->hudR6->SetText(s);
			Spell** spells = this->mBalls[this->mNet->GetIndex()]->GetSpells();
			if(spells[0]->InUse())
				s = "Using";
			else if(spells[0]->IsCharging())
				s = "Charging";
			else if(spells[0]->NeedCoolDown())
				s = "ColdDown";
			else
				s = "Ready";
			this->hudR7->SetText(s);

			if(spells[1]->InUse())
				s = "Using";
			else if(spells[1]->IsCharging())
				s = "Charging";
			else if(spells[1]->NeedCoolDown())
				s = "ColdDown";
			else 
				s = "Ready";
			this->hudR8->SetText(s);

			if(spells[2]->InUse())
				s = "Using";
			else if(spells[2]->IsCharging())
				s = "Charging";
			else if(spells[2]->NeedCoolDown())
				s = "ColdDown";
			else 
				s = "Ready";
			this->hudR9->SetText(s);

			if(spells[3]->InUse())
				s = "Using";
			else if(spells[3]->IsCharging())
				s = "Charging";
			else if(spells[3]->NeedCoolDown())
				s = "ColdDown";
			else 
				s = "Ready";
			this->hudR10->SetText(s);

			if(spells[4]->InUse())
				s = "Using";
			else if(spells[4]->IsCharging())
				s = "Charging";
			else if(spells[4]->NeedCoolDown())
				s = "ColdDown";
			else 
				s = "Ready";
			this->hudR11->SetText(s);

			if(spells[0]->InUse())
				s = "";
			else if(spells[0]->IsCharging())
				s = "";
			else if(spells[0]->NeedCoolDown())
				s = MaloW::convertNrToString(floor(10.0f*spells[0]->GetCoolDownTimeLeft())/10.0f);
			else
				s = "";
			this->hudR12->SetText(s);

			if(spells[1]->InUse())
				s = "";
			else if(spells[1]->IsCharging())
				s = "";
			else if(spells[1]->NeedCoolDown())
				s = MaloW::convertNrToString(floor(10.0f*spells[1]->GetCoolDownTimeLeft())/10.0f);
			else
				s = "";
			this->hudR13->SetText(s);

			if(spells[2]->InUse())
				s = "";
			else if(spells[2]->IsCharging())
				s = "";
			else if(spells[2]->NeedCoolDown())
				s = MaloW::convertNrToString(floor(10.0f*spells[2]->GetCoolDownTimeLeft())/10.0f);
			else
				s = "";
			this->hudR14->SetText(s);

			if(spells[3]->InUse())
				s = "";
			else if(spells[3]->IsCharging())
				s = "";
			else if(spells[3]->NeedCoolDown())
				s = MaloW::convertNrToString(floor(10.0f*spells[3]->GetCoolDownTimeLeft())/10.0f);
			else
				s = "";
			this->hudR15->SetText(s);

			if(spells[4]->InUse())
				s = "";
			else if(spells[4]->IsCharging())
				s = "";
			else if(spells[4]->NeedCoolDown())
				s = MaloW::convertNrToString(floor(10.0f*spells[4]->GetCoolDownTimeLeft())/10.0f);
			else 
				s = "";
			this->hudR16->SetText(s);
			
}

void Warlock::AddBall()
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
			temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetBall(i)->GetStartPos());
			temp[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
			temp[i]->SetStartForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
			temp[i]->AddSpell(new ChargeSpell());
			temp[i]->AddSpell(new SprintSpell());
			temp[i]->AddSpell(new HardenSpell());
			temp[i]->AddSpell(new InvisibilitySpell());
			temp[i]->AddSpell(new JumpSpell());
		}
		delete[] this->mBalls;
		this->mBalls = temp;
	
		if(mGe->GetEngineParameters().CamType == TRD)
				((TRDCamera*)mGe->GetCamera())->setPowerBallToFollow(this->mBalls[this->mNet->GetIndex()]);
		else
		{
			mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 40, this->mNet->GetBall(this->mNet->GetIndex())->GetStartPos().z * 1.5f));
			mGe->GetCamera()->LookAt(D3DXVECTOR3(0,10,0));
		}
}