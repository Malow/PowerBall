#include "Maze2.h"

Maze2::Maze2()
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
	this->mText = this->mGe->CreateText("", D3DXVECTOR2(700,700),2.0f,"Media/Fonts/1");
}

Maze2::Maze2(GraphicsEngine* ge)
{
	this->mGe = ge;
	this->mBox = NULL;
	this->mNumberOfPlayers = 1;
	this->mNumberOfRounds = 1;
	this->mGameMode = CREDITS2;
	this->mTimeElapsed = 0.0f;
	this->mText = this->mGe->CreateText("", D3DXVECTOR2(700,700),2.0f,"Media/Fonts/1");
}

Maze2::~Maze2()
{
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
	SAFE_DELETE(this->mBox);
	for(int i = 0; i<10; i++)
		mGe->DeleteText(this->mHud[i]);
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
		this->mBox = new Map("Media/MazeMapFrame.obj", centerPlatform);
		
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
		this->mBalls[0]->SetAcceleration(this->mBalls[0]->GetAcceleration()*3.0f);
		this->mBalls[0]->SetForcePressed(this->mBalls[0]->GetForcePressed()/15.0f);
		
		/* Set hud */
		this->mHud[0] = mGe->CreateText("",D3DXVECTOR2(20,20),1.0f,"Media/Fonts/1");
		this->mHud[1] = mGe->CreateText("",D3DXVECTOR2(20,140),1.0f,"Media/Fonts/1");
		this->mHud[2] = mGe->CreateText("",D3DXVECTOR2(20,180),1.0f,"Media/Fonts/1");
		this->mHud[3] = mGe->CreateText("",D3DXVECTOR2(20,500),1.0f,"Media/Fonts/1");
		this->mHud[4] = mGe->CreateText("",D3DXVECTOR2(20,540),1.0f,"Media/Fonts/1");
		this->mHud[5] = mGe->CreateText("",D3DXVECTOR2(20,580),1.0f,"Media/Fonts/1");
		this->mHud[6] = mGe->CreateText("",D3DXVECTOR2(20,620),1.0f,"Media/Fonts/1");
		this->mHud[7] = mGe->CreateText("",D3DXVECTOR2(20,660),1.0f,"Media/Fonts/1");
		this->mHud[8] = mGe->CreateText("",D3DXVECTOR2(20,700),1.0f,"Media/Fonts/1");
		this->mHud[9] = mGe->CreateText("",D3DXVECTOR2(200,400),2.0f,"Media/Fonts/1");
		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[0]);
		*/

		this->mTimeElapsedText = this->mGe->CreateText(	"", D3DXVECTOR2(15.0f, 10.0f), 1.0f, "Media/Fonts/1");
}

void Maze2::Intro()
{
		Text*	intro = mGe->CreateText("Maze II",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

bool Maze2::PlaySpecific()
{	
		bool running = true;
		this->mGameMode = CREDITS2;
		float diff;
		
		string s;
		s = "Credits: OldStyle";
		this->mHud[0]->SetText(s);
		this->mPlatform->SetMaxAngleX(10.0f*(PI/180.0f));
		this->mPlatform->SetMaxAngleZ(10.0f*(PI/180.0f));
		diff = mGe->Update();
		float respownAfterDeathCounter = 0.0f;
		float resetMazeCounter = 0.0f;
		while(running)
		{
		
			diff = mGe->Update();
			this->mTimeElapsed += diff * 0.001f;
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
			mBalls[0]->UpdateBallParentMode(this->mPlatform);
			mBalls[0]->Update(diff);
			Vector3 normalPlane;
			if(this->mBalls[0]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
					this->mBalls[0]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
			mBalls[0]->UpdatePost();
			mPlatform->Update(diff);
			if(!this->mGe->isRunning())
				running = false;
		
		
		
			s = "X = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetAngleX()*(180.0f/PI))/10.0f);
			this->mHud[1]->SetText(s);
			s = "Z = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetAngleZ()*(180.0f/PI))/10.0f);
			this->mHud[2]->SetText(s);
		
		
			s = "Position ball:";
			this->mHud[3]->SetText(s);
			s = "X = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().x)/10.0f) + " Y = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().y)/10.0f) + " Z = " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetPosition().z)/10.0f);
			this->mHud[4]->SetText(s);
			s = "Position hotzone flag: ";
			this->mHud[5]->SetText(s);
			s =  "X = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().x)/10.0f) + " Y = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().y)/10.0f) + " Z = " + MaloW::convertNrToString(floor(10.0f*this->mPlatform->GetHotZonePosition().z)/10.0f);
			this->mHud[6]->SetText(s);
			//Vector3 distVec = this->mPlatform->GetHotZonePosition() - this-mBalls[0]->GetPositionVector3();
			//float dist = distVec.GetLength();
			//s = "Distance to flag: " +  MaloW::convertNrToString(floor(10.0f*dist)/10.0f);
			if(this->checkWinConditions(diff))
				running = false;
			if(this->checkRespownConditions())
			{
				respownAfterDeathCounter += diff*0.001f;
				resetMazeCounter += diff*0.001f;
				if(respownAfterDeathCounter > 3.0f)
				{
					this->ResetMaze();
					respownAfterDeathCounter = 0.0f;
				}
				if(resetMazeCounter > 4.0f)
				{
					this->mBalls[0]->SetToStartPosition();
					respownAfterDeathCounter = 0.0f;
					resetMazeCounter = 0.0f;
				}
			}
			
			this->ShowHud();
		}
	return false;
}

void Maze2::ShowStats()
{
	mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,1,0));
}

void Maze2::ShowHud()
{
	//show time elapsed
	float tmp = floor(this->mTimeElapsed * 10.0f) / 10.0f;
	this->mTimeElapsedText->SetText(MaloW::convertNrToString(tmp));
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

bool Maze2::checkRespownConditions()
{
		
		Vector3 pos = this->mPlatform->GetMesh()->GetPosition();
		D3DXMATRIX quat;
		D3DXMatrixRotationQuaternion(&quat, &this->mPlatform->GetMesh()->GetRotation()); 
	
		Matrix4 rotate(quat);
		rotate.TransposeThis();
						
		Vector3 posBall = this->mBalls[0]->GetMesh()->GetPosition();
		
		Vector3 newCoord = rotate.GetInverse() * (posBall - pos - Vector3(0,5.0f,0));
		
		
		if(newCoord.y < -6.0f)
			return true;
		else
			return false;
}

void Maze2::ResetMaze()
{
	this->mPlatform->GetMesh()->ResetRotationAndScale();
	this->mPlatform->ResetXZAngles();
}

