#include "MainMenu.h"

MainMenu::MainMenu(GraphicsEngine* ge)
{
	this->mNrOfSets = 0;
	this->mSets = 0;
	this->mRunning = true;
	this->mCurrentSet = 0;
	this->mGm = new GameManager(ge);
	this->mGe = ge;

	this->Initialize();
}
MainMenu::~MainMenu()
{
	delete [] this->mGm;
}

bool MainMenu::Initialize()
{
	this->mNrOfSets = 2;
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	this->mSets = new GUISet[this->mNrOfSets]();

	Element tempElement = GUIPicture(0, 0, 1, "MainMenu_Wallpaper_1920x1080.png", windowWidth, windowHeight, new NoEvent());
	for(int i = 0; i < this->mNrOfSets; i++)
	{
		this->mSets[i].AddElement(tempElement);
	}

	tempElement = SimpleButton((float)windowWidth*0.03, (float)windowHeight*0.76, 0, "MainMenu_Options.png", (float)windowWidth*0.30, (float)windowHeight*0.15, new ChangeSetEvent(OPTIONS));
	this->mSets[0].AddElement(tempElement);

	tempElement = SimpleButton((float)windowWidth*0.35, (float)windowHeight*0.68, 0, "MainMenu_Play.png", (float)windowWidth*0.30, (float)windowHeight*0.15, new ChangeSetEvent(PLAY));
	this->mSets[0].AddElement(tempElement);

	tempElement = SimpleButton((float)windowWidth*0.35, (float)windowHeight*0.86, 0, "MainMenu_Credit.png", (float)windowWidth*0.30, (float)windowHeight*0.12, new ChangeSetEvent(CREDIT));
	this->mSets[0].AddElement(tempElement);

	tempElement = SimpleButton((float)windowWidth*0.67, (float)windowHeight*0.76, 0, "MainMenu_Quit.png", (float)windowWidth*0.30, (float)windowHeight*0.15, new ChangeSetEvent(QUIT));
	this->mSets[0].AddElement(tempElement);

	this->mSets[0].AddSetToRenderer(this->mGe);

	return true;
}

bool MainMenu::Run()
{
	Event *returnEvent;
	while(this->mGe->isRunning())
	{
		this->mGe->Update();
		if(this->mGe->GetKeyListener()->IsClicked(1))
		{
			D3DXVECTOR2 mousePos;
			mousePos = this->mGe->GetKeyListener()->GetMousePosition();
			returnEvent = this->mSets[this->mCurrentSet].CheckCollision(mousePos.x, mousePos.y);

			if(returnEvent->GetEventMessage() == "ChangeSetEvent")
			{
				int whatSet;
				ChangeSetEvent* temp = (ChangeSetEvent*)returnEvent;
				temp->GetSet(whatSet);
				if(whatSet == PLAY)
				{
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
					this->mCurrentSet = 1;
					this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
				}
			}
		}
		
	}

	return true;
}