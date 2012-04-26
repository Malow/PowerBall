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
	/*Element* tempElement =  new GUIArchButton(0, 0, 1, "Media/MainMenu_Wallpaper_1920x1080.png", windowWidth, windowHeight, new NoEvent(), "", "");
	this->mSets[IGNBACKGROUND].AddElement(tempElement);*/

	Element* tempElement = new SimpleButton(windowWidth * 0.35f, windowHeight * 0.19f, 0, "Media/ResumeGame.png", windowWidth * 0.30f, windowHeight * 0.15f, new ChangeSetEvent(IGNRESUME), "", "", windowWidth * 0.35f, windowHeight * 0.19f, windowWidth * 0.30f, windowHeight * 0.15f);
	this->mSets[INGAMEMENU].AddElement(tempElement);

	tempElement = new SimpleButton(windowWidth * 0.35f, windowHeight * 0.37f, 0, "Media/Options.png", windowWidth * 0.30f, windowHeight * 0.15f, new ChangeSetEvent(IGNOPTIONS), "", "", windowWidth * 0.35f, windowHeight * 0.37f, windowWidth * 0.30f, windowHeight * 0.15f);
	this->mSets[INGAMEMENU].AddElement(tempElement);

	tempElement = new SimpleButton(windowWidth * 0.35f, windowHeight * 0.55f, 0, "Media/Quit.png", windowWidth * 0.30f, windowHeight * 0.15f, new ChangeSetEvent(IGNQUIT), "", "", windowWidth * 0.35f, windowHeight * 0.55f, windowWidth * 0.30f, windowHeight * 0.15f);
	this->mSets[INGAMEMENU].AddElement(tempElement);

	/* Adding the buttons for the options menu*/
	tempElement = new GUIArchButton(offSet, 0, 1, "Media/optionsmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[IGNOPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbacktomenu.png", dx, windowHeight, new ChangeSetEvent(INGAMEMENU), "Media/clickbacktomenu.png" , "Media/mouseoverbacktomenu.png", dx * (18.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (325.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttongraphics.png", dx, windowHeight, new NoEvent(), "Media/clickgraphics.png", "Media/mouseovergraphics.png", dx * (42.0f / 1200) + offSet, windowHeight * (198.0f / 900), dx * (197.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);
	
	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbasic.png", dx, windowHeight, new NoEvent(), "Media/clickbasic.png", "Media/mouseoverbasic.png", dx * (370.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);

	//Ugly drop down list to start with
	float dropX = dx * (380.0f / 1200.0f) + offSet, dropY = windowHeight * (80.0f / 900.0f);
	tempElement = new DropDownList(dropX, dropY,1.0f,"Media/DropDownMenu.png", 300.0f, 60.0f);
	DropDownList* dropdownlist = (DropDownList*)tempElement;

	dropdownlist->AddButton((dropX+73.0f), (dropY+27.0f), 1.0f, "Media/Options1920x1080.png", 200.0f, 30.0f, new NoEvent(), "Media/PressedOptions1920x1080.png", "Media/MouseOverOptions1920x1080.png", (dropX+73.0f), (dropY+27.0f), 200.0f, 30.0f);
	dropdownlist->AddButton((dropX+73.0f), (dropY+27.0f)+31.0f, 1.0f, "Media/Options1680x1050.png", 200.0f, 30.0f, new NoEvent(), "Media/PressedOptions1680x1050.png", "Media/MouseOverOptions1680x1050.png", (dropX+73.0f), (dropY+27.0f)+31.0f, 200.0f, 30.0f);

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
				int whatSet;
				ChangeSetEvent* temp = (ChangeSetEvent*)returnEvent;
				temp->GetSet(whatSet);
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
	}
	return true;
}