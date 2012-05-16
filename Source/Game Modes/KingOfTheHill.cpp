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
		this->mTeam = TEAM::NOTEAM;
}

KingOfTheHill::~KingOfTheHill()
{
		for(int i = 0; i < 5; i++)
		{
			this->mGe->DeleteLight(this->mLights[i]);
		}
		SAFE_DELETE(this->mIGM);

		SAFE_DELETE(this->mChooseTeamMenu);

		mGe->DeleteText(this->mHud[0]);
		mGe->DeleteText(this->mHud[1]);
		mGe->DeleteText(this->mHud[2]);
		mGe->DeleteText(this->mHud[3]);
		mGe->DeleteText(this->mHud[4]);
		

		
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
		this->mPlatform		= new Map("Media/KOTHMap1.obj", centerPlatform);
		this->mPlatform->SetShrinkValue(0.0f);
		Vector3 hotZone = Vector3(0.0f,20.2f,0.0f);
		this->mPlatform->SetHotZonePosition(hotZone);
		float radius = 5.4f;
		this->mPlatform->SetHotZoneRadius(radius);
		
		
		//this->mPlatform->SetMaxTimeInHotZone(30.0f);
		//this->mPlatform->SetMaxTimeInHotZoneContinuously(10.0f);
		
		KOTHInfo* gmi = (KOTHInfo*)this->mServerInfo.GetGameModeInfo();
		this->mPlatform->SetMaxTimeInHotZone(gmi->GetMaxTimeAccumulated());
		this->mPlatform->SetMaxTimeInHotZoneContinuously(gmi->GetMaxTimeContinuously());
		this->mNumberOfRounds = gmi->GetNumRounds();

		

		this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
		this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
		this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
		this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
		this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
		for(int i = 0; i < 5; i++)
			this->mLights[i]->SetIntensity(30.0f);
		this->mIGM	= new InGameMenu(this->mGe);
		
		this->mChooseTeamMenu = new ChooseTeamMenu(this->mGe);

		/* Set hud */
		this->mHud[0] = mGe->CreateText("",D3DXVECTOR2(20,20),2.0f,"Media/Fonts/1");
		this->mHud[1] = mGe->CreateText("",D3DXVECTOR2(10,750),1.0f,"Media/Fonts/1");
		this->mHud[2] = mGe->CreateText("",D3DXVECTOR2(10,790),1.0f,"Media/Fonts/1");
		this->mHud[3] = mGe->CreateText("",D3DXVECTOR2(800,20),1.0f,"Media/Fonts/1");
		this->mHud[4] = mGe->CreateText("",D3DXVECTOR2(1000,20),1.0f,"Media/Fonts/1");
}

void KingOfTheHill::Intro()
{
		Text*	intro = mGe->CreateText("King of the Hill",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

void KingOfTheHill::PlaySpecific()
{	
		GameMode::PlayLan();
}


void KingOfTheHill::ShowStats()
{

}

void KingOfTheHill::ShowHud()
{
		string s;
		float tmp = (600.0f - this->mTimeElapsed);
		tmp = floor(tmp * 10.0f) / 10.0f;
		s = "Timer: " + MaloW::convertNrToString(tmp);
		this->mHud[0]->SetText(s);
		Vector3 n = this->mBalls[this->mNet->GetIndex()]->GetNormalContact();
		string t = " x: " + MaloW::convertNrToString(floor(10*n.x)/10.0f) + " y: " + MaloW::convertNrToString(floor(10*n.y)/10.0) + " z: " + MaloW::convertNrToString(floor(10*n.z)/10.0);
		if(this->mBalls[this->mNet->GetIndex()]->GetHasContact())
			s = "Contact: True";
		else
			s = "Contact: False";
		this->mHud[1]->SetText(s);
		s =  "Normal: " + t;
		this->mHud[2]->SetText(s);
		s = "T1: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetTimeInHotZone())/10.0f);
		this->mHud[3]->SetText(s);
		s = "T2: " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetTimeInHotZoneContinuously())/10.0f);
		this->mHud[4]->SetText(s);
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
		if((numberInHotZone >0) && (numberInHotZone == numberBlue || numberInHotZone == numberRed || numberNone == 1 ))  
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
						this->mBalls[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
						this->mNet->GetBall(i)->SetPos(this->mBalls[i]->GetPosition());
						Vector3 vel = Vector3(0,0,0);
						this->mNet->GetBall(i)->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z));
						this->mNet->GetBall(i)->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
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
					this->mBalls[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
					this->mNet->GetBall(i)->SetPos(this->mBalls[i]->GetPosition());
					Vector3 vel = Vector3(0,0,0);
					this->mNet->GetBall(i)->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z));
					this->mNet->GetBall(i)->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
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
		temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetBall(i)->GetStartPos());
		temp[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		temp[i]->SetStartForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		//if(this->mNet->GetIndex() == i)
			//temp[i]->SetTeamColor(this->mTeam);
		
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