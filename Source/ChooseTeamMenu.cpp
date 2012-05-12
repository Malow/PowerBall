#include "ChooseTeamMenu.h"
ChooseTeamMenu::ChooseTeamMenu()
{
	this->mNrOfSets = 0;
	this->mCurrentSet = 0;
	this->mIsRunning = false;

	this->mSets = NULL;
	this->mGe = NULL;
}
ChooseTeamMenu::ChooseTeamMenu(GraphicsEngine* ge)
{
	this->mNrOfSets = 10;
	this->mCurrentSet = CTMENU;
	this->mIsRunning = false;

	this->mSets = new GUISet[this->mNrOfSets]();
	this->mGe = ge;

	this->Initialize();
}
ChooseTeamMenu::~ChooseTeamMenu()
{
	SAFE_DELETE_ARRAY(this->mSets);
}
bool ChooseTeamMenu::Initialize()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	/*Some calcs to make it easier*/
	float imageWidthHalf = (dx * (400.0f / 1200.0f)) / 2;
	float middleOfScreen = (dx / 2);

	Element* tempElement = new GUIPicture(0,0,0, "Media/Menus/BackscreenOpacity80.png", windowWidth, windowHeight);

	this->mSets[CTMENU].AddElement(tempElement);

	tempElement = new SimpleButton((dx * ((1200.0f/4 - 400.0f/2) / 1200.0f))+(offSet), windowHeight * ((900.0f/2 - 150.0f/2) / 900.0f), 1, "Media/Menus/BlueTeamButton.png", dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f), new SelectTeamEvent(BLUE_TEAM),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png",(dx * ((1200.0f/4 - 400.0f/2) / 1200.0f))+(offSet), windowHeight * ((900.0f/2 - 150.0f/2) / 900.0f), dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f));

	this->mSets[CTMENU].AddElement(tempElement);


	tempElement = new SimpleButton((dx * (((1200.0f/4) * 3 - 400.0f/2) / 1200.0f))+(offSet), windowHeight * ((900.0f/2 - 150.0f/2) / 900.0f), 0, "Media/Menus/RedTeamButton.png", dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f), new SelectTeamEvent(RED_TEAM),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png", (dx * (((1200.0f/4) * 3 - 400.0f/2) / 1200.0f))+(offSet),windowHeight * ((900.0f/2 - 150.0f/2) / 900.0f), dx * (400.0f / 1200.0f), windowHeight * (150.0f / 900.0f));

	this->mSets[CTMENU].AddElement(tempElement);

	tempElement = NULL;


	return true;
}
int ChooseTeamMenu::Run()
{
	this->mSets[CTMENU].AddSetToRenderer(this->mGe);
	this->mIsRunning = true;
	bool IsClicked = false;
	bool mousePressed = false;
	D3DXVECTOR2 mousePos;

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
			this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
			this->mIsRunning = false;
			this->mGe->GetKeyListener()->KeyUp(VK_ESCAPE);
			return true;
		}

		this->mGe->Update();
		IsClicked = this->mGe->GetKeyListener()->IsClicked(1);

		
		mousePos = this->mGe->GetKeyListener()->GetMousePosition();
		returnEvent = this->mSets[this->mCurrentSet].UpdateAndCheckCollision(mousePos.x, mousePos.y, IsClicked, this->mGe);

		if(returnEvent != NULL)
		{
			if(returnEvent->GetEventMessage() == "SelectTeamEvent")
			{
				SelectTeamEvent* temp = (SelectTeamEvent*)returnEvent;
				int whatTeam = temp->GetTeam();
				this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
				return whatTeam;
			}
		}
		if(!IsClicked && mousePressed)
		{
			mousePressed = false;
		}
		if(!this->mGe->isRunning())
			this->mIsRunning = false;
	}
	return -1;
}