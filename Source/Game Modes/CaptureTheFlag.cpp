#include "CaptureTheFlag.h"
#include "..\Game Objects\FlagCTF.h"
#include "..\Physics\PhysicsEngine.h"

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

	this->mTeam = (int)TEAM::NOTEAM;

	this->mEnemyFlag = NULL;
	this->mFriendlyFlag = NULL;	
	this->mRedFlag = NULL;		
	this->mBlueFlag = NULL;	
	this->mRedScore = 0;
	this->mBlueScore = 0;
	this->mRedScoreText = NULL;
	this->mBlueScoreText = NULL;
	this->mIntermediateText = NULL;
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

	this->mTeam = (int)TEAM::NOTEAM;

	this->mEnemyFlag = NULL;
	this->mFriendlyFlag = NULL;
	this->mRedFlag = NULL;		
	this->mBlueFlag = NULL;	
	this->mRedScore = 0;
	this->mBlueScore = 0;
	this->mRedScoreText = NULL;
	this->mBlueScoreText = NULL;
	this->mIntermediateText = NULL;
}

CaptureTheFlag::~CaptureTheFlag()
{
	//Game mode specific:
	//ge
	SAFE_DELETE(this->mIGM);
	//network
	SAFE_DELETE(this->mPlatform); //**
	/*for(int i = 0; i < this->mNumberOfPlayers; i++) 
	{
		//SAFE_DELETE(this->mBalls[i]);
		this->mGe->DeleteStaticMesh(this->mBalls[i]->GetMesh());
    }*/
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
	SAFE_DELETE(this->mChooseTeamMenu);
	//text hud
	for(int i = 0;i<17; i++) //SIZE_HUD
		mGe->DeleteText(this->mHud[i]);
	//text timeelapsedtext
	if(this->mPe)
	{
		#if FixedTimeStep
		/*
			for(int i = 0;i<this->mNumberOfPlayers; i++)
				this->mPe->RemoveBody(this->mBalls[i]);
			this->mPe->RemoveMap(this->mPlatform);*/
			SAFE_DELETE(this->mPe);
			this->mPe = NULL;
		#endif
	}

	//CTF specific:
	this->mGe->DeleteAnimatedMesh(this->mFriendlyFlag->GetMesh());
	SAFE_DELETE(this->mFriendlyFlag);
	this->mGe->DeleteAnimatedMesh(this->mEnemyFlag->GetMesh());
	SAFE_DELETE(this->mEnemyFlag);
	this->mGe->DeleteText(this->mRedScoreText);
	this->mGe->DeleteText(this->mBlueScoreText);
	this->mGe->DeleteText(this->mIntermediateText);
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
		
		CTFInfo* gmi = (CTFInfo*)this->mServerInfo.GetGameModeInfo();
		this->mNumberOfRounds = gmi->GetNumRounds();

		this->mPlatform		= new Map("Media/CTFMap1.obj", centerPlatform);
		this->mPlatform->SetShrinkValue(0.0f);
		
		this->mEnemyFlag = new FlagCTF(mGe->CreateAnimatedMesh("Media/FlagRed.ani", D3DXVECTOR3(0, 20, 25)), D3DXVECTOR3(0, 20, 25));
		this->mEnemyFlag->GetMesh()->Scale(0.75f);
		this->mEnemyFlag->GetMesh()->RotateAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f), DegreesToRadian(90));
		this->mEnemyFlag->GetMesh()->LoopSeamless();
		this->mFriendlyFlag = new FlagCTF(mGe->CreateAnimatedMesh("Media/FlagBlue.ani", D3DXVECTOR3(0, 20, -25)), D3DXVECTOR3(0, 20, -25));
		this->mFriendlyFlag->GetMesh()->Scale(0.75f);
		this->mFriendlyFlag->GetMesh()->RotateAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f), DegreesToRadian(-90));
		this->mFriendlyFlag->GetMesh()->LoopSeamless();
		
		this->mRedFlag = this->mEnemyFlag;
		this->mBlueFlag = this->mFriendlyFlag;

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
		this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -25)); 
		this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 25));
		this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
		this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
		for(int i = 0; i < 5; i++)
		{
			this->mLights[i]->SetIntensity(30.0f);
			this->mLights[i]->SetLookAt(D3DXVECTOR3(0,10,0));
		}

		this->mIGM	= new InGameMenu(this->mGe);
		
		//this->mChooseTeamMenu = new ChooseTeamMenu(this->mGe); //generate memory leaks
		
		this->mTimeElapsedText = this->mGe->CreateText(	"", D3DXVECTOR2(15.0f, 10.0f), 1.0f, "Media/Fonts/1");
		
		float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
		float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
		float dx = (windowHeight * 4.0f) / 3.0f;
		float offSet = (windowWidth - dx) / 2.0f;

		float textHalfWidth = dx * (200.0f / 800.0f);
		float x = this->mGe->GetEngineParameters().windowWidth * 0.5f - textHalfWidth;
		float y = 10.0f;
		this->mIntermediateText	 = this->mGe->CreateText("", D3DXVECTOR2(dx * 0.5f - textHalfWidth + offSet, y), 1.0f, "Media/Fonts/1");
		x = this->mGe->GetEngineParameters().windowWidth * 0.5f - textHalfWidth - this->mGe->GetEngineParameters().windowWidth * 0.1f - this->mGe->GetEngineParameters().windowWidth * 0.025f;
		this->mRedScoreText	 = this->mGe->CreateText("", D3DXVECTOR2(x, y), 1.0f, "Media/Fonts/1");
		x = this->mGe->GetEngineParameters().windowWidth * 0.5f + textHalfWidth + this->mGe->GetEngineParameters().windowWidth * 0.1f;
		this->mBlueScoreText = this->mGe->CreateText("", D3DXVECTOR2(x, y), 1.0f, "Media/Fonts/1");
		
		x = this->mGe->GetEngineParameters().windowWidth * 0.5f - this->mGe->GetEngineParameters().windowWidth * 0.4f;
		y = this->mGe->GetEngineParameters().windowHeight * 0.4f;
		this->mHud[0] = this->mGe->CreateText("", D3DXVECTOR2(x, y), 2.0f, "Media/Fonts/1");
}

