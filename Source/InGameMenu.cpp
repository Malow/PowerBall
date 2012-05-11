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
	this->mSubSet = IGNNOMENU;
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
	tempElement = new GUIPicture(offSet, 0, 1, "Media/OptionsMenu/optionsmenu.png", dx, windowHeight);
	this->mSets[IGNOPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonbacktomenu.png", dx, windowHeight, new ChangeSetEvent(INGAMEMENU), 
		"Media/OptionsMenu/clickbacktomenu.png" , "Media/OptionsMenu/mouseoverbacktomenu.png", dx * (18.0f / 1200)+offSet, windowHeight * (847.0f / 900),
		dx * (325.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttongraphics.png", dx, windowHeight, new ChangeSubSetEvent(IGNOPTIONS_GRAPHICS, IGNNOMENU), 
		"Media/OptionsMenu/clickgraphics.png", "Media/OptionsMenu/mouseovergraphics.png", dx * (42.0f / 1200) + offSet, windowHeight * (198.0f / 900), 
		dx * (197.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonsound.png", dx, windowHeight, new ChangeSubSetEvent(IGNOPTIONS_SOUND, IGNNOMENU), 
		"Media/OptionsMenu/clicksound.png", "Media/OptionsMenu/mouseoversound.png", dx * (42.0f / 1200) + offSet, windowHeight * (248.0f / 900), 
		dx * (130.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);
	
	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonbasic.png", dx, windowHeight, new NoEvent(),
		"Media/OptionsMenu/clickbasic.png", "Media/OptionsMenu/mouseoverbasic.png", dx * (370.0f / 1200) + offSet, windowHeight * (26.0f / 900), 
		dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[IGNOPTIONS].AddElement(tempElement);
	/*
	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonadvanced.png", dx, windowHeight, new NoEvent(), "Media/OptionsMenu/clickadvanced.png", "Media/OptionsMenu/mouseoveradvanced.png", dx * (496.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (222.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	*/
	

	/*
	* Subset code start here
	*/
	tempElement = new GUIPicture(dx * (400.0f / 1200.0f) + offSet, windowHeight * (100.0f / 900.0f), 1, "Media/Menus/Sound.png",
		dx * (155.0f / 1200.0f), windowHeight * (30.0f / 900.0f));
	this->mSets[IGNOPTIONS_SOUND].AddElement(tempElement);

	tempElement = new CheckBox(dx * (555.0f / 1200.0f) + offSet, windowHeight * (100.0f / 900.0f), 1, "Media/Menus/CheckBoxFrame.png",dx * (30.0f / 1200.0f), windowHeight * (30.0f / 900.0f),
		"Media/Menus/CheckBoxChecked.png", true, new ChangeOptionEvent("Sound", "true"), "Sound");
	this->mSets[IGNOPTIONS_SOUND].AddElement(tempElement);


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
		if(returnEvent == NULL)
			returnEvent = this->mSets[this->mSubSet].UpdateAndCheckCollision(mousePos.x, mousePos.y, IsClicked, this->mGe);

		if(returnEvent != NULL)
		{
			if(returnEvent->GetEventMessage() == "ChangeSetEvent")
			{
				ChangeSetEvent* temp = (ChangeSetEvent*)returnEvent;
				int whatSet = temp->GetSet();
				if(whatSet == IGNQUIT)
				{
					
					this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
					this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
					this->mIsRunning = false;
					return false;
				}
				if(whatSet == IGNRESUME)
				{
					
					this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
					this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
					this->mIsRunning = false;
					return true;
				}
				if(whatSet == IGNOPTIONS)
				{
					this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
					this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
					this->mCurrentSet = IGNOPTIONS;
					this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
					this->mSets[this->mSubSet].AddSetToRenderer(this->mGe);
				}
				if(whatSet == INGAMEMENU)
				{
					this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
					this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
					this->mCurrentSet = INGAMEMENU;
					this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
				}
			}
			else if(returnEvent->GetEventMessage() == "ChangeSubSetEvent")
			{
				this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);

				ChangeSubSetEvent* tempReturnEvent = (ChangeSubSetEvent*)returnEvent;
				int tempEventSet = tempReturnEvent->GetSet();

				if(tempEventSet == IGNOPTIONS_SOUND)
				{
					CheckBox* temp = this->mSets[IGNOPTIONS_SOUND].GetCheckBox("Sound");
					temp->SetChecked(BackgroundSong::mPlaying);
				}
				int set = tempReturnEvent->GetSet();
				this->mSubSet = set;
				this->mSets[this->mSubSet].AddSetToRenderer(this->mGe);
				int menuChangeTime = 50;
				while(menuChangeTime > 0)
					menuChangeTime -= this->mGe->Update();
			}
			else if(returnEvent->GetEventMessage() == "ChangeOptionEvent")
			{
				ChangeOptionEvent* tempReturnEvent = (ChangeOptionEvent*)returnEvent;
				if(tempReturnEvent->GetOption() == "Sound")
				{
					if(tempReturnEvent->GetValue() == "true")
					{
						BackgroundSong::mSong->Unmute();
						BackgroundSong::mPlaying = true;
					}
					else
					{
						BackgroundSong::mSong->Mute();
						BackgroundSong::mPlaying = false;
					}
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