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
	this->mBox = NULL;
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
	SAFE_DELETE(this->mBox);
	for(int i = 0;i<5;i++) 
		mGe->DeleteText(this->mHud[i]);
	this->mCb->RemoveFromRenderer(this->mGe);
	delete this->mCb;
}

void Maze::Initialize()
{
		float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
		float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
		float dx = (windowHeight * 4.0f) / 3.0f;
		float offSet = (windowWidth - dx) / 2.0f;

		D3DXVECTOR3 centerPlatform;
		centerPlatform = D3DXVECTOR3(0,20,0);
		mGe->GetCamera()->setPosition(D3DXVECTOR3(-(dx * (20.0f / 1200.0f)), 55, -20));
		mGe->GetCamera()->LookAt(centerPlatform + D3DXVECTOR3(-(dx * (20.0f / 1200.0f)),0,0));
		this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
		this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
		this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
		this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
		this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
		for(int i = 0; i < 5; i++)
			this->mLights[i]->SetIntensity(30.0f);
		mGe->GetCamera()->setUpVector(D3DXVECTOR3(0,0,1));
		this->mPlatform		= new Map("Media/MazeMap.obj", centerPlatform);
		this->mBox = new Map("Media/MazeMapFrame.obj", centerPlatform + D3DXVECTOR3(0,1,0) );
		this->mPlatform->SetRotate(true);
		StaticMesh* fla = this->mGe->CreateStaticMesh("Media/Flag.obj", D3DXVECTOR3(13.5f,22.5f,13.5f));
		this->mPlatform->SetMeshHotZone(fla);
		this->mPlatform->SetHotZonePosition(Vector3(13.5f,22.5f,13.5f));
		this->mPlatform->SetHotZoneRadius(2.0f);
		this->mPlatform->SetShrinkValue(0.0f);
		this->mPlatform->SetRestition(0.2f);
		this->mBalls		= new PowerBall*[this->mNumberOfPlayers];
		this->mBalls[0] = new PowerBall("Media/Ball.obj", D3DXVECTOR3(-13.0f,27,-13));
		this->mBalls[0]->SetForwardVector(Vector3(0,0,1).GetD3DVec());
		this->mBalls[0]->SetKnockoutMode();
		this->mBalls[0]->SetAcceleration(this->mBalls[0]->GetAcceleration()*3.0f);
		this->mBalls[0]->SetForcePressed(this->mBalls[0]->GetForcePressed()/15.0f);
		
		/*
		this->mBalls[0]->SetForwardVector(Vector3(0,0,1).GetD3DVec());
		this->mBalls[0]->SetKnockoutMode();
		this->mBalls[0]->SetForcePressed(this->mBalls[0]->GetForcePressed()/15.0f);
		*/

		/* Set hud */
		this->mWindowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
		this->mWindowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
		
		
		this->mHud[0] = mGe->CreateText("",D3DXVECTOR2(0,0),1.0f,"Media/Fonts/1");
		this->mHud[1] = mGe->CreateText("",D3DXVECTOR2(0,60),1.0f,"Media/Fonts/1");
		this->mHud[2] = mGe->CreateText("",D3DXVECTOR2(0,120),1.0f,"Media/Fonts/1");
		this->mHud[3] = mGe->CreateText("",D3DXVECTOR2(0,180),1.0f,"Media/Fonts/1");
		this->mHud[4] = mGe->CreateText("",D3DXVECTOR2(0,240),1.0f,"Media/Fonts/1");
		this->mCreditSpeed = 0.05f;

		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setBallToFollow(this->mBalls[0]);
		*/
		this->mTimeElapsedText = this->mGe->CreateText(	"",
														D3DXVECTOR2(windowWidth - 150.0f,
																	windowHeight - 100.0f), 
														1.0f, 
														"Media/Fonts/1");
		/*
		CheckBox for the change gamemode
		*/
		this->mCb = new CheckBox(0,0,0,"Media/Menus/CheckBoxFrame.png",	dx * (30.0f / 1200.0f), windowHeight * (30.0f / 900.0f),
			"Media/Menus/CheckBoxChecked.png", true, new ChangeOptionEvent("MazeMode", "true"), "ChangeSet");
}

