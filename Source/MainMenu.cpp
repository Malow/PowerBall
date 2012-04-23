#include "MainMenu.h"

MainMenu::MainMenu(GraphicsEngine* ge)
{
	this->mNrOfSets = 10;
	this->mSets = new GUISet[this->mNrOfSets]();
	this->mRunning = true;
	this->mCurrentSet = MAINMENU;
	this->mGm = NULL;
	this->mGe = ge;
	
	this->Initialize();
}
MainMenu::~MainMenu()
{
	if(this->mSets)
		delete [] this->mSets;
}
bool MainMenu::Initialize()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;
	
	Element* tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/mainmenu.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new NoEvent(), " ", " ");
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonplay.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(PLAY), "Media/clickplay.png", "Media/mouseoverplay.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttoncredits.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(CREDIT), "Media/clickcredits.png", "Media/mouseovercredits.png");
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement	 = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonexit.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(EXIT), "Media/clickexit.png", "Media/mouseoverexit.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonoptions.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_GAMEPLAY), "Media/clickoptions.png", "Media/mouseoveroptions.png");
	this->mSets[MAINMENU].AddElement(tempElement);
	
	/* Adding the buttons for the options menu*/
	tempElement = new GUIArchButton(offSet, 0, 1, "Media/optionsmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbacktomenu.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/clickbacktomenu.png" , "Media/mouseoverbacktomenu.png", dx * (18.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (325.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttongraphics.png", dx, windowHeight, new NoEvent(), "Media/clickgraphics.png", "Media/mouseovergraphics.png", dx * (42.0f / 1200) + offSet, windowHeight * (198.0f / 900), dx * (197.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	
	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbasic.png", dx, windowHeight, new NoEvent(), "Media/clickbasic.png", "Media/mouseoverbasic.png", dx * (370.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	/*
	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonadvanced.png", dx, windowHeight, new NoEvent(), "Media/clickadvanced.png", "Media/mouseoveradvanced.png", dx * (496.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (222.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	*/

	//Ugly drop down list to start with
	float dropX = dx * (380.0f / 1200.0f) + offSet, dropY = windowHeight * (80.0f / 900.0f);
	tempElement = new DropDownList(dropX, dropY,1.0f,"Media/DropDownMenu.png", 300.0f, 60.0f);
	DropDownList* dropdownlist = (DropDownList*)tempElement;

	dropdownlist->AddButton((dropX+73.0f), (dropY+27.0f), 1.0f, "Media/Options1920x1080.png", 200.0f, 30.0f, new ChangeResEvent(1920, 1080), "Media/PressedOptions1920x1080.png", "Media/MouseOverOptions1920x1080.png", (dropX+73.0f), (dropY+27.0f), 200.0f, 30.0f);
	dropdownlist->AddButton((dropX+73.0f), (dropY+27.0f)+31.0f, 1.0f, "Media/Options1680x1050.png", 200.0f, 30.0f, new ChangeResEvent(1680, 1050), "Media/PressedOptions1680x1050.png", "Media/MouseOverOptions1680x1050.png", (dropX+73.0f), (dropY+27.0f)+31.0f, 200.0f, 30.0f);

	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	tempElement = NULL;
	
	return true;
}
void MainMenu::UpdateMousePosition()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float lengthFromMiddle = (windowHeight * 0.745f) / 3;
	D3DXVECTOR2 mouseVector = this->mGe->GetKeyListener()->GetMousePosition();
	D3DXVECTOR2 centerVector = D3DXVECTOR2(windowWidth / 2, windowHeight / 2);
	D3DXVECTOR2 centerToMouseVec = (centerVector - mouseVector);

	if(D3DXVec2Length(&centerToMouseVec) > lengthFromMiddle+1 || D3DXVec2Length(&centerToMouseVec) < lengthFromMiddle-1)
	{
		centerToMouseVec = (lengthFromMiddle / D3DXVec2Length(&centerToMouseVec)) * centerToMouseVec;
		this->mGe->GetKeyListener()->SetMousePosition(centerVector + (centerToMouseVec * (-1)));
	}
	
}

bool MainMenu::Run()
{
	this->mSets[MAINMENU].AddSetToRenderer(this->mGe);
	bool IsClicked = false;

	CursorControl cc;
	cc.SetVisibility(false);

	float dt;
	float updateMouse = 50;
	this->mGe->GetKeyListener()->SetMousePosition(D3DXVECTOR2((float)this->mGe->GetEngineParameters().windowWidth / 2, (float)this->mGe->GetEngineParameters().windowHeight / 3));
	GUIEvent *returnEvent = NULL;
	bool menuChange = false;
	bool mousePressed = false;
	bool isRunning = true;
	float menuChangeTime = 0;
	while(isRunning)
	{
		
		returnEvent = NULL;
		dt = this->mGe->Update();
		IsClicked = this->mGe->GetKeyListener()->IsClicked(1);

		if(this->mCurrentSet == MAINMENU)
		{
			this->KeyBoardSteering(IsClicked);

			CursorControl cc;
			cc.SetVisibility(false);

			/*Mouse Update*/
			if(updateMouse < 0)
			{
				if(GetForegroundWindow() == this->mGe->GetWindowHandle())
					this->UpdateMousePosition();
				
				updateMouse = 50;
			}
			else{ updateMouse -= dt; }
		}
		else if(this->mCurrentSet == OPTIONS_GAMEPLAY)
		{
			CursorControl cc;
			cc.SetVisibility(true);
		}

		/*If mouse is clicked*/
		if(IsClicked && !mousePressed)
		{
			mousePressed = true;
		}

		if(!menuChange)
		{
			D3DXVECTOR2 mousePos = this->mGe->GetKeyListener()->GetMousePosition();
			returnEvent = this->mSets[this->mCurrentSet].UpdateAndCheckCollision(mousePos.x, mousePos.y, IsClicked, this->mGe);

			if(returnEvent != NULL)
			{
				if(returnEvent->GetEventMessage() == "ChangeSetEvent")
				{
					ChangeSetEvent* tempReturnEvent = (ChangeSetEvent*)returnEvent;
					int tempEventSet = NULL;
					tempReturnEvent->GetSet(tempEventSet);
					if(tempEventSet == PLAY)
					{
						CursorControl cc;
						cc.SetVisibility(true);

						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mGm = new GameManager(this->mGe);
						this->mGm->PlayLAN("", NONE); //Change "" to "the servers ip" if you want to connect to a server.  (if you want to be host leave it blank)
						//this->mGm->Play(2);
						SAFE_DELETE(this->mGm);
						this->mCurrentSet = MAINMENU;
						this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
					}
					else if(tempEventSet == EXIT)
					{
						SAFE_DELETE(this->mGm);
						isRunning = false;
						return true;
					}
					else if(tempEventSet == OPTIONS_GAMEPLAY)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mCurrentSet = OPTIONS_GAMEPLAY;
						this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
						menuChange = true;
						menuChangeTime = 50;
					}
					else if(tempEventSet == MAINMENU)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mCurrentSet = MAINMENU;
						this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
						menuChange = true;
						menuChangeTime = 50;
					}
				}
				else if(returnEvent->GetEventMessage() == "ChangeResEvent")
				{
					ChangeResEvent* tempReturnEvent = (ChangeResEvent*)returnEvent;
					int width = 0, height = 0;
					width = tempReturnEvent->GetWidth();
					height = tempReturnEvent->GetHeight();
					/*
					Make something that change res here
					*/
				}
			}
		}
		/*If mouse is not clicked*/
		if(!IsClicked && mousePressed)
		{
			mousePressed = false;
		}
		else
		{
			menuChangeTime = menuChangeTime - dt;
			
			if(menuChangeTime < 0)
			{
				menuChange = false;
			}
		}
	}
	
	return true;
}

