#include "SystemReqRestart.h"
SystemReqRestart::SystemReqRestart()
{
	this->mNrOfSets = 0;
	this->mCurrentSet = 0;
	this->mIsRunning = false;

	this->mSets = NULL;
	this->mGe = NULL;
}
SystemReqRestart::SystemReqRestart(GraphicsEngine* ge)
{
	this->mNrOfSets = 10;
	this->mCurrentSet = SRRMENU;
	this->mIsRunning = false;

	this->mSets = new GUISet[this->mNrOfSets]();
	this->mGe = ge;

	this->Initialize();
}
SystemReqRestart::~SystemReqRestart()
{
	SAFE_DELETE_ARRAY(this->mSets);
}
bool SystemReqRestart::Initialize()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	/*Some calcs to make it easier*/
	float imageWidthHalf = (dx * (400.0f / 1200.0f)) / 2;
	float middleOfScreen = (dx / 2);

	Element* tempElement = new GUIPicture(0,0,0, "Media/Menus/BackscreenOpacity80.png", windowWidth, windowHeight);

	this->mSets[SRRMENU].AddElement(tempElement);

	float x = dx * (300.0f / 1200.0f) + offSet;
	float y = windowHeight * (300.0f / 900.0f);

	float width = dx * (600.0f / 1200.0f);
	float height = windowHeight * (300.0f / 900.0f);

	float buttonX = x + dx * (165.0f / 1200.0f);
	float buttonY = y + windowHeight * (136.0f / 900.0f);

	float buttonWidth = dx * (262.0f / 1200.0f);
	float buttonHeight = windowHeight * (110.0f / 900.0f);

	tempElement = new SimpleButton(x, y, 1, "Media/MenuSet/ReqSysResart.png", width, height, new ChangeOptionEvent("Leave", "true"),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png",buttonX, buttonY, buttonWidth, buttonHeight);
	this->mSets[SRRMENU].AddElement(tempElement);

	tempElement = NULL;


	return true;
}
int SystemReqRestart::Run()
{
	this->mCurrentSet = SRRMENU;
	this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
	
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

		this->mGe->Update();
		IsClicked = this->mGe->GetKeyListener()->IsClicked(1);

		
		mousePos = this->mGe->GetKeyListener()->GetMousePosition();
		returnEvent = this->mSets[this->mCurrentSet].UpdateAndCheckCollision(mousePos.x, mousePos.y, IsClicked, this->mGe);

		if(returnEvent != NULL)
		{
			if(returnEvent->GetEventMessage() == "ChangeOptionEvent")
			{
				ChangeOptionEvent* temp = (ChangeOptionEvent*)returnEvent;
				string value = temp->GetValue();
				this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
				if(value == "true")
					this->mIsRunning = false;
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