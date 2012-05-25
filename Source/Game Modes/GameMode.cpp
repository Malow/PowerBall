#include "GameMode.h"
#include "..\Game Objects\PowerBall.h"
#include "GraphicsEngine.h"
#include "..\Physics\PhysicsEngine.h"

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
	this->mChooseTeamMenu = NULL;
	this->mQuitByMenu = false;
	this->mTimeElapsedText = NULL;
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
	SAFE_DELETE(this->mChooseTeamMenu);
	this->mGe->DeleteText(this->mTimeElapsedText);
}

bool GameMode::PlayLan()
{
	bool zoomOutPressed = false;
	bool zoomInPressed = false;
	bool quitByMenu = false;
	this->mGe->Update();
	bool roundsLeft = true;
	int roundsPlayed = 0;

	//choose team before starting the game
	this->mChooseTeamMenu = new ChooseTeamMenu(this->mGe);

	if(this->mGameMode != GAMEMODE::WARLOCK && this->mTeam == TEAM::NOTEAM)
		CreateThread(0, 0, &SelectTeamThread, (void*) this, 0, 0);//this->mChooseTeamMenu->Run();
	else if(this->mGameMode == GAMEMODE::WARLOCK)
		this->mTeam = TEAM::NOTEAM;
	this->mNet->Reset();
	//this->mBalls[this->mNet->GetIndex]->SetTeamColor(team);**
	//MsgHandler::GetInstance().JoinTeam((TEAM)this->mTeam);
	#if FixedTimeStep
		this->mPe = new PhysicsEngine(this->mGe, this->mNet, this->mServerInfo);
		this->mPe->Initialize();
		this->mPe->AddMap(this->mPlatform);
	#else
			
	#endif
	while(roundsLeft && this->mGe->isRunning())
	{
		quitByMenu = this->PlayRoundLan(roundsLeft, zoomInPressed, zoomOutPressed); 
		roundsPlayed++;;
		if(roundsPlayed == this->mNumberOfRounds)
			roundsLeft = false;
	}
		
	delete this->mChooseTeamMenu;
	this->mChooseTeamMenu = NULL;
	//this->mNet->Close();
	return quitByMenu;
}

