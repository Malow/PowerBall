#include "InGameMenu.h"
InGameMenu::InGameMenu()
{
	this->mNrOfSets = 0;
	this->mCurrentSet = 0;
	this->mIsRunning = false;

	this->mSets = NULL;
	this->mGe = NULL;
}
InGameMenu::InGameMenu(GraphicsEngine* ge)
{
	this->mNrOfSets = 10;
	this->mCurrentSet = INGAMEMENU;
	this->mIsRunning = false;

	this->mSets = new GUISet[this->mNrOfSets]();
	this->mGe = ge;

	this->Initialize();
}
InGameMenu::~InGameMenu()
{
	SAFE_DELETE_ARRAY(this->mSets);
}
bool InGameMenu::Initialize()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;
	/*Element* tempElement =  new GUIArchButton(0, 0, 1, "Media/MainMenu_Circle/MainMenu_Wallpaper_1920_1080.png", windowWidth, windowHeight, new NoEvent(), "Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	this->mSets[IGNBACKGROUND].AddElement(tempElement);*/

	/*Some calcs to make it easier*/
	float imageWidthHalf = (dx * (400.0f / 1200.0f)) / 2;
	float middleOfScreen = (dx / 2);

	Element* tempElement = new SimpleButton(middleOfScreen - imageWidthHalf + offSet, windowHeight * (170.0f / 900.0f), 0, "Media/Menus/ResumeGame.png", dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f), new ChangeSetEvent(IGNRESUME),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png", middleOfScreen - imageWidthHalf + offSet, windowHeight * (170.0f / 900.0f), dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f));

	this->mSets[INGAMEMENU].AddElement(tempElement);


	tempElement = new SimpleButton(middleOfScreen - imageWidthHalf + offSet, windowHeight * (325.0f / 900.0f), 0, "Media/Menus/Options.png", dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f), new ChangeSetEvent(IGNOPTIONS),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png", middleOfScreen - imageWidthHalf + offSet, windowHeight * (325.0f / 900.0f), dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f));

	this->mSets[INGAMEMENU].AddElement(tempElement);


	tempElement = new SimpleButton(middleOfScreen - imageWidthHalf + offSet, windowHeight * (480.0f / 900.0f), 0, "Media/Menus/Quit.png", dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f), new ChangeSetEvent(IGNQUIT),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png", middleOfScreen - imageWidthHalf + offSet, windowHeight * (480.0f / 900.0f), dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f));


	this->mSets[INGAMEMENU].AddElement(tempElement);

	/* Adding the buttons for the options menu*/
	tempElement = new GUIArchButton(offSet, 0, 1, "Media/OptionsMenu/optionsmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[IGNOPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonbacktomenu.png", dx, windowHeight, new ChangeSetEvent(INGAMEMENU), "Media/OptionsMenu/clickbacktomenu.png" , "Media/OptionsMenu/mouseoverbacktomenu.png", dx * (18.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (325.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttongraphics.png", dx, windowHeight, new NoEvent(), "Media/OptionsMenu/clickgraphics.png", "Media/OptionsMenu/mouseovergraphics.png", dx * (42.0f / 1200) + offSet, windowHeight * (198.0f / 900), dx * (197.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);
	
	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonbasic.png", dx, windowHeight, new NoEvent(), "Media/OptionsMenu/clickbasic.png", "Media/OptionsMenu/mouseoverbasic.png", dx * (370.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);
	/*
	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonadvanced.png", dx, windowHeight, new NoEvent(), "Media/clickadvanced.png", "Media/mouseoveradvanced.png", dx * (496.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (222.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	*/
	tempElement = NULL;


	return true;
}
bool InGameMenu::Run()
{
	this->mSets[IGNBACKGROUND].AddSetToRenderer(this->mGe);
	this->mSets[INGAMEMENU].AddSetToRenderer(this->mGe);
	this->mIsRunning = true;
	bool IsClicked = false;
	bool mousePressed = false;
	GUIEvent *returnEvent = NULL;
	this->mGe->GetKeyListener()->KeyUp(VK_ESCAPE);
	while(this->mIsRunning)
	{
		/*If mouse is clicked*/
		if(IsClicked && !mousePressed)
		{
			mousePressed = true;
		}
		if(this->mGe->GetKeyListener()->IsPressed(VK_ESCAPE))
		{
			this->mSets[INGAMEMENU].RemoveSetFromRenderer(this->mGe);
			this->mIsRunning = false;
			this->mGe->GetKeyListener()->KeyUp(VK_ESCAPE);
			return true;
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
				if(whatSet == IGNQUIT)
				{
					
					this->mSets[INGAMEMENU].RemoveSetFromRenderer(this->mGe);
					this->mIsRunning = false;
					return false;
				}
				if(whatSet == IGNRESUME)
				{
					
					this->mSets[INGAMEMENU].RemoveSetFromRenderer(this->mGe);
					this->mIsRunning = false;
					return true;
				}
				if(whatSet == IGNOPTIONS)
				{
					this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
					this->mCurrentSet = IGNOPTIONS;
					this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
				}
				if(whatSet == INGAMEMENU)
				{
					this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
					this->mCurrentSet = INGAMEMENU;
					this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
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
	return true;
}