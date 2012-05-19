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
		SAFE_DELETE(this->mChooseTeamMenu);
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
		
		CTFInfo* gmi = (CTFInfo*)this->mServerInfo.GetGameModeInfo();
		this->mNumberOfRounds = gmi->GetNumRounds();

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
		this->mChooseTeamMenu = new ChooseTeamMenu(this->mGe);
		
}

void CaptureTheFlag::Intro()
{
		Text*	intro = mGe->CreateText("CaptureTheFlag",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

void CaptureTheFlag::PlaySpecific()
{	
		GameMode::PlayLan();
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
	/*if(this->mNet->IsServer())
	{*/
		this->mNet->SetFlagPos(this->mFriendlyFlag->GetMesh()->GetPosition(), 0);
		this->mNet->SetFlagPos(this->mEnemyFlag->GetMesh()->GetPosition(), 1);
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
		temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetBall(i)->GetStartPos());
		temp[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		temp[i]->SetStartForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		
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

}