#include "GameMode.h"
#include "..\Game Objects\PowerBall.h"
#include "GraphicsEngine.h"
GameMode::GameMode()
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

GameMode::~GameMode()
{
	SAFE_DELETE(this->mPlatform);
	for (int i = 0; i < this->mNumberOfPlayers; i++) 
	{
		SAFE_DELETE(this->mBalls[i]);
    }
	SAFE_DELETE_ARRAY(this->mBalls);
	//SAFE_DELETE(this->mNet);
}

void GameMode::PlayLan()
{
		bool zoomOutPressed = false;
		bool zoomInPressed = false;
		this->mNet->Start(this->mServerInfo);
		this->mGe->Update();
		bool roundsLeft = true;
		int roundsPlayed = 0;

		//choose team before starting the game
		if(this->mGameMode != GAMEMODE::WARLOCK)
			this->mTeam = this->mChooseTeamMenu->Run();
		else
			this->mTeam = TEAM::NOTEAM;
		//this->mBalls[this->mNet->GetIndex]->SetTeamColor(team);**
		MsgHandler::GetInstance().JoinTeam((TEAM)this->mTeam);
		while(roundsLeft && this->mGe->isRunning())
		{

			this->PlayRoundLan(roundsLeft, zoomInPressed, zoomOutPressed); 
			roundsPlayed++;;
			if(roundsPlayed == this->mNumberOfRounds)
				roundsLeft = false;
		}
		
		this->mNet->Close();
}

void GameMode::InputKnockout(float diff, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& roundsLeft, bool& quitByMenu )
{
	if(mGe->GetEngineParameters().CamType == RTS)
		{
			if(mGe->GetKeyListener()->IsPressed('A'))
				mBalls[0]->AddForce(Vector3(- diff,0,0));	
			if(mGe->GetKeyListener()->IsPressed('D'))
				mBalls[0]->AddForce(Vector3(diff,0,0));
			if(mGe->GetKeyListener()->IsPressed('W'))
				mBalls[0]->AddForce(Vector3(0,0,diff));	
			if(mGe->GetKeyListener()->IsPressed('S'))
				mBalls[0]->AddForce(Vector3(0,0,-diff));
			if(mGe->GetKeyListener()->IsClicked(2))
				mBalls[0]->AddForce(Vector3(0,diff*(11.0f/6.0f),0));
		}
		else if(mGe->GetEngineParameters().CamType == TRD)
		{
			if(mGe->GetKeyListener()->IsPressed('W'))
				mBalls[0]->AddForceForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsPressed('S'))
				mBalls[0]->AddForceOppositeForwardDirection(diff);
			if(mGe->GetKeyListener()->IsPressed('Q'))
				mBalls[0]->RotateForwardLeft(diff);
			if(mGe->GetKeyListener()->IsPressed('E'))
				mBalls[0]->RotateForwardRight(diff);
			if(mGe->GetKeyListener()->IsClicked(2))
				mBalls[0]->AddForce(Vector3(0,diff*(11.0f/6.0f),0));
			if(mGe->GetKeyListener()->IsPressed('A'))
				mBalls[0]->AddForceLeftOfForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsPressed('D'))
				mBalls[0]->AddForceRightOfForwardDirection(diff);	
			if(mGe->GetKeyListener()->IsPressed('1'))
				mBalls[0]->UseSpell(1);
			if(mGe->GetKeyListener()->IsPressed('2'))
				mBalls[0]->UseSpell(2);
			if(mGe->GetKeyListener()->IsPressed('3'))
				mBalls[0]->UseSpell(3);
			if(mGe->GetKeyListener()->IsPressed('Z') && !zoomOutPressed)
			{
				mBalls[0]->ZoomOut();
				zoomOutPressed = true;
			}
			else if(!mGe->GetKeyListener()->IsPressed('Z'))
				zoomOutPressed = false;
			if(mGe->GetKeyListener()->IsPressed('C') && !zoomInPressed)
			{
				mBalls[0]->ZoomIn();
				zoomInPressed = true;
			}
			else if(!mGe->GetKeyListener()->IsPressed('C'))
				zoomInPressed = false;
		}
		

		// move ball 2
		if(mGe->GetKeyListener()->IsPressed(VK_LEFT))
			mBalls[1]->AddForce(Vector3(-diff,0,0));	
		if(mGe->GetKeyListener()->IsPressed(VK_RIGHT))
			mBalls[1]->AddForce(Vector3(diff,0,0));
		if(mGe->GetKeyListener()->IsPressed(VK_UP))
			mBalls[1]->AddForce(Vector3(0,0,diff));	
		if(mGe->GetKeyListener()->IsPressed(VK_DOWN))
			mBalls[1]->AddForce(Vector3(0,0,-diff));

		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
		{
			roundsLeft = running = this->mIGM->Run();
			quitByMenu = !running;
		}

}