void Maze::Intro()
{
	Text*	intro = mGe->CreateText("Maze",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
	mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
	intro->SetText("");
	mGe->DeleteText(intro);
}

bool Maze::PlaySpecific()
{	
	this->mHud[0]->SetText("Rikard Johansson");
	this->mHud[1]->SetText("Marcus Lowegren");
	this->mHud[2]->SetText("Markus Tillman");
	this->mHud[3]->SetText("Didrik Axelsson");
	this->mHud[4]->SetText("Jerry Rahmqvist");
		
		
	this->mRunning = true;
	this->mGameMode = CREDITS;
		
		
	int choice = 1; // choice = 2 is version2 of maze
	this->mDiff = mGe->Update();
	srand ( time(NULL) );
	this->mTargetX = -10 + rand() % 21;			// random angle [-10, 10] in x-axis
	this->mTargetZ = -10 + rand() % 21;			// random angle [-10, 10] in z-axis
	this->mTargetX = this->mTargetX*(PI/180.0f);		// convert to radians
	this->mTargetZ = this->mTargetZ*(PI/180.0f);		// convert to radians
		
	/* set so the first Maze mode will do the random tilt and gets a start angle . */
	this->mPlatform->SetTargetAngleX(this->mTargetX);
	this->mPlatform->SetTargetAngleZ(this->mTargetZ);

	/* set so we cant tilt it more than these angles. */
	this->mPlatform->SetMaxAngleX(10.0f*(PI/180.0f));
	this->mPlatform->SetMaxAngleZ(10.0f*(PI/180.0f));

	/* to swap between Maze modes with space button and we don't want it to flip. */
	bool spacePressed = false;

	/* counters for the restart restart of maze. */
	float respownAfterDeathCounter = 0.0f;
	float resetMazeCounter = 0.0f;
	this->mDelayTimer = 2.0f;

	this->mCb->AddToRenderer(this->mGe);

	while(this->mRunning && mGe->isRunning())
	{
			
		this->mDiff = mGe->Update();
		this->mTimeElapsed += this->mDiff * 0.001f;
		if(choice == 1)
		{
			this->PlayMazeV1();
		}
		else
		{
			this->PlayMazeV2();
		}
			
		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
			this->mRunning = false;
		
		GraphicsEngine* ge = GetGraphicsEngine();
		D3DXVECTOR2 mousePos = ge->GetKeyListener()->GetMousePosition();
		GUIEvent* tempEvent = this->mCb->CheckCollision(mousePos.x, mousePos.y, ge->GetKeyListener()->IsClicked(1), ge);
		if(tempEvent != NULL)
		{
			if(((ChangeOptionEvent*)tempEvent)->GetValue() == "false")
			{
				choice = 2;
				this->mPlatform->SetRotate(false);
				this->ResetMaze();
				this->mBalls[0]->SetToStartPosition();
				this->mBalls[0]->ResetParent();
				this->mDelayTimer = 2.0f;
				this->mDelayTimer -= this->mDiff;
			}
			else
			{
				choice = 1;
				this->mPlatform->SetRotate(true);
				this->ResetMaze();
				this->mBalls[0]->SetToStartPosition();
				this->mBalls[0]->ResetParent();
				this->mDelayTimer = 2.0f;
				this->mDelayTimer -= this->mDiff;		
			}
		}

		if(this->checkWinConditions(this->mDiff))
			this->mRunning = false;
		if(this->checkRespownConditions())
		{
			resetMazeCounter += this->mDiff * 0.001f;
			if(resetMazeCounter > 1.5f)
			{
				this->ResetMaze();
				this->mBalls[0]->ResetParent();
				this->mBalls[0]->SetToStartPosition();
				resetMazeCounter = 0.0f;
			}

				
		}

		this->ShowHud();

	}
	return !this->mRunning;
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

bool Maze::checkRespownConditions()
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

void Maze::ResetMaze()
{
	this->mPlatform->GetMesh()->ResetRotationAndScale();
	this->mPlatform->ResetXZAngles();
}


void Maze::ShowHud()
{
	 
	 if(this->mHud[0]->GetPosition().y + this->mDiff * this->mCreditSpeed > this->mWindowHeight)
		this->mHud[0]->SetPosition(D3DXVECTOR2(this->mHud[0]->GetPosition().x, 0));
	else
		this->mHud[0]->SetPosition(D3DXVECTOR2(this->mHud[0]->GetPosition().x, this->mHud[0]->GetPosition().y + this->mDiff * this->mCreditSpeed));

	if(this->mHud[1]->GetPosition().y + this->mDiff * this->mCreditSpeed > this->mWindowHeight)
		this->mHud[1]->SetPosition(D3DXVECTOR2(this->mHud[1]->GetPosition().x, 0));
	else
		this->mHud[1]->SetPosition(D3DXVECTOR2(this->mHud[1]->GetPosition().x, this->mHud[1]->GetPosition().y + this->mDiff * this->mCreditSpeed));

	if(this->mHud[2]->GetPosition().y + this->mDiff * this->mCreditSpeed > this->mWindowHeight)
		this->mHud[2]->SetPosition(D3DXVECTOR2(this->mHud[2]->GetPosition().x, 0));
	else
		this->mHud[2]->SetPosition(D3DXVECTOR2(this->mHud[2]->GetPosition().x, this->mHud[2]->GetPosition().y + this->mDiff * this->mCreditSpeed));

	if(this->mHud[3]->GetPosition().y + this->mDiff * this->mCreditSpeed > this->mWindowHeight)
		this->mHud[3]->SetPosition(D3DXVECTOR2(this->mHud[3]->GetPosition().x, 0));
	else
		this->mHud[3]->SetPosition(D3DXVECTOR2(this->mHud[3]->GetPosition().x, this->mHud[3]->GetPosition().y + this->mDiff * this->mCreditSpeed));

	if(this->mHud[4]->GetPosition().y + this->mDiff * this->mCreditSpeed > this->mWindowHeight)
		this->mHud[4]->SetPosition(D3DXVECTOR2(this->mHud[4]->GetPosition().x, 0));
	else
		this->mHud[4]->SetPosition(D3DXVECTOR2(this->mHud[4]->GetPosition().x, this->mHud[4]->GetPosition().y + this->mDiff * this->mCreditSpeed));
	
	
	//show time elapsed
	float tmp = floor(this->mTimeElapsed * 10.0f) / 10.0f;
	this->mTimeElapsedText->SetText(MaloW::convertNrToString(tmp));
}

void Maze::PlayMazeV1()
{
	if(this->mDelayTimer > 0)
	{
		this->mDelayTimer -= this->mDiff;
		return;
	}
	if(this->mPlatform->IsTargetAngleReachedX())
	{
		this->mTargetX = -10 + rand() % 21;
		this->mTargetX = this->mTargetX*(PI/180.0f);
		this->mPlatform->SetTargetAngleX(this->mTargetX);
	}
	if(this->mPlatform->IsTargetAngleReachedZ())
	{
		this->mTargetZ = -10 + rand() % 21;
		this->mTargetZ = this->mTargetZ*(PI/180.0f);
		this->mPlatform->SetTargetAngleZ(this->mTargetZ);
	}
		
		
	if(mGe->GetKeyListener()->IsPressed('W'))
		this->mBalls[0]->AddForceForwardDirection(this->mDiff);	
	if(mGe->GetKeyListener()->IsPressed('S'))
		this->mBalls[0]->AddForceOppositeForwardDirection(this->mDiff);
	if(mGe->GetKeyListener()->IsPressed('A'))
		this->mBalls[0]->AddForceLeftOfForwardDirection(this->mDiff);	
	if(mGe->GetKeyListener()->IsPressed('D'))
		this->mBalls[0]->AddForceRightOfForwardDirection(this->mDiff);	
	if(mGe->GetKeyListener()->IsClicked(2))
		this->mBalls[0]->AddForce(Vector3(0,this->mDiff*(11.0f/6.0f),0));
	this->mBalls[0]->UpdateBallParentMode(this->mPlatform);
	this->mBalls[0]->Update(this->mDiff);
	Vector3 normalPlane;
	if(this->mBalls[0]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
		this->mBalls[0]->collisionPlatformResponse(this->mPlatform, normalPlane, this->mDiff);
	this->mBalls[0]->UpdatePost();
	this->mPlatform->Update(this->mDiff);
	
}

void Maze::PlayMazeV2()
{
	if(this->mDelayTimer > 0)
	{
		this->mDelayTimer -= this->mDiff;
		return;
	}
	if(mGe->GetKeyListener()->IsPressed('W'))
		this->mPlatform->RotateX(this->mDiff);
	if(mGe->GetKeyListener()->IsPressed('S'))
		this->mPlatform->RotateX(-this->mDiff);
	if(mGe->GetKeyListener()->IsPressed('A'))
		this->mPlatform->RotateZ(this->mDiff);
	if(mGe->GetKeyListener()->IsPressed('D'))
		this->mPlatform->RotateZ(-this->mDiff);
	
	if(mGe->GetKeyListener()->IsClicked(2))
		this->mBalls[0]->AddForce(Vector3(0,this->mDiff*(11.0f/6.0f),0));
	// cheating is gooood!!!
	if(mGe->GetKeyListener()->IsPressed(VK_UP))
		this->mBalls[0]->AddForceForwardDirection(this->mDiff);	
	if(mGe->GetKeyListener()->IsPressed(VK_DOWN))
		this->mBalls[0]->AddForceOppositeForwardDirection(this->mDiff);
	if(mGe->GetKeyListener()->IsPressed(VK_LEFT))
		this->mBalls[0]->AddForceLeftOfForwardDirection(this->mDiff);	
	if(mGe->GetKeyListener()->IsPressed(VK_RIGHT))
		this->mBalls[0]->AddForceRightOfForwardDirection(this->mDiff);	
	if(mGe->GetKeyListener()->IsClicked(2))
		this->mBalls[0]->AddForce(Vector3(0,this->mDiff*(11.0f/6.0f),0));
	this->mBalls[0]->UpdateBallParentMode(this->mPlatform);
	this->mBalls[0]->Update(this->mDiff);
	Vector3 normalPlane;
	if(this->mBalls[0]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
		this->mBalls[0]->collisionPlatformResponse(this->mPlatform, normalPlane, this->mDiff);
	this->mBalls[0]->UpdatePost();
	this->mPlatform->Update(this->mDiff);

	
}

void Maze::HideHud()
{

}