DWORD WINAPI GameMode::SelectTeamThread(void* param)
{
	GameMode* gm = (GameMode*) param;
	gm->mTeam = gm->mChooseTeamMenu->Run();
	MsgHandler::GetInstance().JoinTeam((TEAM)gm->mTeam);

	return 0;
}
DWORD WINAPI GameMode::InGameMenuThread(void* param)
{
	ThreadParam* tp = (ThreadParam*) param;
	tp->resume = tp->igm->Run();
	tp->finished = true;
	return 0;
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
	
	if(this->mServerInfo.GetGameMode() == GAMEMODE::WARLOCK)
	{
		if(key == '1')
		{
			if(mBalls[index]->GetSpells()[0]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 1);
			mBalls[index]->UseSpell(1);
		}
		if(key == '2')
		{
			if(mBalls[index]->GetSpells()[1]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 2);
			mBalls[index]->UseSpell(2);
		}
		if(key == '3')
		{
			if(mBalls[index]->GetSpells()[2]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 3);
			mBalls[index]->UseSpell(3);
		}
		if(key == '4')
		{
			if(mBalls[index]->GetSpells()[3]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 4);
			mBalls[index]->UseSpell(4);
		}
		if(key == VK_SPACE)
		{
			if(mBalls[index]->GetSpells()[4]->ReadyToBeCast())
				MsgHandler::GetInstance().SendCastSpell(index, 5);
			mBalls[index]->UseSpell(5);
		}
	}
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
		if(this->mServerInfo.GetGameMode() == GAMEMODE::WARLOCK)
		{
			if(mGe->GetKeyListener()->IsPressed('1'))
			{
				if(this->mNet->IsServer() && mBalls[index]->GetSpells()[0]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 1);
				mBalls[index]->UseSpell(1);
			}
			if(mGe->GetKeyListener()->IsPressed('2'))
			{
				if(this->mNet->IsServer() && mBalls[index]->GetSpells()[1]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 2);
				mBalls[index]->UseSpell(2);
			}
			if(mGe->GetKeyListener()->IsPressed('3'))
			{
				if(this->mNet->IsServer() && mBalls[index]->GetSpells()[2]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 3);
				mBalls[index]->UseSpell(3);
			}
			if(mGe->GetKeyListener()->IsPressed('4'))
			{
				if(this->mNet->IsServer() && mBalls[index]->GetSpells()[3]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 4);
				mBalls[index]->UseSpell(4);
			}
			if(mGe->GetKeyListener()->IsPressed(VK_SPACE))
			{
				if(this->mNet->IsServer() && mBalls[index]->GetSpells()[4]->ReadyToBeCast())
					MsgHandler::GetInstance().SendCastSpell(index, 5);
				mBalls[index]->UseSpell(5);
			}
		}
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

			if(this->mNet->GetBall(i)->GetNumCommands() > 0)
			{
				this->mBalls[i]->UseSpell((int)this->mNet->GetBall(i)->GetNextCommand()->GetInput(0));
				this->mNet->GetBall(i)->PopCommand();
			}
				
			for(int c = 0; c < this->mBalls[i]->GetNrOfSpells(); c++)
				this->mBalls[i]->GetSpells()[c]->UpdateSpecial(diff * 0.001f);
			

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

bool GameMode::PlayRoundLan(bool& roundsLeft, bool& zoomInPressed, bool& zoomOutPressed)
{
	bool running = true;
	bool quitByMenu = false;
	int numAlivePlayers = 0;
	LARGE_INTEGER oldTick = LARGE_INTEGER();
	QueryPerformanceCounter(&oldTick);
	LARGE_INTEGER now =  LARGE_INTEGER();

	ThreadParam* tp = NULL;
	bool resume = true;
	bool menuPressed = false;
	#if FixedTimeStep
		while(running && this->mGe->isRunning())
		{
				float diff = mGe->Update(); //A problem when the user opens up ingame menu is that the diff after resume is incredibly high so it breaks game logic, game gotta continue in the background if network :P	

				if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE) && tp == NULL)
				{
					this->HideHud();
					menuPressed = true;
					tp = new ThreadParam(this->mIGM, resume);
					CreateThread(0, 0, &InGameMenuThread, (void*)tp, 0, 0);
				}
				if(tp != NULL)
				{
					if(tp->finished)
					{
						roundsLeft = running = tp->resume;
						quitByMenu = !running;
						delete tp;
						tp = NULL;
						menuPressed = false;
					}
				}
					
				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					if(this->mBalls[i]->GetTeamColor() != this->mNet->GetBall(i)->GetTeam()) //causes lag otherwise re-setting the color every frame if its alrdy set.
						this->mBalls[i]->SetTeamColor(this->mNet->GetBall(i)->GetTeam());
				}

				if(this->mNet->IsServer())
					this->mPe->SimulateServer();
				else //is client
					this->mPe->SimulateClient();
				
					
				this->mNumberOfPlayers = this->mPe->Size();
					
				if(this->mNet->GetNumPlayers() > this->mNumberOfPlayers)
				{
					this->AddBall();
					numAlivePlayers++;
				}
					
				for(int i = 0; i < this->mNumberOfPlayers; i++)
				{
					if(this->mBalls[i]->IsAlive())
						numAlivePlayers += 1;
				}
				if(this->mNet->IsServer())
				if(numAlivePlayers < 1)
				{
					running = false;
				}
				
				if(this->checkWinConditions(diff))
					running = false;
				if(!menuPressed)
					this->ShowHud();
				
			

				float newdt = diff/1000.0f;
				
				this->mTimeElapsed += newdt;
				if(this->mTimeElapsed > 600.0f)
					running = false;
				if(this->mQuitByMenu)
				{
					quitByMenu = true;
					running = false;
				}
		}
	#else
		while(running && this->mGe->isRunning())
		{
				float diff = mGe->Update(); //A problem when the user opens up ingame menu is that the diff after resume is incredibly high so it breaks game logic, game gotta continue in the background if network :P	

				if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE) && tp == NULL)
				{
					this->HideHud();
					menuPressed = true;
					tp = new ThreadParam(this->mIGM, resume);
					CreateThread(0, 0, &InGameMenuThread, (void*)tp, 0, 0);
				}
				if(tp != NULL)
				{
					if(tp->finished)
					{
						roundsLeft = running = tp->resume;
						quitByMenu = !running;
						delete tp;
						tp = NULL;
						menuPressed = false;
					}
				}
		
				QueryPerformanceCounter(&now);
				LARGE_INTEGER proc_freq;
				::QueryPerformanceFrequency(&proc_freq);
				double frequency = proc_freq.QuadPart;

				diff = 1000*((now.QuadPart - oldTick.QuadPart) / frequency); //2			WITH A VARIABLE DELTATIME THE BALL PHYSICS RESULT DIFFER IF MORE THAN TWO CLIENTS WITH DIFFERENT DELTA TIMES PROCESS EXACTLY THE SAME INPUT, SETTING A CONSTANT DELTATIME HOWEVER LEADS TO THE SAME PHYSICS RESULT (THOUGH WITH A HUGE DELAY DUE TO THE CLIENT IN THE BACKGROUND IS A ASSIGNED LESS CPU TIME (-> low FPS)). IS THERE SOMETHING IN BALL PHYSICS THAT SHOULD BE DEPENDANT ON DELTATIME THAT ISNT?//
				QueryPerformanceCounter(&oldTick);
				if(diff > 100)
					diff = 5;

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
				if(numAlivePlayers < 1)
				{
					running = false;
				}
				
				if(this->checkWinConditions(diff))
					running = false;
				if(!menuPressed) 
					this->ShowHud();
				
					

				float newdt = diff/1000.0f;
				
				this->mTimeElapsed += newdt;
				if(this->mTimeElapsed > 600.0f)
					running = false;
				if(this->mQuitByMenu)
				{
					quitByMenu = true;
					running = false;
				}
		}
	#endif
		
	#if FixedTimeStep
		this->mPe->ResetTimers();
			
	#else
			
	#endif
		
	return quitByMenu;
}

void GameMode::AddBall()
{

}

void GameMode::ShowHud()
{
	//show time elapsed
	float tmp = floor(this->mTimeElapsed * 10.0f) / 10.0f;
	this->mTimeElapsedText->SetText(MaloW::convertNrToString(tmp));
}

void GameMode::HideHud()
{
	this->mTimeElapsedText->SetText("");
}