void GameMode::ClientKeyPress(float diff, const int index, char key)
{

	if(key == 'A')
		mBalls[index]->AddForceLeftOfForwardDirection(diff);
	if(key == 'D')
		mBalls[index]->AddForceRightOfForwardDirection(diff);
	/* wTF */
	if(key == 'W')
		mBalls[index]->AddForceForwardDirection(diff);
	
	
	if(key == 'S')
		mBalls[index]->AddForceOppositeForwardDirection(diff);
	
	if(key == 'Q')
		mBalls[index]->RotateForwardLeft(diff);
	if(key == 'E')
		mBalls[index]->RotateForwardRight(diff);
	if(key == '1')
		mBalls[index]->UseSpell(1);
	if(key == '2')
		mBalls[index]->UseSpell(2);
	if(key == '3')
		mBalls[index]->UseSpell(3);
	if(key == '4')
		mBalls[index]->UseSpell(4);
	if(key == VK_SPACE)
		mBalls[index]->UseSpell(5);
	//if(key == VK_SPACE)
		//mBalls[index]->AddForce(Vector3(0, diff,0));
	
}

void GameMode::InputKeysPressedSelf(float diff, int index, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu)
{
	if(mGe->GetEngineParameters().CamType == TRD)
	{
		if(mGe->GetKeyListener()->IsPressed('A'))
			mBalls[index]->AddForceLeftOfForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('W'))
			mBalls[index]->AddForceForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('S'))
			mBalls[index]->AddForceOppositeForwardDirection(diff);
		if(mGe->GetKeyListener()->IsPressed('Q'))
			mBalls[index]->RotateForwardLeft(diff);
		if(mGe->GetKeyListener()->IsPressed('E'))
			mBalls[index]->RotateForwardRight(diff);
		if(mGe->GetKeyListener()->IsPressed('D'))
			mBalls[index]->AddForceRightOfForwardDirection(diff);	
		if(mGe->GetKeyListener()->IsPressed('1'))
			mBalls[index]->UseSpell(1);
		if(mGe->GetKeyListener()->IsPressed('2'))
			mBalls[index]->UseSpell(2);
		if(mGe->GetKeyListener()->IsPressed('3'))
			mBalls[index]->UseSpell(3);
		if(mGe->GetKeyListener()->IsPressed('4'))
			mBalls[index]->UseSpell(4);
		if(mGe->GetKeyListener()->IsPressed(VK_SPACE))
			mBalls[index]->UseSpell(5);
		if(mGe->GetKeyListener()->IsPressed('Z') && !zoomOutPressed)
		{
			mBalls[index]->ZoomOut();
			zoomOutPressed = true;
		}
		else if(!mGe->GetKeyListener()->IsPressed('Z'))
			zoomOutPressed = false;
		if(mGe->GetKeyListener()->IsPressed('C') && !zoomInPressed)
		{
			mBalls[index]->ZoomIn();
			zoomInPressed = true;
		}
		else if(!mGe->GetKeyListener()->IsPressed('C'))
			zoomInPressed = false;
	}
	else
	{

	}
	if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
	{
		running = this->mIGM->Run();
		quitByMenu = !running;
	}
}


