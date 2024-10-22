#include "MainMenu.h"

bool GameOptions::isPlaying = false;
SoundSong* GameOptions::songPlaying = NULL;
SoundEffect* GameOptions::mouseClick = NULL;
float GameOptions::masterVolume = 0.05f;
float GameOptions::songVolume = 0.05f;
float GameOptions::effectVolume = 0.05f;

MainMenu::MainMenu(GraphicsEngine* ge)
{
	this->mNrOfSets = 30;
	this->mSets = new GUISet[this->mNrOfSets]();
	this->mRunning = true;
	this->mRestart = false;
	this->mNeedRestart = false;
	this->mCurrentSet = MAINMENU;
	this->mSubSet = NOMENU;

	this->mGe = ge;
	this->mGh = new GameHandler(ge);

	SoundEngine* tempSoundEngine = GetGraphicsEngine()->GetSoundEngine();
	GameOptions gameParams;
	tempSoundEngine->SetMasterVolume(gameParams.masterVolume);
	//tempSoundEngine->SetSongVolume(gameParams.songVolume);
	//tempSoundEngine->SetSoundEffectVolume(gameParams.effectVolume);
	
	int i = 0; //LOL TROLL TEST IF I CAN PUSH
	
	gameParams.mouseClick = tempSoundEngine->LoadSoundEffect("Media/Sounds/SoundEffects/Mouse_Click_Menu3FIXED.flac", false);

	gameParams.songPlaying = tempSoundEngine->LoadSong("Media/Sounds/Songs/George_Ellinas_-_Pulse_(George_Ellinas_remix).mp3", true);
	this->Initialize();
}
MainMenu::~MainMenu()
{
	if(this->mSets)
		delete [] this->mSets;

	SAFE_DELETE(this->mCamRec);

	SAFE_DELETE(this->mGh);
}
bool MainMenu::Initialize()
{
	this->CreateCircleMenu();

	this->CreateOptionsMenu();
	
	this->CreateHotseatMenu();
	
	this->CreateOnlineAndLanMenu();

	this->CreateScene();
	
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
	mGe->LoadingScreen("Media/LoadingScreen/StartScreen.png", "", 0.0f, 1.0f, 1.0f, 1.0f);

	GameOptions::songPlaying->Play();
	if(GameOptions::isPlaying == false)
	{
		GameOptions::songPlaying->Mute();
	}

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
		if(this->mCamRec != NULL)
			this->mCamRec->Update(dt);

		IsClicked = this->mGe->GetKeyListener()->IsClicked(1);

		if(this->mCurrentSet == MAINMENU || this->mCurrentSet == MAINMENU_PLAY)
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
		else if(this->mCurrentSet == OPTIONS || this->mCurrentSet == OPTIONS_HOTSEAT || this->mCurrentSet == OPTIONS_ONLINE
			|| this->mCurrentSet == OPTIONS_LAN)
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
			if(returnEvent == NULL && this->mSubSet != NOMENU)
				returnEvent = this->mSets[this->mSubSet].UpdateAndCheckCollision(mousePos.x, mousePos.y, IsClicked, this->mGe);
			if(returnEvent != NULL)
			{
				ChangeSetEvent* tempReturnEvent1 = (ChangeSetEvent*)returnEvent;
				if(returnEvent->GetEventMessage() == "ChangeSetEvent")
				{
					ChangeSetEvent* tempReturnEvent = (ChangeSetEvent*)returnEvent;
					int tempEventSet = tempReturnEvent->GetSet();
					if(tempEventSet == PLAY_LAN)
					{
						CursorControl cc;
						cc.SetVisibility(true);
						int lifes = -1;
						int rounds = -1; 
						int flags = -1;
						int continuously = -1;
						int accumulated = -1;
						int teams = 0;
						if(this->mSets[this->mCurrentSet].GetCheckBox("TEAM")->GetOn())
							teams = 2;

						MaloW::Debug(teams);

						string serverName = "PowerBall server";

						/* Getting some needed info to start a server*/
						GUIEvent* tempEvent = this->mSets[this->mCurrentSet].GetEventFromDropDown("GameMode");
						ChangeSubSetEvent* GameMode = (ChangeSubSetEvent*) tempEvent;
						GameModeInfo* gmi;
						if(GameMode->GetGameMode() == CTF)
						{
							flags = atoi(this->mSets[this->mSubSet].GetTextFromField("Flags").c_str());
							gmi = new CTFInfo(flags, 2);
						}
						else if(GameMode->GetGameMode() == KOTH)
						{
							continuously = atoi(this->mSets[this->mSubSet].GetTextFromField("Continuously").c_str());
							accumulated = atoi(this->mSets[this->mSubSet].GetTextFromField("Accumulated").c_str());
							rounds = atoi(this->mSets[this->mSubSet].GetTextFromField("Rounds").c_str());
							gmi = new KOTHInfo(rounds, 2, continuously, accumulated);
						}
						else if(GameMode->GetGameMode() == WARLOCK)
						{
							rounds = atoi(this->mSets[this->mSubSet].GetTextFromField("Rounds").c_str());
							gmi = new WARLOCKInfo(rounds, 2, 1234.56f); //replace 1234.56 with atoi(this->mSets[this->mSubSet].GetTextFromField("Health").c_str());
						}
						serverName  = this->mSets[this->mSubSet].GetTextFromField("ServerName");
						this->DeleteScene();
						//this->mGm = new GameManager(this->mGe);
						
						//add button called find servers or something and copy the row below to retrieve a list of all servers currently open on the LAN
						vector<ServerInfo> servers = this->mGh->GetLanPointer()->FindServers();
						int chosenServer = 0; //index of the server selected from the list

						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						if(servers.size() > 0)
						{
							//this->mGm->PlayLAN(servers[chosenServer]);
							ServerInfo tett = this->mGh->GetLanPointer()->ConnectTo(servers[chosenServer].GetIP()); // replace servers[chosenServer].GetIP() with the IP u wanna connect to
							if(tett.GetIP() != "") //"83.233.57.248");//
							{
								this->mGh->CreateGame(tett.GetGameMode(), tett);//servers[chosenServer].GetGameMode(), servers[chosenServer]);
								this->mGh->Start();
							}
							else cout << "SERVER NOT FOUND";
						}
						else //atm, will host if no servers running on LAN
						{
							ServerInfo host(serverName , 0, 5, GameMode->GetGameMode(), "", 10000, gmi);
							this->mGh->GetLanPointer()->Start(host);
							this->mGh->CreateGame(GameMode->GetGameMode(), host);
							this->mGh->Start();

							//this->mGm->PlayLAN(host);
						} 
						/*
						ServerInfo tett = this->mGh->GetLanPointer()->ConnectTo("88.129.204.14");//servers[chosenServer].GetIP()); // replace servers[chosenServer].GetIP() with the IP u wanna connect to
						   if(tett.GetIP() != "")
						   {
							this->mGh->CreateGame(tett.GetGameMode(), tett);//servers[chosenServer].GetGameMode(), servers[chosenServer]);
							this->mGh->Start();
						   }
						   */
						this->CreateScene();
						mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");

						this->mSubSet = NOMENU;
						this->mCurrentSet = MAINMENU;
						
						//SAFE_DELETE(this->mGm);
					}
					if(tempEventSet == PLAY_ONLINE)
					{
						/*CursorControl cc;
						cc.SetVisibility(true);
						this->mGm = new GameManager(this->mGe);
						
						//this->mGm->PlayONLINE("", CTF); //Change "" to "the servers ip" if you want to connect to a server.  (if you want to be host leave it blank)
						this->mGm->Play(2);
						
						SAFE_DELETE(this->mGm);*/
						
						CursorControl cc;
						cc.SetVisibility(true);
						//this->mGm = new GameManager(this->mGe);

						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);

						this->DeleteScene();

						//this->mGm->PlayCredits();
						this->mGh->CreateMazeGame2();
						this->mGh->Start();

						this->CreateScene();
						mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");

						this->mSubSet = NOMENU;
						this->mCurrentSet = MAINMENU;

						//SAFE_DELETE(this->mGm);						
					}
					if(tempEventSet == MAINMENU_PLAY)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						this->mSubSet = NOMENU;
						this->mCurrentSet = MAINMENU_PLAY;
					}
					if(tempEventSet == PLAY_HOTSEAT)
					{
						this->mSubSet = NOMENU;
						CursorControl cc;
						cc.SetVisibility(true);
						string lifes = this->mSets[this->mCurrentSet].GetTextFromField("Lifes");
						string rounds = this->mSets[this->mCurrentSet].GetTextFromField("Rounds");

						//this->mGm = new GameManager(this->mGe);

						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);

						this->DeleteScene();
						
						this->mGh->CreateKnockoutGame(2, atoi(rounds.c_str()));
						this->mGh->Start();
						//this->mGm->Play(2, atoi(lifes.c_str()), atoi(rounds.c_str()));

						this->CreateScene();
						mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");

						this->mSubSet = NOMENU;
						this->mCurrentSet = MAINMENU;

						//SAFE_DELETE(this->mGm);
						
					}
					else if(tempEventSet == EXIT)
					{
						this->mSubSet = NOMENU;
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						//SAFE_DELETE(this->mGm);
						this->mGh->DeleteCreatedGame();
						isRunning = false;
						return true;
					}
					else if(tempEventSet == OPTIONS)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						this->mSubSet = OPTIONS_GRAPHICS;
						this->mCurrentSet = OPTIONS;
					}
					else if(tempEventSet == MAINMENU)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						this->mSubSet = NOMENU;
						this->mCurrentSet = MAINMENU;
					}
					else if(tempEventSet == OPTIONS_HOTSEAT)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						this->mSubSet = NOMENU;
						this->mCurrentSet = OPTIONS_HOTSEAT;
					}
					else if(tempEventSet == OPTIONS_ONLINE)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						this->mSubSet = NOMENU;
						this->mCurrentSet = OPTIONS_ONLINE;
					}
					else if(tempEventSet == CREDIT)
					{

						CursorControl cc;
						cc.SetVisibility(true);
						//this->mGm = new GameManager(this->mGe);

						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);

						this->DeleteScene();

						bool temp = false;
						//while(!temp)
							//temp = this->mGm->PlayCredits2();

						this->mGh->CreateMazeGame();
						this->mGh->Start();

						this->CreateScene();
						mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");

						this->mSubSet = NOMENU;
						this->mCurrentSet = MAINMENU;

						//SAFE_DELETE(this->mGm);
						/*
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						this->mGm = new GameManager(this->mGe);

						mGm->PlayCredits2(); 

						this->mSubSet = NOMENU;
						this->mCurrentSet = MAINMENU;
						*/
					}
					else if(tempEventSet == OPTIONS_LAN)
					{
						this->mSets[this->mCurrentSet].RemoveSetFromRenderer(this->mGe);
						this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);
						this->mCurrentSet = OPTIONS_LAN;
						GUIEvent* temp = this->mSets[this->mCurrentSet].GetEventFromDropDown("GameMode");
						this->mSubSet = ((ChangeSubSetEvent*)temp)->GetSet();
						

						float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
						float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
						float dx = (windowHeight * 4.0f) / 3.0f;
						float offSet = (windowWidth - dx) / 2.0f;

						//this->mGm = new GameManager(this->mGe);
						//add button called find servers or something and copy the row below to retrieve a list of all servers currently open on the LAN
						/*vector<ServerInfo> servers = this->mGh->GetLanPointer()->FindServers();
						Element* tempElement;
						for(int i = 0; i < servers.size(); i++)
						{
							tempElement = new TextBox(dx * (680.0f / 1200.0f), windowHeight * (280.0f / 900.0f) + 40 * i, 1, "Media/Menus/EmptyMenu.png", 0, 0, servers.at(i).GetServerName(), "Server" + MaloW::convertNrToString(i), 0.80, servers.at(i).GetServerName().size());
							this->mSets[this->mSubSet].AddElement(tempElement);
						}
						tempElement = NULL;*/
						//SAFE_DELETE(this->mGm);
					}

					this->mSets[this->mCurrentSet].AddSetToRenderer(this->mGe);
					this->mSets[this->mSubSet].AddSetToRenderer(this->mGe);
					menuChange = true;
					menuChangeTime = 50;


					if(this->mSubSet == OPTIONS_GRAPHICS) // A extraspecial case
					{
						GraphicsEngineParams tempCfg;

						CheckBox* tempCheck = this->mSets[OPTIONS_GRAPHICS].GetCheckBox("Windowed");
						tempCheck->SetChecked(!tempCfg.Maximized);

						string startValue = MaloW::convertNrToString(tempCfg.FXAAQuality);
						TextBox* temp = this->mSets[OPTIONS_GRAPHICS].GetTextBox("FXAA");
						temp->SetText(startValue);

						startValue = MaloW::convertNrToString(tempCfg.ShadowMapSettings);
						temp = this->mSets[OPTIONS_GRAPHICS].GetTextBox("SHADOW");
						temp->SetText(startValue);
					}
				}
				else if(returnEvent->GetEventMessage() == "ChangeOptionEvent")
				{
					ChangeOptionEvent* tempReturnEvent = (ChangeOptionEvent*)returnEvent;
					/*if(tempReturnEvent->GetOption() == "Sound")
					{
						if(tempReturnEvent->GetValue() == "true")
						{
							GameOptions::songPlaying->Unmute();
							GameOptions::isPlaying = true;
						}
						else
						{
							GameOptions::songPlaying->Mute();
							GameOptions::isPlaying = false;
						}
					}*/
					if(tempReturnEvent->GetOption() == "Apply")
					{
						GraphicsEngineParams gep = GetGraphicsEngine()->GetEngineParameters();
						TextBox* FXAA = this->mSets[this->mSubSet].GetTextBox("FXAA");
						gep.FXAAQuality = atoi(FXAA->GetText().c_str());

						GUIEvent* tempEvent = this->mSets[this->mSubSet].GetEventFromDropDown("Resolution");
						float windowWidth = ((ChangeResEvent*)tempEvent)->GetWidth();
						float windowHeight = ((ChangeResEvent*)tempEvent)->GetHeight();

						CheckBox* tempCheck = this->mSets[OPTIONS_GRAPHICS].GetCheckBox("Windowed");
						if(gep.Maximized != tempCheck->GetOn())
						{
							this->mNeedRestart = true;
						}

						if(gep.windowWidth != windowWidth || gep.windowHeight != windowHeight)
						{
							this->mNeedRestart = true;
						}

						TextBox* Shadow = this->mSets[this->mSubSet].GetTextBox("SHADOW");
						if(gep.ShadowMapSettings != atoi(Shadow->GetText().c_str()))
						{
							this->mNeedRestart = true;
						}

						if(tempReturnEvent->GetValue() == "true")
						{
							this->mRestart = true;
						}
						GraphicsEngineParams params;
						params.SaveToFile("config.cfg", windowWidth, windowHeight, !tempCheck->GetOn(), atoi(Shadow->GetText().c_str()), atoi(FXAA->GetText().c_str()));
					}
					else if(tempReturnEvent->GetOption() == "ApplySound")
					{
						GameOptions go;
						CheckBox* tempCheck = this->mSets[this->mSubSet].GetCheckBox("Sound");

						go.isPlaying = tempCheck->GetOn();
						if(tempCheck->GetOn())
						{
							GameOptions::songPlaying->Unmute();
							GameOptions::isPlaying = true;
						}
						else
						{
							GameOptions::songPlaying->Mute();
							GameOptions::isPlaying = false;
						}

						TextBox* tempText = this->mSets[this->mSubSet].GetTextBox("MASTERVOLUME");

						float volume = atoi(tempText->GetText().c_str());
						GameOptions::masterVolume = volume/100;
						GetGraphicsEngine()->GetSoundEngine()->SetMasterVolume(volume/100);
						//
						// Do last
						go.SaveToFile("GameSettings.cfg");
					}
				}
				else if(returnEvent->GetEventMessage() == "ServerListEvent")
				{
					ServerListEvent* temp = (ServerListEvent*)returnEvent;
					if(temp->GetEventMessage() == "Refresh")
					{
						float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
						float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
						float dx = (windowHeight * 4.0f) / 3.0f;
						float offSet = (windowWidth - dx) / 2.0f;
						float x = 50, y = 400;

						vector<ServerInfo> servers = this->mGh->GetLanPointer()->FindServers();
						Element* tempElement;
						for(int i = 0; i < servers.size(); i++)
						{
							tempElement = new TextButton(x, y, 0, "Media/Menus/textbutton.png",dx * (550.0f / 1200.0f),windowHeight * (50.0f / 900.0f),
								servers.at(i).GetServerName() + "\t\t" + MaloW::convertNrToString(servers.at(i).GetNumPlayers()) + "/" + MaloW::convertNrToString(servers.at(i).GetMaxNumPlayers()),
								new NoEvent());
							this->mSets[SERVERLIST].AddElement(tempElement);
							x += 60;
						}
						this->mSets[SERVERLIST].AddSetToRenderer(this->mGe);

					}
				}
				if(returnEvent->GetEventMessage() == "ChangeSubSetEvent")
				{
					this->mSets[this->mSubSet].RemoveSetFromRenderer(this->mGe);

					ChangeSubSetEvent* tempReturnEvent = (ChangeSubSetEvent*)returnEvent;
					int tempEventSet = tempReturnEvent->GetSet();

					if(tempEventSet == OPTIONS_SOUND)
					{
						CheckBox* temp = this->mSets[OPTIONS_SOUND].GetCheckBox("Sound");
						temp->SetChecked(GameOptions::isPlaying);
					}
					if(tempEventSet == OPTIONS_GRAPHICS)
					{
						GraphicsEngineParams gep = GetGraphicsEngine()->GetEngineParameters();
						string startValue = MaloW::convertNrToString(gep.ShadowMapSettings);

						TextBox* temp = this->mSets[OPTIONS_GRAPHICS].GetTextBox("FXAA");
						temp->SetText(startValue);

						startValue = MaloW::convertNrToString(gep.FXAAQuality);
						temp = this->mSets[OPTIONS_GRAPHICS].GetTextBox("SHADOW");
						temp->SetText(startValue);
					}

					int set = tempReturnEvent->GetSet();
					this->mSubSet = set;
					this->mSets[this->mSubSet].AddSetToRenderer(this->mGe);
					menuChange = true;
					menuChangeTime = 50;
				}
			}
		}
		/*If mouse is not clicked*/
		if(!IsClicked && mousePressed)
		{
			mousePressed = false;
		}
		if(!this->mGe->isRunning())
			isRunning = false;
		else
		{
			menuChangeTime = menuChangeTime - dt;
			
			if(menuChangeTime < 0)
			{
				menuChange = false;
			}
		}
		if(this->mRestart)
		{
			if(this->mNeedRestart)
			{
				SystemReqRestart* srr =  new SystemReqRestart(this->mGe);
				srr->Run();
				delete srr;
				this->mNeedRestart = false;
				this->mRestart = false;
				//return true;
			}
		}
	}
	
	return false;
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