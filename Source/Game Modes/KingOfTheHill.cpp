#include "KingOfTheHill.h"
#include "..\Physics\PhysicsEngine.h"

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

	this->mTotalTimeCapture = NULL;
	this->mTeam = (int)TEAM::NOTEAM;
	this->mProgressBar = NULL;
	/*
	this->mBackground = NULL;
	this->mTimeLeft = NULL;
	*/
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

	this->mTotalTimeCapture = NULL;
	this->mTeam = (int)TEAM::NOTEAM;
	this->mProgressBar = NULL;
	/*
	this->mBackground = NULL;
	this->mTimeLeft = NULL;
	*/
}

KingOfTheHill::~KingOfTheHill()
{
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
	SAFE_DELETE(this->mIGM);

	SAFE_DELETE(this->mProgressBar);
		

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

	this->mTotalTimeCapture = this->mGe->CreateImage(D3DXVECTOR2(100, 100), D3DXVECTOR2(300, 50), "Media/LoadingScreen/FadeTexture.png");
	this->mTotalTimeCapture->SetOpacity(0.0f);

		
	float width = (float)GetGraphicsEngine()->GetEngineParameters().windowWidth;
	float height = (float)GetGraphicsEngine()->GetEngineParameters().windowHeight;
	float sizeBarX = width/4.0f;
	float sizeBarY = height/24.0f;
	float posBarX = ((width/2.0f) - (sizeBarX/2.0f))/width;
	float posBarY = ((height/15.0f) - (sizeBarY/2.0f))/height;
		
	this->mProgressBar = new ProgressBar(D3DXVECTOR2(posBarX, posBarY), sizeBarX/width, sizeBarY/height);
	this->mProgressBar->SetPercentOfProgressBarColor1(0.0f);
	this->mProgressBar->SetPercentOfProgressBarColor2(0.0f);
	this->mProgressBar->SetPercentOfProgressBackground(0.0f);
	this->mBestBallInHotZone = -1;
	this->mBestTime = -1;
		
		

	/* Set hud */
	for(int i = 0; i < 5; i++)
	{
		this->mHud[i] = NULL;
	}
	this->mTimeElapsedText = this->mGe->CreateText(	"", D3DXVECTOR2(15.0f, 10.0f), 1.0f, "Media/Fonts/1");
}

void KingOfTheHill::Intro()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;
	float textHalfWidth = (dx * (490.0f / 800.0f)) * 0.5f;

	float y = this->mGe->GetEngineParameters().windowHeight * 0.4f;

	Text*	intro = mGe->CreateText("King of the Hill",D3DXVECTOR2(dx * 0.5f - textHalfWidth + offSet,y),2.0f,"Media/Fonts/1");
	mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
	intro->SetText("");
	mGe->DeleteText(intro);
}

bool KingOfTheHill::PlaySpecific()
{	
	return GameMode::PlayLan();
	/* 
	*  or run it the way down below. 
	*  
	bool zoomOutPressed = false;
	bool zoomInPressed = false;
	this->mNet->Start(this->mServerInfo);
	this->mGe->Update();
	bool roundsLeft = true;
	int roundsPlayed = 0;

	//choose team before starting the game
	this->mTeam = this->mChooseTeamMenu->Run();
	MsgHandler::GetInstance().JoinTeam((TEAM)this->mTeam);
	//set full visibility for the total time captured images**
	this->mTotalTimeCapture->SetOpacity(1.0f);
	while(roundsLeft && this->mGe->isRunning())
	{
		this->PlayRoundLan(roundsLeft, zoomInPressed, zoomOutPressed); 
		roundsPlayed++;;
		if(roundsPlayed == this->mNumberOfRounds)
			roundsLeft = false;
	}
	*/
}


void KingOfTheHill::ShowStats()
{

}