void GameMode::SendKeyInputs(const int clientIndex, float diff)
{
	char keyDowns[5] = {0};
	char keysToCheck[12] = {'A', 'D', 'W', 'S', 'Q', 'E', VK_SPACE, '1', '2', '3', '4', VK_SPACE};
	int numKeys = 0;
	for(int i = 0; i < 12; i++)
	{
		if(mGe->GetKeyListener()->IsPressed(keysToCheck[i]) && numKeys < 5)
			keyDowns[numKeys++] = keysToCheck[i];
	}
				
	if(numKeys == 0)
		keyDowns[numKeys++] = '?'; //"idle"-key
	
	Vector3 temp = this->mBalls[clientIndex]->GetForwardVector();
	this->mNet->GetBall(clientIndex)->AddKeyInput(keyDowns, numKeys, diff, D3DXVECTOR3(temp.x, temp.y, temp.z));
	
}
void GameMode::HandleClientKeyInputs(const int clientIndex, float diff)
{
	//keep reading client inputs until the sum of all DT has exceeded server DT (->not allowed to move any more)
	Command* command = this->mNet->GetBall(clientIndex)->GetNextCommand();
	float duration = 0.0f;
	if(command != NULL)
	{
		duration = command->GetDuration();
		while(duration <=  diff && command != NULL)
		{
			this->mBalls[clientIndex]->SetForwardVector(command->GetForward());
			for(int c = 0; c < command->GetNumInputs(); c++)
			{
				this->ClientKeyPress(command->GetDuration(), clientIndex, command->GetInput(c));
			}
			this->mNet->GetBall(clientIndex)->SetExecTime(this->mNet->GetBall(clientIndex)->GetExecTime() + command->GetDuration());
			this->mNet->GetBall(clientIndex)->PopCommand();


			command = this->mNet->GetBall(clientIndex)->GetNextCommand();
			if(command != NULL)
				duration += command->GetDuration();
								
		}
		if(duration > diff && command != NULL)
		{
			this->mBalls[clientIndex]->SetForwardVector(command->GetForward());
			duration -= command->GetDuration();
									
			for(int c = 0; c < command->GetNumInputs(); c++)
			{
				//ADD A CHECK HERE SO THAT THE SAME KEY CANT APPEAR MORE THAN ONCE IN THE ARRAY (COULD CHEAT THE SYSTEM THIS WAY)
				
				this->ClientKeyPress((diff - duration), clientIndex, command->GetInput(c));
			}

			command->ModifyDuration(-(diff - duration));
								
			this->mNet->GetBall(clientIndex)->SetExecTime(this->mNet->GetBall(clientIndex)->GetExecTime() + (diff - duration));
		}
	}
}

void GameMode::IsServer(float diff, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu)
{
	// will be moved to phisics simulation class
	for(int i = 0; i < this->mNumberOfPlayers; i++)
	{
		if(i != this->mNet->GetIndex())
		{
			this->HandleClientKeyInputs(i, diff);
		}
		else
		{
			this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
		}	

		PowerBall* b1 = this->mBalls[i];
		for(int j = i+1; j < this->mNumberOfPlayers; j++)
		{
			PowerBall* b2 = this->mBalls[j];
			if(b1->collisionWithSphereSimple(b2))
				b1->collisionSphereResponse(b2);
		}
		Vector3 normalPlane;
		if(b1->collisionWithPlatformSimple(this->mPlatform,normalPlane))
			b1->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
						//for(int i = 0; i < this->mNumberOfPlayers; i++)
							//this->mBalls[i]->UpdatePost();
	
	}
	for(int i = 0; i < this->mNumberOfPlayers; i++)
	{					
		this->mBalls[i]->UpdatePost();
		bool clientBall = true;
		if(i == this->mNet->GetIndex())
			clientBall = false;
		this->mBalls[i]->Update(diff, clientBall); //split up due to the balls affecting each other, so cant send final position until all balls updated
		clientBall = true;

	}

	for(int i = 0; i < this->mNumberOfPlayers; i++)
	{
		this->mNet->GetBall(i)->SetPos(this->mBalls[i]->GetPosition());
		Vector3 vel = this->mBalls[i]->GetVelocity();
		this->mNet->GetBall(i)->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z));
	}
}

