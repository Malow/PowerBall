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
		if(mGe->GetKeyListener()->IsPressed('H'))
			mBalls[1]->AddForce(Vector3(-diff,0,0));	
		if(mGe->GetKeyListener()->IsPressed('K'))
			mBalls[1]->AddForce(Vector3(diff,0,0));
		if(mGe->GetKeyListener()->IsPressed('U'))
			mBalls[1]->AddForce(Vector3(0,0,diff));	
		if(mGe->GetKeyListener()->IsPressed('J'))
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
		mBalls[index]->AddForce(Vector3(0, diff,0));
	
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
	char keysToCheck[11] = {'A', 'D', 'W', 'S', 'Q', 'E', VK_SPACE, '1', '2', '3', '4'};
	int numKeys = 0;
	for(int i = 0; i < 11; i++)
	{
		if(mGe->GetKeyListener()->IsPressed(keysToCheck[i]) && numKeys < 5)
			keyDowns[numKeys++] = keysToCheck[i];
	}
				
	if(numKeys == 0)
		keyDowns[numKeys++] = '?'; //"idle"-key

	this->mNet->AddKeyInput(clientIndex, keyDowns, numKeys, diff);
	
}
void GameMode::HandleClientKeyInputs(const int clientIndex, float diff)
{
	
	int flip = 1;
	if(this->mNet->GetStartPos(clientIndex).z > 0)
		flip = -1;
	//keep reading client inputs until the sum of all DT has exceeded server DT (->not allowed to move any more)
	KeyInput* command = this->mNet->GetNextCommand(clientIndex);
	float duration = 0.0f;
	if(command != NULL)
	{
		duration = command->dt;
		while(duration <=  diff && command != NULL)
		{
			for(int c = 0; c < command->numKeys; c++)
			{
				this->ClientKeyPress(command->dt, clientIndex, command->keys[c]);
			}
			this->mNet->SetExecTime(this->mNet->GetExecTime(clientIndex) + command->dt, clientIndex);
			this->mNet->PopCommand(clientIndex);


			command = this->mNet->GetNextCommand(clientIndex);
			if(command != NULL)
				duration += command->dt;
								
		}
		if(duration > diff && command != NULL)
		{
			duration -= command->dt;
									
			for(int c = 0; c < command->numKeys; c++)
			{
				//ADD A CHECK HERE SO THAT THE SAME KEY CANT APPEAR MORE THAN ONCE IN THE ARRAY (COULD CHEAT THE SYSTEM THIS WAY)
				
				this->ClientKeyPress((diff - duration), clientIndex, command->keys[c]);
			}

			command->dt -= (diff - duration);
								
			this->mNet->SetExecTime(this->mNet->GetExecTime(clientIndex) + (diff - duration), clientIndex);
		}
	}
}