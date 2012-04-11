#include "GameManager.h"

GameManager::GameManager(GraphicsEngine* ge)
{
	this->mNumPlayers	= 0;
	this->mPlatform		= NULL;
	this->mBalls		= NULL;
	this->mGe			= ge;
}
GameManager::~GameManager()
{
	// removing the mesh from the graphics engine
	mGe->DeleteMesh(this->mPlatform->GetMesh());
	for (int i = 0; i < this->mNumPlayers; i++) 
	{
		mGe->DeleteMesh(this->mBalls[i]->GetMesh());
	}
	
	// removing the dynamically allocated memory
	SAFE_DELETE(this->mPlatform);
	for (int i = 0; i < this->mNumPlayers; i++) 
	{
		SAFE_DELETE(this->mBalls[i]);
    }
	SAFE_DELETE_ARRAY(this->mBalls);
}

bool GameManager::Play(const int numPlayers)
{
	this->mNumPlayers = numPlayers;
	this->Initialize();
	bool running = true;
	
	
	
	while(running)
	{
		int numAlivePlayers = 0;
				
		float diff = mGe->Update();	
		
		if(mGe->GetKeyListener()->IsPressed('P'))
			mGe->GetCamera()->moveForward(diff);
		if(mGe->GetKeyListener()->IsClicked(1))
			mGe->GetCamera()->moveBackward(diff);
		
		diff *= 0.05f;
		/* the move of ball 1 and ball 2 should be done using a 
		*  timer that perhaps is set to only add force every 
		*  intervall. so we don't get a jumpy motion of the 
		*  balls :)
		*/
		// move ball 1
		if(mGe->GetKeyListener()->IsPressed('A'))
			mBalls[0]->AddForce(Vector3(-1,0,0));	
		if(mGe->GetKeyListener()->IsPressed('D'))
			mBalls[0]->AddForce(Vector3(1,0,0));
		if(mGe->GetKeyListener()->IsPressed('W'))
			mBalls[0]->AddForce(Vector3(0,0,1));	
		if(mGe->GetKeyListener()->IsPressed('S'))
			mBalls[0]->AddForce(Vector3(0,0,-1));
		if(mGe->GetKeyListener()->IsClicked(2))
			mBalls[0]->AddForce(Vector3(0,11,0));
		// move ball 2
		if(mGe->GetKeyListener()->IsPressed('H'))
			mBalls[1]->AddForce(Vector3(-1,0,0));	
		if(mGe->GetKeyListener()->IsPressed('K'))
			mBalls[1]->AddForce(Vector3(1,0,0));
		if(mGe->GetKeyListener()->IsPressed('U'))
			mBalls[1]->AddForce(Vector3(0,0,1));	
		if(mGe->GetKeyListener()->IsPressed('J'))
			mBalls[1]->AddForce(Vector3(0,0,-1));
		for(int i = 0; i < this->mNumPlayers; i++)
		{
			this->mBalls[i]->Update(diff, this->mPlatform);
			/*Vector3 temp = this->mBalls[i]->GetPositionXZ() - this->mPlatform->GetPositionXZ();

			
			if( temp.GetLength() < this->mPlatform->GetScaledRadius() )
				numAlivePlayers += 1;*/
			
			
			if(this->mBalls[i]->IsAlive())
				numAlivePlayers += 1;
			
		}

		if(numAlivePlayers <= 1)
			running = false;
		mPlatform->Update(diff*0.05);
	}
	
	//returns to menu after some win/draw screen.
	return true;
}

void GameManager::Initialize()
{
	
	D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,10,0);
	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 30, -15));
	mGe->GetCamera()->LookAt(centerPlatform);
	Image* testImg = mGe->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	Light* testLight = mGe->CreateLight(D3DXVECTOR3(0, 25, 0));
	Light* moreLight1 = mGe->CreateLight(D3DXVECTOR3(0, 20, -5)); 
	Light* moreLight2 = mGe->CreateLight(D3DXVECTOR3(0, 20, 5));
	Light* moreLight3 = mGe->CreateLight(D3DXVECTOR3(5, 25, 0));
	Light* moreLight4 = mGe->CreateLight(D3DXVECTOR3(-5, 25, 0));
	/*
	Light* moreLight5 = mGe->CreateLight(D3DXVECTOR3(-8, 25, 0));
	Light* moreLight6 = mGe->CreateLight(D3DXVECTOR3(-10, 25, 0));
	Light* moreLight7 = mGe->CreateLight(D3DXVECTOR3(-12, 25, 0));
	*/
	
	
	
	this->mPlatform		= new Platform("Media/Cylinder.obj", centerPlatform);
	this->mBalls		= new Ball*[this->mNumPlayers];

	for(int i = 0; i < this->mNumPlayers; i++)
	{
		if( i == 0)
			this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,14.7f,-5));
		else
			this->mBalls[i] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,14.7f,5));
	}
}

