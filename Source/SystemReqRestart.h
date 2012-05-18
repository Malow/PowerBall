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

enum SYSRESART {SRRMENU,
};

class SystemReqRestart
{
private:
	int mNrOfSets;
	int mCurrentSet;

	bool mIsRunning;

	GraphicsEngine* mGe;
	GUISet* mSets;

	bool Initialize();
public:
	SystemReqRestart();
	SystemReqRestart(GraphicsEngine* ge);
	virtual ~SystemReqRestart();

	/*! This function will start to draw the in game menu and will go back when you have picked a team is clicked*/
	int Run();

};