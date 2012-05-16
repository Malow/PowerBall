#include "Maze.h"

Maze::Maze()
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

Maze::Maze(GraphicsEngine* ge)
{
	this->mGe = ge;
	this->mNumberOfPlayers = 1;
	this->mNumberOfRounds = 1;
	this->mGameMode = CREDITS;
	this->mTimeElapsed = 0.0f;
}

Maze::~Maze()
{
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
	for(int i = 0;i<5;i++) 
		mGe->DeleteText(this->mHud[i]);
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
		this->mPlatform->SetRotate(true);
		StaticMesh* fla = this->mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(13.5f,22.5f,13.5f));
		this->mPlatform->SetMeshHotZone(fla);
		this->mPlatform->SetHotZonePosition(Vector3(13.5f,22.5f,13.5f));
		this->mPlatform->SetHotZoneRadius(2.0f);
		this->mPlatform->SetShrinkValue(0.0f);
		this->mPlatform->SetRestition(0.2f);
		this->mBalls		= new PowerBall*[this->mNumberOfPlayers];
		this->mBalls[0] = new PowerBall("Media/Ball.obj", D3DXVECTOR3(-13.5f,24,-13));
		this->mBalls[0]->SetForwardVector(Vector3(0,0,1).GetD3DVec());
		this->mBalls[0]->SetKnockoutMode();
		this->mBalls[0]->SetForcePressed(this->mBalls[0]->GetForcePressed()/15.0f);
		
		/* Set hud */
		float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
		float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
		
		this->mHud[0] = mGe->CreateText("",D3DXVECTOR2(windowWidth-800,0),2.0f,"Media/Fonts/1");
		this->mHud[1] = mGe->CreateText("",D3DXVECTOR2(windowWidth-850,60),2.0f,"Media/Fonts/1");
		this->mHud[2] = mGe->CreateText("",D3DXVECTOR2(windowWidth-775,120),2.0f,"Media/Fonts/1");
		this->mHud[3] = mGe->CreateText("",D3DXVECTOR2(windowWidth-700,180),2.0f,"Media/Fonts/1");
		this->mHud[4] = mGe->CreateText("",D3DXVECTOR2(windowWidth-785,240),2.0f,"Media/Fonts/1");
		

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

void Maze::PlaySpecific()
{	
		this->mHud[0]->SetText("Rikard Johansson");
		this->mHud[1]->SetText("Marcus Lowegren");
		this->mHud[2]->SetText("Markus Tillman");
		this->mHud[3]->SetText("Didrik Axelsson");
		this->mHud[4]->SetText("Jerry Rahmqvist");
		float creditSpeed = 0.05f;
		float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
		float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
		bool running = true;
		this->mGameMode = CREDITS;
		float diff;
		
		diff = mGe->Update();
		srand ( time(NULL) );
		float targetX = -10 + rand() % 21; // random angle [-10, 10] in x-axis
		float targetZ = -10 + rand() % 21; // random angle [-10, 10] in z-axis
		targetX = targetX*(PI/180.0f);
		targetZ = targetZ*(PI/180.0f);
		this->mPlatform->SetTargetAngleX(targetX);
		this->mPlatform->SetTargetAngleZ(targetZ);
		while(running && mGe->isRunning())
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
			mBalls[0]->Update(diff);
			Vector3 normalPlane;
			if(this->mBalls[0]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
					this->mBalls[0]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
			mBalls[0]->UpdatePost();
			mPlatform->Update(diff);
			
			if(!mBalls[0]->IsAlive())
				running = false;		

			if(this->checkWinConditions(diff))
				running = false;


			if(this->mHud[0]->GetPosition().y + diff * creditSpeed > windowHeight)
				this->mHud[0]->SetPosition(D3DXVECTOR2(this->mHud[0]->GetPosition().x, 0));
			else
				this->mHud[0]->SetPosition(D3DXVECTOR2(this->mHud[0]->GetPosition().x, this->mHud[0]->GetPosition().y + diff * creditSpeed));

			if(this->mHud[1]->GetPosition().y + diff * creditSpeed > windowHeight)
				this->mHud[1]->SetPosition(D3DXVECTOR2(this->mHud[1]->GetPosition().x, 0));
			else
				this->mHud[1]->SetPosition(D3DXVECTOR2(this->mHud[1]->GetPosition().x, this->mHud[1]->GetPosition().y + diff * creditSpeed));

			if(this->mHud[2]->GetPosition().y + diff * creditSpeed > windowHeight)
				this->mHud[2]->SetPosition(D3DXVECTOR2(this->mHud[2]->GetPosition().x, 0));
			else
				this->mHud[2]->SetPosition(D3DXVECTOR2(this->mHud[2]->GetPosition().x, this->mHud[2]->GetPosition().y + diff * creditSpeed));

			if(this->mHud[3]->GetPosition().y + diff * creditSpeed > windowHeight)
				this->mHud[3]->SetPosition(D3DXVECTOR2(this->mHud[3]->GetPosition().x, 0));
			else
				this->mHud[3]->SetPosition(D3DXVECTOR2(this->mHud[3]->GetPosition().x, this->mHud[3]->GetPosition().y + diff * creditSpeed));

			if(this->mHud[4]->GetPosition().y + diff * creditSpeed > windowHeight)
				this->mHud[4]->SetPosition(D3DXVECTOR2(this->mHud[4]->GetPosition().x, 0));
			else
				this->mHud[4]->SetPosition(D3DXVECTOR2(this->mHud[4]->GetPosition().x, this->mHud[4]->GetPosition().y + diff * creditSpeed));
		}
		
}

void Maze::ShowStats()
{
	mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,1,0));
}

bool Maze::checkWinConditions(float dt)
{
	Text* hud10 = NULL;
	string s;
	if(this->mPlatform->IsInHotZone(this->mBalls[0]->GetPositionVector3(), this->mBalls[0]->GetRadius()))
	{
		hud10 = mGe->CreateText("",D3DXVECTOR2(200,400),1.0f,"Media/Fonts/1");
		s = "Mission Accomplished:";
		hud10->SetText(s);
		while(dt < 4000)
			dt += mGe->Update();
		hud10->SetText("");
		mGe->DeleteText(hud10);
		return true;
	}
	return false;
}

void Maze::ShowHud()
{

}