void KingOfTheHill::ShowHud()
{
	GameMode::ShowHud();

	/*
	float percentageContinuously = (this->mPlatform->GetTimeInHotZoneContinuously()/this->mPlatform->GetMaxTimeInHotZoneContinuously())*100.0f;
	float percentageAccumulated = (this->mBalls[this->mNet->GetIndex()]->GetTimeInHotZone()/this->mPlatform->GetMaxTimeInHotZone())*100.0f;
	*/
	float percentage = 0;
	if( this->mBestBallInHotZone != -1)
	{
		float diffContinuously = this->mPlatform->GetMaxTimeInHotZoneContinuously() - this->mPlatform->GetTimeInHotZoneContinuously();
		float diffAccumulated = this->mPlatform->GetMaxTimeInHotZone() - this->mBestTime;

		if( diffAccumulated > diffContinuously)
			percentage = 100.0f - (this->mPlatform->GetTimeInHotZoneContinuously()/this->mPlatform->GetMaxTimeInHotZoneContinuously())*100.0f;
		else
			percentage = 100.0f - (this->mBestTime/this->mPlatform->GetMaxTimeInHotZone())*100.0f;
			
		if( (this->mBalls[this->mBestBallInHotZone]->GetTeam() == (int)TEAM::NOTEAM) && (this->mNet->GetIndex() == this->mBestBallInHotZone) )
		{
			this->mProgressBar->SetPercentOfProgressBarColor1(percentage);
			this->mProgressBar->SetPercentOfProgressBackground(100);
			//this->mProgressBar->SetPercentOfProgressBarColor2(0);
		}
		else if(this->mBalls[this->mBestBallInHotZone]->GetTeam() == this->mBalls[this->mNet->GetIndex()]->GetTeam())
		{
			this->mProgressBar->SetPercentOfProgressBarColor1(percentage);
			this->mProgressBar->SetPercentOfProgressBackground(100);
		}
		else
		{
			//this->mProgressBar->SetPercentOfProgressBarColor1(0);
			this->mProgressBar->SetPercentOfProgressBarColor2(percentage);
			this->mProgressBar->SetPercentOfProgressBackground(100);
		}
			
	}
	else
	{
		this->mProgressBar->HideBar();
	}
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
		if(this->mBalls[ballIndex]->GetTeam() == (int)TEAM::BLUETEAM)
			numberBlue++;
		if(this->mBalls[ballIndex]->GetTeam() == (int)TEAM::REDTEAM)
			numberRed++;
		if(this->mBalls[ballIndex]->GetTeam() == (int)TEAM::NOTEAM) // this is for free 4 all
			numberNone++;
	}
	/* if all balls belong to the same team and they are more that zero in the hotzone or one in free 4 all then update the time. */
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

		/* checking which ball that is the "best". */
		int bestIndex = -1;
		float bestTime = -1;
		for(int i = 0; i<numberInHotZone; i++)
		{
			ballIndex = arrayIndexs[i];
			
			if(i == 0 || (this->mBalls[ballIndex]->GetTimeInHotZone() > bestTime) )
			{
				bestTime = this->mBalls[ballIndex]->GetTimeInHotZone();
				bestIndex = i;
			}

		}
		this->mBestBallInHotZone = bestIndex;
		this->mBestTime = bestTime;

	}
	else
	{
		this->mBestBallInHotZone = -1;
		this->mBestTime = -1;
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
		if(i < 4)
			temp[i] = new PowerBall("Media/Ball"+ MaloW::convertNrToString((float)(i+1)) + ".obj", this->mNet->GetBall(i)->GetStartPos(), (int)GAMEMODE::KOTH);
		else
			temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetBall(i)->GetStartPos(), (int)GAMEMODE::KOTH);
		temp[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		temp[i]->SetStartForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		#if FixedTimeStep
			this->mPe->AddBody(temp[i]);
		#endif
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

void KingOfTheHill::HideHud()
{	GameMode::HideHud();
	this->mProgressBar->HideBar();
}