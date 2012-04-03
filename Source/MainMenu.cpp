#include "MainMenu.h"

MainMenu::MainMenu()
{
	this->mNrOfSets = 0;
	this->mSets = 0;
	this->mRunning = true;
	this->mCurrentSet = 0;
	this->mGm = new GameManager();

	this->Initialize();
}

bool MainMenu::Initialize()
{
	this->mNrOfSets = 1;

	this->mSets = new GUISet[this->mNrOfSets]();
	

	Element tempElement = GUIPicture(0,0,1, L'MainMenu_Wallpaper_1920x1080',1,1);
	this->mSets[0].AddElement(tempElement);

	this->mSets[0].AddSetToRenderer();

}

bool MainMenu::Run()
{
	Event* getEvent = new Event('PLAY', ' ');
	while(this->mRunning)
	{
		if(getEvent->GetEventMessage() == 'PLAY')
		{
			this->mSets[this->mCurrentSet].RemoveSetFromRenderer();
			this->mGm->Play(2);
			this->mCurrentSet = 0;
		}
		//this->mSets[this->mCurrentSet].Update();
	}
	delete [] getEvent;
}