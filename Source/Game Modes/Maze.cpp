#include "Maze.h"

Maze::Maze()
{
	this->mGe = NULL;
	this->mIGM = NULL;
	this->mBalls = NULL;
	this->mPlatform = NULL;
	this->mBox = NULL;
	this->mNumberOfPlayers = -1;
	this->mNumberOfRounds = -1;
	this->mGameMode = -1;
	this->mTimeElapsed = 0.0f;
}

Maze::Maze(GraphicsEngine* ge)
{
	this->mGe = ge;
	this->mNumberOfPlayers = 1;
	this->mNumberOfRounds = 1;
	this->mGameMode = CREDITS;
	this->mBox = NULL;
	this->mTimeElapsed = 0.0f;
}

Maze::~Maze()
{
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
	SAFE_DELETE(this->mBox);
}

void Maze::Initialize()
{
		D3DXVECTOR3 centerPlatform;
		centerPlatform = D3DXVECTOR3(0,20,0);
		mGe->GetCamera()->setPosition(D3DXVECTOR3(20, 55, -20));
		mGe->GetCamera()->LookAt(centerPlatform + D3DXVECTOR3(20,0,0));
		this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
		this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
		this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
		this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
		this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
		for(int i = 0; i < 5; i++)
			this->mLights[i]->SetIntensity(30.0f);
		mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,0,1));
		this->mPlatform		= new Map("Media/MazeMap.obj", centerPlatform);
		this->mBox = new Map("Media/MazeMapFrame.obj", centerPlatform);
		this->mPlatform->SetRotate(true);
		StaticMesh* fla = this->mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(13.5f,22.5f,13.5f));
		this->mPlatform->SetMeshHotZone(fla);
		this->mPlatform->SetHotZonePosition(Vector3(13.5f,22.5f,13.5f));
		this->mPlatform->SetHotZoneRadius(2.0f);
		this->mPlatform->SetShrinkValue(0.0f);
		this->mPlatform->SetRestition(0.2f);
		this->mBalls		= new PowerBall*[this->mNumberOfPlayers];
		this->mBalls[0] = new PowerBall("Media/Ball.obj", D3DXVECTOR3(-13.5f,27,-13));
		this->mBalls[0]->SetForwardVector(Vector3(0,0,1).GetD3DVec());
		this->mBalls[0]->SetKnockoutMode();
		this->mBalls[0]->SetForcePressed(this->mBalls[0]->GetForcePressed()/15.0f);
		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[0]);
		*/
}