void MainMenu::KeyBoardSteering(bool& IsClicked)
{

	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	if(this->mGe->GetKeyListener()->IsPressed(VK_RETURN))
		IsClicked = true;

	if(this->mGe->GetKeyListener()->IsPressed(VK_UP) || this->mGe->GetKeyListener()->IsPressed('W'))
		this->mGe->GetKeyListener()->SetMousePosition(D3DXVECTOR2( (windowWidth/2), (windowHeight/4)));
	else if(this->mGe->GetKeyListener()->IsPressed(VK_LEFT) || this->mGe->GetKeyListener()->IsPressed('A'))
		this->mGe->GetKeyListener()->SetMousePosition(D3DXVECTOR2( (windowWidth/4), (windowHeight/2)));
	else if(this->mGe->GetKeyListener()->IsPressed(VK_DOWN) || this->mGe->GetKeyListener()->IsPressed('S'))
		this->mGe->GetKeyListener()->SetMousePosition(D3DXVECTOR2( (windowWidth/2), (windowHeight/4)*3));
	else if(this->mGe->GetKeyListener()->IsPressed(VK_RIGHT) || this->mGe->GetKeyListener()->IsPressed('D'))
		this->mGe->GetKeyListener()->SetMousePosition(D3DXVECTOR2( (windowWidth/4)*3, (windowHeight/2)));
}