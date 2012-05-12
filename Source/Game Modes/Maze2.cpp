#include "Maze2.h"

Maze2::Maze2()
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

Maze2::Maze2(GraphicsEngine* ge)
{
	this->mGe = ge;
	this->mNumberOfPlayers = 1;
	this->mNumberOfRounds = 1;
	this->mGameMode = CREDITS2;
	this->mTimeElapsed = 0.0f;
}

Maze2::~Maze2()
{
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
}

void Maze2::Initialize()
{
		D3DXVECTOR3 centerPlatform;
		centerPlatform = D3DXVECTOR3(0,20,0);
		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 55, 0));
		mGe->GetCamera()->LookAt(centerPlatform);
		this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
		this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
		this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
		this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
		this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
		for(int i = 0; i < 5; i++)
			this->mLights[i]->SetIntensity(30.0f);
		mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,0,1));
		this->mPlatform		= new Map("Media/MazeMap.obj", centerPlatform);
		
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
		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[0]);
		*/
}

void Maze2::Intro()
{
		Text*	intro = mGe->CreateText("Maze II",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

void Maze2::Play()
{	
		bool running = true;
		this->mGameMode = CREDITS2;
		float diff;
		Text* hudR1 = mGe->CreateText("",D3DXVECTOR2(20,20),1.0f,"Media/Fonts/1");
		Text* hudR2 = mGe->CreateText("",D3DXVECTOR2(20,140),1.0f,"Media/Fonts/1");
		Text* hudR3 = mGe->CreateText("",D3DXVECTOR2(20,180),1.0f,"Media/Fonts/1");
		Text* hudR4 = mGe->CreateText("",D3DXVECTOR2(20,500),1.0f,"Media/Fonts/1");
		Text* hudR5 = mGe->CreateText("",D3DXVECTOR2(20,540),1.0f,"Media/Fonts/1");
		Text* hudR6 = mGe->CreateText("",D3DXVECTOR2(20,580),1.0f,"Media/Fonts/1");
		Text* hudR7 = mGe->CreateText("",D3DXVECTOR2(20,620),1.0f,"Media/Fonts/1");
		Text* hudR8 = mGe->CreateText("",D3DXVECTOR2(20,660),1.0f,"Media/Fonts/1");
		Text* hudR9 = mGe->CreateText("",D3DXVECTOR2(20,700),1.0f,"Media/Fonts/1");
		Text* hudR10 = mGe->CreateText("",D3DXVECTOR2(200,400),2.0f,"Media/Fonts/1");
		string s;
		s = "Credits: OldStyle";
		hudR1->SetText(s);
		this->mPlatform->SetMaxAngleX(10.0f*(PI/180.0f));
		this->mPlatform->SetMaxAngleZ(10.0f*(PI/180.0f));
		diff = mGe->Update();
	
		while(running)
		{
		
			diff = mGe->Update();
			MaloW::Debug(diff);
			if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
				running = false;
		
			if(mGe->GetKeyListener()->IsPressed('W'))
				this->mPlatform->RotateX(diff);
			if(mGe->GetKeyListener()->IsPressed('S'))
				this->mPlatform->RotateX(-diff);
			if(mGe->GetKeyListener()->IsPressed('A'))
				this->mPlatform->RotateZ(diff);
			if(mGe->GetKeyListener()->IsPressed('D'))
				this->mPlatform->RotateZ(-diff);
			if(mGe->GetKeyListener()->IsClicked(2))
				mBalls[0]->AddForce(Vector3(0,diff*(11.0f/6.0f),0));
			// cheating is gooood!!!
			if(mGe->GetKeyListener()->IsPressed(VK_UP))
				mBalls[0]->AddForceForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsPressed(VK_DOWN))
				mBalls[0]->AddForceOppositeForwardDirection(diff);
			if(mGe->GetKeyListener()->IsPressed(VK_LEFT))
				mBalls[0]->AddForceLeftOfForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsPressed(VK_RIGHT))
				mBalls[0]->AddForceRightOfForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsClicked(2))
				mBalls[0]->AddForce(Vector3(0,diff*(11.0f/6.0f),0));
			mBalls[0]->Update(diff);
			Vector3 normalPlane;
			if(this->mBalls[0]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
					this->mBalls[0]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
			mBalls[0]->UpdatePost();
			mPlatform->Update(diff);
			if(!this->mGe->isRunning())
				running = false;
		
		
		
			s = "X = " + MaloW::convertNrToString(floor(this->mPlatform->GetAngleX()*(180.0f/PI)));
			hudR2->SetText(s);
			s = "Z = " + MaloW::convertNrToString(floor(this->mPlatform->GetAngleZ()*(180.0f/PI)));
			hudR3->SetText(s);
		
		
			s = "Position ball:";
			hudR4->SetText(s);
			s = "X = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().x)/10.0f) + " Y = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().y)/10.0f) + " Z = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().z)/10.0f);
			hudR5->SetText(s);
			s = "Position hotzone flag: ";
			hudR6->SetText(s);
			s =  "X = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().x)/10.0f) + " Y = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().y)/10.0f) + " Z = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().z)/10.0f);
			hudR7->SetText(s);
			//Vector3 distVec = this->mPlatform->GetHotZonePosition() - this-mBalls[0]->GetPositionVector3();
			//float dist = distVec.GetLength();
			//s = "Distance to flag: " +  MaloW::convertNrToString(floor(10.0f*dist)/10.0f);
			if(this->checkWinConditions(diff))
				running = false;
			
		}
			mGe->DeleteText(hudR1);
			mGe->DeleteText(hudR2);
			mGe->DeleteText(hudR3);
			mGe->DeleteText(hudR4);
			mGe->DeleteText(hudR5);
			mGe->DeleteText(hudR6);
			mGe->DeleteText(hudR7);
			mGe->DeleteText(hudR8);

	
}

void Maze2::ShowStats()
{
	mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,1,0));
}

bool Maze2::checkWinConditions(float dt)
{
	Text* hudR10 = NULL;
	string s;
	if(this->mPlatform->IsInHotZone(this->mBalls[0]->GetPositionVector3(), this->mBalls[0]->GetRadius()))
	{
		hudR10 = mGe->CreateText("",D3DXVECTOR2(200,400),2.0f,"Media/Fonts/1");
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