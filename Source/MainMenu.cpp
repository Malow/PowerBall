#include "MainMenu.h"

MainMenu::MainMenu(GraphicsEngine* ge)
{
	this->mNrOfSets = 10;
	this->mSets = new GUISet[this->mNrOfSets]();
	this->mRunning = true;
	this->mCurrentSet = MAINMENU;
	this->mGm = new GameManager(ge);
	this->mGe = ge;
	
	this->Initialize();
}
MainMenu::~MainMenu()
{
	SAFE_DELETE(this->mGm);
	SAFE_DELETE_ARRAY(this->mSets);
}

bool MainMenu::Initialize()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	
	Element* tempElement = new GUIPicture(0, 0, 1, "MainMenu_Wallpaper_1920x1080.png", windowWidth, windowHeight, new NoEvent());
	this->mSets[BACKGROUND].AddElement(tempElement);
	
	tempElement = new SimpleButton((float)windowWidth*0.03, (float)windowHeight*0.76, 0, "MainMenu_Options.png", (float)windowWidth*0.30, (float)windowHeight*0.15, new ChangeSetEvent(OPTIONS));
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new SimpleButton((float)windowWidth*0.35, (float)windowHeight*0.68, 0, "MainMenu_Play.png", (float)windowWidth*0.30, (float)windowHeight*0.15, new ChangeSetEvent(PLAY));
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new SimpleButton((float)windowWidth*0.35, (float)windowHeight*0.86, 0, "MainMenu_Credit.png", (float)windowWidth*0.30, (float)windowHeight*0.12, new ChangeSetEvent(CREDIT));
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new SimpleButton((float)windowWidth*0.67, (float)windowHeight*0.76, 0, "MainMenu_Quit.png", (float)windowWidth*0.30, (float)windowHeight*0.15, new ChangeSetEvent(QUIT));
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement = new SimpleButton((float)windowWidth*0.03, (float)windowHeight*0.88, 0, "MainMenu_Back.png", (float)windowWidth*0.09, (float)windowHeight*0.07, new ChangeSetEvent(MAINMENU));
	this->mSets[OPTIONS].AddElement(tempElement);

	tempElement = NULL;
	this->mSets[BACKGROUND].AddSetToRenderer(this->mGe);
	this->mSets[MAINMENU].AddSetToRenderer(this->mGe);
	
	return true;
}

bool MainMenu::Run()
{
	Event *returnEvent = NULL;
	while(this->mGe->isRunning())
	{
		this->mGe->Update();
		if(this->mGe->GetKeyListener()->IsClicked(1))
		{
			D3DXVECTOR2 mousePos;
			mousePos = this->mGe->GetKeyListener()->GetMousePosition();
			returnEvent = this->mSets[this->mCurrentSet].CheckCollision(mousePos.x, mousePos.y);
			if(returnEvent != NULL)
			{
				if(returnEvent->GetEventMessage() == "ChangeSetEvent")
				{
					int whatSet;
					ChangeSetEvent* temp = (ChangeSetEvent*)returnEvent;
					temp->GetSet(whatSet);
					if(whatSet == PLAY)
					{
						this->mSets[BACKGROUND].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mGm->Play(2);
						this->mCurrentSet = 0;
					}
					if(whatSet == QUIT)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						return false;
					}
					if(whatSet == OPTIONS)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mCurrentSet = OPTIONS;
						this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
					}
					if(whatSet == MAINMENU)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mCurrentSet = MAINMENU;
						this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
					}
				}
			}
		}
		returnEvent = NULL;
	}

	return true;
}