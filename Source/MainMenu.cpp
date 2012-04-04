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
	this->mNrOfSets = 1;

	this->mSets = new GUISet[this->mNrOfSets]();
	
	Event tempEvent = NoEvent();
	Element tempElement = GUIPicture(-1, -1, 1, "MainMenu_Wallpaper_1920x1080.png", 2, 2, tempEvent);
	this->mSets[0].AddElement(tempElement);

	tempEvent = ChangeSetEvent(1);
	tempElement = SimpleButton(0, 0, 0, "MainMenu_Play.png", 0.5, 0.5, tempEvent);
	this->mSets[0].AddElement(tempElement);

	this->mSets[0].AddSetToRenderer(this->mGe);

	return true;
}

bool MainMenu::Run()
{
	while(this->mGe->isRunning())
	{
		this->mGe->Update();
		//this->mSets[this->mCurrentSet].CheckCollision(this->mGe->GetM);
		if(this->mGe->GetKeyListener()->IsPressed('W'))
		{
			this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
			this->mGm->Play(2);
			this->mCurrentSet = 0;
			this->mRunning = false;
		}
	}

	return true;
}