void GameMode::IsClient(float diff, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu)
{
	for(int i = 0; i < this->mNumberOfPlayers; i++)
	{
		if(this->mNet->GetIndex() != i)
		{
			D3DXVECTOR3 rotVector = this->mNet->GetBall(i)->GetPos() - this->mBalls[i]->GetPosition();
			this->mBalls[i]->SetPosition(this->mNet->GetBall(i)->GetPos());
			this->mBalls[i]->Rotate(rotVector);
		}
	}
	if(this->mNet->GetIndex() < this->mNumberOfPlayers)
	{
		int i = this->mNet->GetIndex();
		this->SendKeyInputs(i, diff);
		this->InputKeysPressedSelf(diff, i, zoomOutPressed, zoomInPressed, running, quitByMenu);
		
		for(int c = 0; c < this->mNumberOfPlayers; c++)
		{
			PowerBall* b1 = this->mBalls[c];
			for(int j = c+1; j < this->mNumberOfPlayers; j++)
			{
				PowerBall* b2 = this->mBalls[j];
				if(b1->collisionWithSphereSimple(b2))
					b1->collisionSphereResponse(b2);
			}
		}
		
		Vector3 normalPlane;
		if(this->mBalls[i]->collisionWithPlatformSimple(this->mPlatform,normalPlane))
			this->mBalls[i]->collisionPlatformResponse(this->mPlatform, normalPlane, diff);
		this->mBalls[i]->UpdatePost();
		this->mBalls[i]->Update(diff);
		this->mNet->GetBall(i)->AddMovementPowerBall(this->mBalls[i]);
	}
}

void GameMode::PlayRoundLan(bool& roundsLeft, bool& zoomInPressed, bool& zoomOutPressed)
{
		bool running = true;
		bool quitByMenu = false;
		int numAlivePlayers = 0;
		LARGE_INTEGER oldTick = LARGE_INTEGER();
		LARGE_INTEGER now =  LARGE_INTEGER();
		while(running && this->mGe->isRunning())
		{
				float diff = mGe->Update(); //A problem when the user opens up ingame menu is that the diff after resume is incredibly high so it breaks game logic, game gotta continue in the background if network :P	

				if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
					roundsLeft = running = this->mIGM->Run();
		
				QueryPerformanceCounter(&now);
				LARGE_INTEGER proc_freq;
				::QueryPerformanceFrequency(&proc_freq);
				double frequency = proc_freq.QuadPart;

				//diff = 1000*((now.QuadPart - oldTick.QuadPart) / frequency); //2			WITH A VARIABLE DELTATIME THE BALL PHYSICS RESULT DIFFER IF MORE THAN TWO CLIENTS WITH DIFFERENT DELTA TIMES PROCESS EXACTLY THE SAME INPUT, SETTING A CONSTANT DELTATIME HOWEVER LEADS TO THE SAME PHYSICS RESULT (THOUGH WITH A HUGE DELAY DUE TO THE CLIENT IN THE BACKGROUND IS A ASSIGNED LESS CPU TIME (-> low FPS)). IS THERE SOMETHING IN BALL PHYSICS THAT SHOULD BE DEPENDANT ON DELTATIME THAT ISNT?//
				QueryPerformanceCounter(&oldTick);
				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					if(this->mBalls[i]->GetTeamColor() != this->mNet->GetBall(i)->GetTeam()) //causes lag otherwise re-setting the color every frame if its alrdy set.
						this->mBalls[i]->SetTeamColor(this->mNet->GetBall(i)->GetTeam());
				}
				if(this->mNet->IsServer())
					this->IsServer(diff, zoomOutPressed, zoomInPressed, running, quitByMenu);
				else //is client
					this->IsClient(diff, zoomOutPressed, zoomInPressed, running, quitByMenu);
				

				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					if(this->mBalls[i]->IsAlive())
						numAlivePlayers += 1;
				}
				mPlatform->Update(diff);

		
				if(!this->mNet->UpdatePowerBall(this->mBalls, this->mNumberOfPlayers, diff))
					running = false;

				if(this->mNet->GetNumPlayers() > this->mNumberOfPlayers)
				{
					this->AddBall();
					numAlivePlayers++;
				}

				if(this->mNet->IsServer())
				if((numAlivePlayers == 1 && this->mNet->GetNumPlayers() > 1) || numAlivePlayers < 1)
				{
					running = false;
				}
				
				if(this->checkWinConditions(diff))
					running = false;
				this->ShowHud();
				float newdt = diff/1000.0f;
				
				this->mTimeElapsed += newdt;
				if(this->mTimeElapsed > 600.0f)
					running = false;
		}
}

void GameMode::AddBall()
{

}