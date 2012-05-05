#pragma once
/**
* Author: Rikard Johansson
* Create Date: 28/03/2012
*
* This class handels the main menu of the game
* It starts the "game" and its the menu you get back to
* when you finish a game.
**/

//Includes
#include "GUI\GUISet.h"
#include "GUI\GUIEvent.h"
#include "GUI\NoEvent.h"
#include "GUI\ChangeResEvent.h"
#include "GUI\ChangeSetEvent.h"
#include "GUI\ChangeSubSetEvent.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "stdafx.h"

enum MENU {NOMENU,
	BACKGROUND,
	MAINMENU,
	MAINMENU_PLAY,
	PLAY_ONLINE,
	PLAY_HOTSEAT,
	PLAY_LAN,
	OPTIONS_HOTSEAT,
	OPTIONS_ONLINE,
	OPTIONS_LAN,
	OPTIONS_LAN_SUBCTF,
	OPTIONS_LAN_SUBKOTH,
	OPTIONS_LAN_SUBWARLOCK,
	OPTIONS_GAMEPLAY, 
	CREDIT,
	EXIT};

class MainMenu
{
private:
	int mNrOfSets;
	int mCurrentSet;
	int mSubSet;
	bool mRunning;

	GraphicsEngine* mGe;
	GameManager* mGm;
	GUISet* mSets;

	/*! Creates the sets etc, has to be called before run.*/
	bool Initialize();

	/*! Update mouse position*/
	void UpdateMousePosition();

	void KeyBoardSteering(bool& IsClicked);

	void CreateCircleMenu();
	void CreateOptionsMenu();
	void CreateHotseatMenu();
	void CreateOnlineAndLanMenu();

public:
	/*! This function will create the main menu and init all buttons etc.*/
	MainMenu(GraphicsEngine* ge);
	virtual ~MainMenu();

	/*! This function will start to use and draw the main menu*/
	bool Run();
};