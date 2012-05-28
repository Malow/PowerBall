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

	this->mEnemyFlag = NULL;
	this->mFriendlyFlag = NULL;
	this->mEnemyScore = 0;
	this->mFriendlyScore = 0;
	this->mEnemyScoreText = NULL;
	this->mFriendlyScoreText = NULL;
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

	this->mEnemyFlag = NULL;
	this->mFriendlyFlag = NULL;
	this->mEnemyScore = 0;
	this->mFriendlyScore = 0;
	this->mEnemyScoreText = NULL;
	this->mFriendlyScoreText = NULL;
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
	this->mGe->DeleteText(this->mEnemyScoreText);
	this->mGe->DeleteText(this->mFriendlyScoreText);
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
		this->mEnemyFlag->GetMesh()->RotateAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f), DegreesToRadian(90));
		this->mEnemyFlag->GetMesh()->LoopSeamless();
		this->mFriendlyFlag = new FlagCTF(mGe->CreateAnimatedMesh("Media/FlagBlue.ani", D3DXVECTOR3(0, 20, -25)), D3DXVECTOR3(0, 20, -25));
		this->mFriendlyFlag->GetMesh()->RotateAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f), DegreesToRadian(-90));
		this->mFriendlyFlag->GetMesh()->LoopSeamless();
		


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
		
		this->mEnemyScoreText = this->mGe->CreateText("", D3DXVECTOR2(200.0f, 10.0f), 1.0f, "Media/Fonts/1");
		this->mFriendlyScoreText = this->mGe->CreateText("", D3DXVECTOR2(this->mGe->GetEngineParameters().windowWidth - 275, 10.0f), 1.0f, "Media/Fonts/1");
		this->mIntermediateText = this->mGe->CreateText("", D3DXVECTOR2(this->mGe->GetEngineParameters().windowWidth * 0.5f - 275, 10.0f), 1.0f, "Media/Fonts/1");
}

void CaptureTheFlag::Intro()
{
		Text*	intro = mGe->CreateText("Capture The Flag",D3DXVECTOR2(this->mGe->GetEngineParameters().windowWidth * 0.5f - 450.0f, 500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);

		//net while(this->mChooseTeamMenu->Run());

		this->mEnemyScoreText->SetText("0");
		this->mFriendlyScoreText->SetText("0");
		string totalScore = MaloW::convertNrToString(this->mNumberOfRounds);
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
	//check if enemy flag is at base
	if(this->mEnemyFlag->GetAtBase())
	{
		for(int i = 0; i < this->mNumberOfPlayers; i++)
		{
			D3DXVECTOR3 BallToFlag = D3DXVECTOR3((this->mEnemyFlag->GetMesh()->GetPosition() + D3DXVECTOR3(0, this->mBalls[i]->GetRadius(), 0)) - this->mBalls[0]->GetPosition());

			if(D3DXVec3Length(&BallToFlag) - this->mEnemyFlag->GetFlagRadius() < (this->mBalls[i]->GetRadius()))
			{
				this->mBalls[i]->AddFlag(this->mEnemyFlag);
				this->mBalls[i]->SetMaxVelocity(this->mBalls[i]->GetMaxVelocity() * 0.8f);
				this->mBalls[i]->SetRestitution(this->mBalls[i]->GetRestitution() * 2.0f);
				this->mEnemyFlag->SetAtBase(false);
			}
		}
	}
	else //otherwise check how far the flags are apart
	{
		D3DXVECTOR3 distBetweenFlags = D3DXVECTOR3(this->mEnemyFlag->GetMesh()->GetPosition() - this->mFriendlyFlag->GetMesh()->GetPosition());
		
		for(int i = 0; i < this->mNumberOfPlayers; i++)
		{
			if((D3DXVec3Length(&distBetweenFlags) - this->mEnemyFlag->GetFlagRadius() < this->mBalls[i]->GetRadius()) && this->mFriendlyFlag->GetAtBase())
			{
				this->mBalls[i]->ResetFlag();
				this->mBalls[i]->SetMaxVelocity(this->mBalls[i]->GetMaxVelocity() * 1.25f);
				this->mBalls[i]->SetRestitution(this->mBalls[i]->GetRestitution() * 0.5f);
				this->mEnemyFlag->Reset();

				int teamColor = this->mBalls[i]->GetTeamColor();
				if(teamColor == BLUETEAM)
				{
					this->mFriendlyScoreText->SetText(MaloW::convertNrToString(++this->mFriendlyScore));
				}
				else if(teamColor == REDTEAM)
				{
					this->mEnemyScoreText->SetText(MaloW::convertNrToString(++this->mEnemyScore));
				}
				else //**temp (teams not added)**
				{
					this->mFriendlyScoreText->SetText(MaloW::convertNrToString(++this->mFriendlyScore));
				}

				if(this->mFriendlyScore >= this->mNumberOfRounds || this->mEnemyScore >= this->mNumberOfRounds)
				{
					return true;
				}

				//this->mNumberOfRounds--;
				//if(this->mNumberOfRounds <= 0)
				//	return true;
			}
		}
	}

	/*
	//just copied temporarily to try - Rille
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
		temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetBall(i)->GetStartPos(), GAMEMODE::CTF);
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