void CaptureTheFlag::Intro()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;
	float textHalfWidth = (dx * (675.0f / 800.0f)) * 0.5f;

	float y = this->mGe->GetEngineParameters().windowHeight * 0.4f;
	Text* intro = mGe->CreateText("Capture The Flag", D3DXVECTOR2(dx * 0.5f - textHalfWidth + offSet, y), 2.0f,"Media/Fonts/1");
	mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
	intro->SetText("");
	mGe->DeleteText(intro);

	//net while(this->mChooseTeamMenu->Run());

	this->mRedScoreText->SetText("0");
	this->mBlueScoreText->SetText("0");
	string totalScore = MaloW::convertNrToString((float)this->mNumberOfRounds);
	this->mIntermediateText->SetText("Flags captured of " + totalScore);
}

bool CaptureTheFlag::PlaySpecific()
{	
	return GameMode::PlayLan();
}

void CaptureTheFlag::ShowStats()
{

}

bool CaptureTheFlag::checkWinConditions(float dt)
{
	int team = -1;
	for(int i = 0; i < this->mNumberOfPlayers; i++)
	{
		team = this->mBalls[i]->GetTeam(); 

		if(team == (int)TEAM::REDTEAM) 
		{
			if(this->mFriendlyFlag != this->mRedFlag)
			{	
				this->mFriendlyFlag = this->mRedFlag;
				this->mEnemyFlag = this->mBlueFlag;
			}
		}
		else if(team == (int)TEAM::BLUETEAM)
		{
			if(this->mFriendlyFlag != this->mRedFlag)
			{	
				this->mFriendlyFlag = this->mBlueFlag;
				this->mEnemyFlag = this->mRedFlag;
			}
		}
		
		//check if enemy flag is at it's base
		if(this->mEnemyFlag->GetAtBase())
		{
			//check the distance between it and the ball
			D3DXVECTOR3 ballToFlag = D3DXVECTOR3((this->mEnemyFlag->GetMesh()->GetPosition() + D3DXVECTOR3(0, this->mBalls[i]->GetRadius(), 0)) - this->mBalls[i]->GetPosition());
			//if close enough, pick it up and set a penalty for holding it
			if(D3DXVec3Length(&ballToFlag) - this->mEnemyFlag->GetFlagRadius() < (this->mBalls[i]->GetRadius()))
			{
				this->mBalls[i]->AddFlag(this->mEnemyFlag);
				this->mBalls[i]->SetMaxVelocity(this->mBalls[i]->GetMaxVelocity() * 0.8f);
				this->mBalls[i]->SetRestitution(this->mBalls[i]->GetRestitution() * 2.0f);
				this->mEnemyFlag->SetAtBase(false);
			}
		}
		else if(this->mFriendlyFlag->GetAtBase()) //otherwise check if the enemy flag can be captured (friendly flag at base)
		{
			//check if ball has flag
			if(this->mBalls[i]->HasFlag())
			{
				//check the distance between the friendly flag and the ball
				D3DXVECTOR3 ballToFlag = D3DXVECTOR3((this->mFriendlyFlag->GetMesh()->GetPosition() + D3DXVECTOR3(0, this->mBalls[i]->GetRadius(), 0)) - this->mBalls[i]->GetPosition());
				//if close enough, capture flag & reset it and the penalty for holding it
				if(D3DXVec3Length(&ballToFlag) - this->mFriendlyFlag->GetFlagRadius() < (this->mBalls[i]->GetRadius()))
				{
					this->mBalls[i]->ResetFlag();
					this->mBalls[i]->SetMaxVelocity(this->mBalls[i]->GetMaxVelocity() * 1.25f);
					this->mBalls[i]->SetRestitution(this->mBalls[i]->GetRestitution() * 0.5f);
					this->mEnemyFlag->Reset();

					//update score
					if(team == (int)TEAM::REDTEAM)
					{
						this->mRedScoreText->SetText(MaloW::convertNrToString((float)++this->mRedScore));
					}
					else if(team == (int)TEAM::BLUETEAM)
					{
						this->mBlueScoreText->SetText(MaloW::convertNrToString((float)++this->mBlueScore));
					}

					//if a team has scored enough, announce winner and return
					if(this->mRedScore >= this->mNumberOfRounds)
					{
						this->mHud[0]->SetText("Red team wins!");
						while(dt < 2000)
						{
							dt += mGe->Update();
						}
						this->mHud[0]->SetText("");

						return true;
					}
					else if(this->mBlueScore >= this->mNumberOfRounds)
					{
						this->mHud[0]->SetText("Blue team wins!");
						while(dt < 2000)
						{
							dt += mGe->Update();
						}
						this->mHud[0]->SetText("");

						return true;
					}
				}
			}
		}
	}
		

	/*if(this->mNet->IsServer())
	{*/
		//this->mNet->SetFlagPos(this->mFriendlyFlag->GetMesh()->GetPosition(), 0);
		//this->mNet->SetFlagPos(this->mEnemyFlag->GetMesh()->GetPosition(), 1);
	/*}
	else
	{
		this->mFriendlyFlag->SetPosition(this->mNet->GetFlagPos(1));
		this->mEnemyFlag->SetPosition(this->mNet->GetFlagPos(0));
	}*/

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
		if(i < 4)
			temp[i] = new PowerBall("Media/Ball"+ MaloW::convertNrToString((float)(i + 1)) + ".obj", this->mNet->GetBall(i)->GetStartPos(), (int)GAMEMODE::CTF);
		else
			temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetBall(i)->GetStartPos(), (int)GAMEMODE::CTF);
		temp[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		temp[i]->SetStartForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		#if FixedTimeStep
			this->mPe->AddBody(temp[i]);
		#endif
		
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

void CaptureTheFlag::ShowHud()
{
	GameMode::ShowHud();
}

void CaptureTheFlag::HideHud()
{
	GameMode::HideHud();
	this->mTimeElapsedText->SetText("");
}