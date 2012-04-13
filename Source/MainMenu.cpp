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
	SAFE_DELETE_ARRAY(this->mSets);
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

	tempElement = new SimpleButton(offSet + dx * (45.0f / 1440), windowHeight * (1000.0f / 1080), 1, "Media/buttonbacktomenu.png", dx * (365.0f / 1440), windowHeight * (70.0f / 1080), new ChangeSetEvent(MAINMENU), "Media/clickbacktomenu.png" , "Media/mouseoverbacktomenu.png", offSet + dx * (45.0f / 1440) + dx * (15.0f / 1440), windowHeight * (1000.0f / 1080) + windowHeight * (12.5f / 1080), dx * (335.0f / 1440), windowHeight * (45.0f / 1080));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet + dx * (50.0f / 1440), windowHeight * (300.0f / 1080), 1, "Media/buttongraphics.png", dx * (200.0f / 1440), windowHeight * (30.0f / 1080), new NoEvent(), "Media/buttongraphics.png", "Media/buttongraphics.png", offSet + dx * (50.0f / 1440), windowHeight * (300.0f / 1080), dx * (200.0f / 1440), windowHeight * (30.0f / 1080));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet + dx * (450.0f / 1440), windowHeight * (30.0f / 1080), 1, "Media/buttonbasic.png", dx * (125.0f / 1440), windowHeight * (30.0f / 1080), new NoEvent(), "Media/buttonbasic.png", "Media/buttonbasic.png", offSet + dx * (450.0f / 1440), windowHeight * (30.0f / 1080), dx * (125.0f / 1440), windowHeight * (30.0f / 1080));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet + dx * (575.0f / 1440), windowHeight * (30.0f / 1080), 1, "Media/buttonadvanced.png", dx * (225.0f / 1440), windowHeight * (30.0f / 1080), new NoEvent(), "Media/buttonadvanced.png", "Media/buttonadvanced.png", offSet + dx * (575.0f / 1440), windowHeight * (30.0f / 1080), dx * (225.0f / 1440), windowHeight * (30.0f / 1080));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	
	tempElement = NULL;
	this->mSets[MAINMENU].AddSetToRenderer(this->mGe);
	
	return true;
}
void MainMenu::UpdateMousePosition()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float lengthFromMiddle = (windowHeight * 0.745f) / 3;
	
	D3DXVECTOR2 centerVector = D3DXVECTOR2(windowWidth / 2, windowHeight / 2);
	D3DXVECTOR2 mouseVector = this->mGe->GetKeyListener()->GetMousePosition();
	D3DXVECTOR2 centerToMouseVec = (centerVector - mouseVector);

	
	if(D3DXVec2Length(&centerToMouseVec) != lengthFromMiddle)
	{
		centerToMouseVec = (lengthFromMiddle / D3DXVec2Length(&centerToMouseVec)) * centerToMouseVec;
		this->mGe->GetKeyListener()->SetMousePosition(centerVector + (centerToMouseVec * (-1)));
	}
	
}

bool MainMenu::Run()
{
	ShowCursor(FALSE);
	float dt;
	float updateMouse = 50;
	this->mGe->GetKeyListener()->SetMousePosition(D3DXVECTOR2((float)this->mGe->GetEngineParameters().windowWidth / 2, (float)this->mGe->GetEngineParameters().windowHeight / 3));
	GUIEvent *returnEvent = NULL;
	bool menuChange = false;
	bool mousePressed = false;
	float menuChangeTime = 0;
	while(this->mGe->isRunning())
	{
		dt = this->mGe->Update();
		
		if(this->mCurrentSet == MAINMENU)
		{
			//this->mGe->GetKeyListener()->ShowCursor(false);
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
			int i = 0; //this->mGe->GetKeyListener()->ShowCursor(true);

		/*If mouse is clicked*/
		if(this->mGe->GetKeyListener()->IsClicked(1) && !mousePressed)
		{
			mousePressed = true;
		}

		if(!menuChange)
		{
			if(this->mCurrentSet == MAINMENU)
			{
				returnEvent = this->mSets[this->mCurrentSet].UpdateButtons(this->mGe, mousePressed);

				if(returnEvent != NULL)
				{
					if(returnEvent->GetEventMessage() == "ChangeSetEvent")
					{
						ChangeSetEvent* tempReturnEvent = (ChangeSetEvent*)returnEvent;
						int tempEventSet = NULL;
						tempReturnEvent->GetSet(tempEventSet);
						if(tempEventSet == PLAY)
						{
							this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
							this->mGm = new GameManager(this->mGe);
							this->mGm->Play(2, false);
							SAFE_DELETE(this->mGm);
							this->mCurrentSet = MAINMENU;
							this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
						}
						if(tempEventSet == EXIT)
						{
							SAFE_DELETE(this->mGm);
							return true;
						}
						if(tempEventSet == OPTIONS_GAMEPLAY)
						{
							this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
							this->mCurrentSet = OPTIONS_GAMEPLAY;
							this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
							menuChange = true;
							menuChangeTime = 50;
						}
					}
				}
			}
			else if(this->mCurrentSet == OPTIONS_GAMEPLAY) 
			{
				D3DXVECTOR2 mousePos;
				mousePos = this->mGe->GetKeyListener()->GetMousePosition();
				returnEvent = this->mSets[this->mCurrentSet].CheckCollision(mousePos.x, mousePos.y, this->mGe->GetKeyListener()->IsClicked(1), this->mGe);

				if(returnEvent != NULL)
				{
					if(returnEvent->GetEventMessage() == "ChangeSetEvent")
					{
						int whatSet;
						ChangeSetEvent* temp = (ChangeSetEvent*)returnEvent;
						temp->GetSet(whatSet);
						if(whatSet == MAINMENU)
						{
							this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
							this->mCurrentSet = MAINMENU;
							this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
							menuChange = true;
							menuChangeTime = 50;
						}
					}
				}
			}
			returnEvent = NULL;
		}
		/*If mouse is not clicked*/
		if(!this->mGe->GetKeyListener()->IsClicked(1) && mousePressed)
		{
			mousePressed = false;
		}
		/*if(this->mGe->GetKeyListener()->IsClicked(1))
		{
			this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
			this->mGm->Play(2);
			delete this->mGm;
			this->mGm = new GameManager(this->mGe);
			this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
		}*/
		/*if(this->mGe->GetKeyListener()->IsClicked(1))
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
		}*/
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