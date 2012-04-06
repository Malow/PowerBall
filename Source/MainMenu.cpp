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
	
	Element* tempElement = new GUIPicture(0, 0, 1, "Media/MainMenu_Wallpaper_1920x1080.png", windowWidth, windowHeight, new NoEvent());
	this->mSets[BACKGROUND].AddElement(tempElement);
	
	tempElement = new SimpleButton(windowWidth * 0.03f, windowHeight * 0.76f, 0, "Media/MainMenu_Options.png", windowWidth * 0.30f, windowHeight * 0.15f, new ChangeSetEvent(OPTIONS));
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement = new SimpleButton(windowWidth * 0.35f, windowHeight * 0.68f, 0, "Media/MainMenu_Play.png", windowWidth * 0.30f, windowHeight * 0.15f, new ChangeSetEvent(PLAY));
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new SimpleButton(windowWidth * 0.35f, windowHeight * 0.86f, 0, "Media/MainMenu_Credit.png", windowWidth * 0.30f, windowHeight * 0.12f, new ChangeSetEvent(CREDIT));
	this->mSets[MAINMENU].AddElement(tempElement);
	  
	tempElement = new SimpleButton(windowWidth * 0.67f, windowHeight * 0.76f, 0, "Media/MainMenu_Quit.png", windowWidth * 0.30f, windowHeight * 0.15f, new ChangeSetEvent(QUIT));
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement = new SimpleButton(windowWidth * 0.03f, windowHeight * 0.88f, 0, "Media/MainMenu_Back.png", windowWidth * 0.09f, windowHeight * 0.07f, new ChangeSetEvent(MAINMENU));
	this->mSets[OPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(windowWidth * 0.03f, windowHeight * 0.88f, 0, "Media/MainMenu_Back.png", windowWidth * 0.09f, windowHeight * 0.07f, new ChangeSetEvent(MAINMENU));
	this->mSets[CREDIT].AddElement(tempElement);

	tempElement = new GUIPicture(windowWidth * 0.19f, windowHeight * 0.08f, 1, "Media/MainMenu_Wallpaper_Credit.png", windowWidth * 0.62f, windowHeight * 0.84f, new NoEvent());
	this->mSets[CREDIT].AddElement(tempElement);

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
					if(whatSet == CREDIT)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mCurrentSet = CREDIT;
						this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
					}
				}
			}
		}
		returnEvent = NULL;
	}

	return true;
}