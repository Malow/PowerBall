#include "EndGameMenu.h"
EndGameMenu::EndGameMenu()
{
	this->mNrOfSets = 0;
	this->mCurrentSet = 0;
	this->mIsRunning = false;

	this->mSets = NULL;
	this->mGe = NULL;
}
EndGameMenu::EndGameMenu(GraphicsEngine* ge)
{
	this->mNrOfSets = 10;
	this->mCurrentSet = EGMMENU;
	this->mIsRunning = false;

	this->mSets = new GUISet[this->mNrOfSets]();
	this->mGe = ge;

	this->Initialize();
}
EndGameMenu::~EndGameMenu()
{
	SAFE_DELETE_ARRAY(this->mSets);
}
bool EndGameMenu::Initialize()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	/*Some calcs to make it easier*/
	float imageWidthHalf = (dx * (400.0f / 1200.0f)) / 2;
	float middleOfScreen = (dx / 2);

	Element* tempElement = new SimpleButton(middleOfScreen - imageWidthHalf + offSet, windowHeight * (280.0f / 900.0f), 0, "Media/Menus/PlayAgain.png", dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f), new ChangeSetEvent(PLAYAGAIN),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png", middleOfScreen - imageWidthHalf + offSet, windowHeight * (325.0f / 900.0f), 
		dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f));

	this->mSets[EGMMENU].AddElement(tempElement);

	tempElement = new SimpleButton(middleOfScreen - imageWidthHalf + offSet, windowHeight * (500.0f / 900.0f), 0, "Media/Menus/Quit.png", dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f), new ChangeSetEvent(QUITGAME),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png", middleOfScreen - imageWidthHalf + offSet, windowHeight * (480.0f / 900.0f), 
		dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f));

	this->mSets[EGMMENU].AddElement(tempElement);

	tempElement = NULL;


	return true;
}
int EndGameMenu::Run()
{
	this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
	this->mIsRunning = true;
	bool IsClicked = false;
	bool mousePressed = false;
	GUIEvent *returnEvent = NULL;
	CursorControl cc;
	cc.SetVisibility(true);
	while(this->mGe->GetKeyListener()->IsClicked(1))
	{

	}

	while(this->mIsRunning)
	{
		/*If mouse is clicked*/
		if(IsClicked && !mousePressed)
		{
			mousePressed = true;
		}
		this->mGe->Update();
		IsClicked = this->mGe->GetKeyListener()->IsClicked(1);

		D3DXVECTOR2 mousePos;
		mousePos = this->mGe->GetKeyListener()->GetMousePosition();
		returnEvent = this->mSets[this->mCurrentSet].UpdateAndCheckCollision(mousePos.x, mousePos.y, IsClicked, this->mGe);

		if(returnEvent != NULL)
		{
			if(returnEvent->GetEventMessage() == "ChangeSetEvent")
			{
				ChangeSetEvent* temp = (ChangeSetEvent*)returnEvent;
				int whatSet = temp->GetSet();
				if(whatSet == QUITGAME)
				{
					
					this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
					this->mIsRunning = false;
					return QUITGAME;
				}
				if(whatSet == PLAYAGAIN)
				{
					
					this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
					this->mIsRunning = false;
					return PLAYAGAIN;
				}
			}
		}
		if(!IsClicked && mousePressed)
		{
			mousePressed = false;
		}
		if(!this->mGe->isRunning())
			this->mIsRunning = false;
	}
	cc.SetVisibility(false);
	return true;
}