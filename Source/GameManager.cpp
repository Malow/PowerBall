#include "GameManager.h"

GameManager::GameManager(GraphicsEngine* ge)
{
	this->mNumPlayers	= 0;
	this->mPlatform		= NULL;
	this->mBalls		= NULL;
	this->mGe			= ge;
	this->mNet			= NULL;
}
GameManager::~GameManager()
{
	// removing the dynamically allocated memory
	SAFE_DELETE(this->mPlatform);
	for (int i = 0; i < this->mNumPlayers; i++) 
	{
		SAFE_DELETE(this->mBalls[i]);
    }
	SAFE_DELETE_ARRAY(this->mBalls);
	SAFE_DELETE(this->mNet);
	for(int i = 0; i < 5; i++)
	{
		this->mGe->DeleteLight(this->mLights[i]);
	}
}

bool GameManager::Play(const int numPlayers)
{
	this->~GameManager();
	this->mNumPlayers = numPlayers;
	this->Initialize();
	bool running = true;
	
	
	while(running)
	{
		int numAlivePlayers = 0;
				
		float diff = mGe->Update();	
		

		
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

		if(mGe->GetKeyListener()->IsPressed('P'))
			mGe->GetCamera()->moveForward(diff);
		if(mGe->GetKeyListener()->IsClicked(1))
			mGe->GetCamera()->moveBackward(diff);

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

			if(this->mBalls[i]->IsAlive())
				numAlivePlayers += 1;
			
		}
		
		// will be moved to phisics simulation class
		for(int i = 0; i < this->mNumPlayers; i++)
		{
			for(int j = i+1; j < this->mNumPlayers; j++)
			{
				Ball* b1 = this->mBalls[i];
				Ball* b2 = this->mBalls[j];
				if(b1->collisionWithSphereSimple(b2))
					b1->collisionSphereResponse(b2, diff);

			}
		}

		mPlatform->Update(diff*0.05);

		if(numAlivePlayers <= 1)
			running = false;
	}
	
	//returns to menu after some win/draw screen.
	return true;
}
bool GameManager::PlayLAN(const int numPlayers)
{
	this->~GameManager();
	this->mNumPlayers = numPlayers;
	this->Initialize();
	bool running = true;

	this->mNet = new GameNetwork();
	this->mNet->SetIP("79.138.27.193");
	this->mNet->Start();
	if(this->mNet->IsServer())
	{
		this->mBalls[0]->SetPosition(0,14.7f,5);
		//this->mBalls[1]->SetPosition(0,14.7f,-5);
	}
	this->mGe->Update();
	
	
	while(running)
	{
		int numAlivePlayers = 0;
				
		float diff = mGe->Update();	
		
		//this->mNet->Update(this->mBalls, this->mNumPlayers); 

		diff *= 0.05f;

		if(mGe->GetKeyListener()->IsPressed('A'))
			mBalls[this->mNet->GetIndex()]->AddForce(Vector3(-1,0,0));	
		if(mGe->GetKeyListener()->IsPressed('D'))
			mBalls[this->mNet->GetIndex()]->AddForce(Vector3(1,0,0));
		if(mGe->GetKeyListener()->IsPressed('W'))
			mBalls[this->mNet->GetIndex()]->AddForce(Vector3(0,0,1));	
		if(mGe->GetKeyListener()->IsPressed('S'))
			mBalls[this->mNet->GetIndex()]->AddForce(Vector3(0,0,-1));
		if(mGe->GetKeyListener()->IsClicked(2))
			mBalls[this->mNet->GetIndex()]->AddForce(Vector3(0,11,0));



		//if(this->mNet->IsServer())
		{
			// will be moved to phisics simulation class
			for(int i = 0; i < this->mNumPlayers; i++)
			{
				//this->mBalls[i]->Update(diff, this->mPlatform);
				for(int j = i+1; j < this->mNumPlayers; j++)
				{
					Ball* b1 = this->mBalls[i];
					Ball* b2 = this->mBalls[j];
					if(b1->collisionWithSphereSimple(b2))
						b1->collisionSphereResponse(b2, diff);

				}
			}
		}
			
		this->mBalls[this->mNet->GetIndex()]->Update(diff, this->mPlatform);

		this->mNet->SetPos(this->mBalls[this->mNet->GetIndex()]->GetPosition());
		

		
		this->mNet->Update(this->mBalls, this->mNumPlayers); 


		for(int i = 0; i < this->mNumPlayers; i++)
		{
			
			
			if(this->mBalls[i]->IsAlive())
				numAlivePlayers += 1;
			
		}
		//mPlatform->Update(diff*0.05);


		if(this->mNet->GetNumPlayers() > this->mNumPlayers)
		{
			for(int i = this->mNumPlayers; i < this->mNet->GetNumPlayers(); i++)
			{
				this->mNumPlayers++;
				Ball** temp = new Ball*[this->mNumPlayers];
				for(int i = 0; i < this->mNumPlayers - 1; i++)
				{
					temp[i] = this->mBalls[i];
				}
				temp[this->mNumPlayers - 1] = new Ball("Media/Ball.obj", D3DXVECTOR3(0,14.7f,0));

				delete[] this->mBalls;
				this->mBalls = temp;
			}
		}

		//if(numAlivePlayers <= 1 && this->mNet->GetNumPlayers() > 1)
			//running = false;
	}
	//returns to menu after some win/draw screen.
	return true;
}
void GameManager::Initialize()
{
	
	D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,10,0);
	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 30, -15));
	mGe->GetCamera()->LookAt(centerPlatform);
	//Image* testImg = mGe->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 25, 0));
	this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 20, -5)); 
	this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 20, 5));
	this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(5, 25, 0));
	this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-5, 25, 0));
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