void Maze::Intro()
{
	Text*	intro = mGe->CreateText("Maze",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
	mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
	intro->SetText("");
	mGe->DeleteText(intro);
}

void Maze::Play()
{	
		float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
		float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
		float creditSpeed = 0.05f;


		bool running = true;
		this->mGameMode = CREDITS;
		float diff;
		Text* hudR1 = mGe->CreateText("Rikard Johansson",D3DXVECTOR2(windowWidth-800,0),2.0f,"Media/Fonts/1");
		Text* hudR2 = mGe->CreateText("Marcus Lowegren",D3DXVECTOR2(windowWidth-850,60),2.0f,"Media/Fonts/1");
		Text* hudR3 = mGe->CreateText("Markus Tillman",D3DXVECTOR2(windowWidth-775,120),2.0f,"Media/Fonts/1");
		Text* hudR4 = mGe->CreateText("Didrik Axelsson",D3DXVECTOR2(windowWidth-700,180),2.0f,"Media/Fonts/1");
		Text* hudR5 = mGe->CreateText("Jerry Rahmqvist",D3DXVECTOR2(windowWidth-785,240),2.0f,"Media/Fonts/1");
		diff = mGe->Update();
		srand ( time(NULL) );
		float targetX = -10 + rand() % 21; // random angle [-10, 10] in x-axis
		float targetZ = -10 + rand() % 21; // random angle [-10, 10] in z-axis
		targetX = targetX*(PI/180.0f);
		targetZ = targetZ*(PI/180.0f);
		this->mPlatform->SetTargetAngleX(targetX);
		this->mPlatform->SetTargetAngleZ(targetZ);
		float respownAfterDeathCounter = 0.0f;
		float resetMazeCounter = 0.0f;
		while(running && this->mGe->isRunning())
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
			mBalls[0]->UpdateBallParentMode(this->mPlatform);
			mBalls[0]->Update(diff);
			Vector3 normalPlane;
			if(this->mBalls[0]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
					this->mBalls[0]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
			mBalls[0]->UpdatePost();
			mPlatform->Update(diff);
			
				

			if(this->checkWinConditions(diff))
				running = false;

			if(this->checkRespownConditions())
			{
				respownAfterDeathCounter += diff*0.001f;
				resetMazeCounter += diff*0.001f;
				if(respownAfterDeathCounter > 3.0f)
				{
					this->mPlatform->SetRotate(false);
					this->ResetMaze();
					respownAfterDeathCounter = 0.0f;
				}
				if(resetMazeCounter > 4.0f)
				{
					this->mPlatform->SetRotate(true);
					this->mBalls[0]->SetToStartPosition();
					respownAfterDeathCounter = 0.0f;
					resetMazeCounter = 0.0f;
				}
			}

			if(hudR1->GetPosition().y + diff * creditSpeed > windowHeight)
				hudR1->SetPosition(D3DXVECTOR2(hudR1->GetPosition().x, 0));
			else
				hudR1->SetPosition(D3DXVECTOR2(hudR1->GetPosition().x, hudR1->GetPosition().y + diff * creditSpeed));

			if(hudR2->GetPosition().y + diff * creditSpeed > windowHeight)
				hudR2->SetPosition(D3DXVECTOR2(hudR2->GetPosition().x, 0));
			else
				hudR2->SetPosition(D3DXVECTOR2(hudR2->GetPosition().x, hudR2->GetPosition().y + diff * creditSpeed));

			if(hudR3->GetPosition().y + diff * creditSpeed > windowHeight)
				hudR3->SetPosition(D3DXVECTOR2(hudR3->GetPosition().x, 0));
			else
				hudR3->SetPosition(D3DXVECTOR2(hudR3->GetPosition().x, hudR3->GetPosition().y + diff * creditSpeed));

			if(hudR4->GetPosition().y + diff * creditSpeed > windowHeight)
				hudR4->SetPosition(D3DXVECTOR2(hudR4->GetPosition().x, 0));
			else
				hudR4->SetPosition(D3DXVECTOR2(hudR4->GetPosition().x, hudR4->GetPosition().y + diff * creditSpeed));

			if(hudR5->GetPosition().y + diff * creditSpeed > windowHeight)
				hudR5->SetPosition(D3DXVECTOR2(hudR5->GetPosition().x, 0));
			else
				hudR5->SetPosition(D3DXVECTOR2(hudR5->GetPosition().x, hudR5->GetPosition().y + diff * creditSpeed));
		}
		hudR1->SetText("");
		hudR2->SetText("");
		hudR3->SetText("");
		hudR4->SetText("");
		hudR5->SetText("");
		mGe->DeleteText(hudR1);
		mGe->DeleteText(hudR2);
		mGe->DeleteText(hudR3);
		mGe->DeleteText(hudR4);
		mGe->DeleteText(hudR5);
}

void Maze::ShowStats()
{
	mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,1,0));
}

bool Maze::checkWinConditions(float dt)
{
	Text* hudR10 = NULL;
	string s;
	if(this->mPlatform->IsInHotZone(this->mBalls[0]->GetPositionVector3(), this->mBalls[0]->GetRadius()))
	{
		hudR10 = mGe->CreateText("",D3DXVECTOR2(200,400),1.0f,"Media/Fonts/1");
		s = "Mission Accomplished:";
		hudR10->SetText(s);
		while(dt < 4000)
			dt += mGe->Update();
		hudR10->SetText("");
		mGe->DeleteText(hudR10);
		return true;
	}
	return false;
}

bool Maze::checkRespownConditions()
{
		
		Vector3 pos = this->mPlatform->GetMesh()->GetPosition();
		D3DXMATRIX quat;
		D3DXMatrixRotationQuaternion(&quat, &this->mPlatform->GetMesh()->GetRotation()); 
	
		Matrix4 rotate(quat);
		rotate.TransposeThis();
						
		Vector3 posBall = this->mBalls[0]->GetMesh()->GetPosition();
		
		Vector3 newCoord = rotate.GetInverse() * (posBall - pos - Vector3(0,5.0f,0));
		/*
		string s = MaloW::convertNrToString(newCoord.y);
		this->mText->SetText(s);
		*/
		if(newCoord.y < -6.0f)
			return true;
		else
			return false;
}

void Maze::ResetMaze()
{
	this->mPlatform->GetMesh()->ResetRotationAndScale();
	this->mPlatform->ResetXZAngles();
}