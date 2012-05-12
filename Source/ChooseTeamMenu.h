#pragma once
/**
* Author: Rikard Johansson
* Create Date: 05/04/2012
* 
* This class is used when you are ingame and press a button to get to the menu ingame.
**/
#include "GUI\GUISet.h"
#include "GUI\SelectTeamEvent.h"
#include "stdafx.h"
#include "GameOptions.h"

enum INGAMEMENU {CTMENU,
	BLUE_TEAM,
	RED_TEAM,
};

class ChooseTeamMenu
{
private:
	int mNrOfSets;
	int mCurrentSet;

	bool mIsRunning;

	GraphicsEngine* mGe;
	GUISet* mSets;

	bool Initialize();
public:
	ChooseTeamMenu();
	ChooseTeamMenu(GraphicsEngine* ge);
	virtual ~ChooseTeamMenu();

	/*! This function will start to draw the in game menu and will go back when you have picked a team is clicked*/
	int Run();

};