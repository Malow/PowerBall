#pragma once
/**
* Author: Rikard Johansson
* Create Date: 05/04/2012
* 
* This class is used when you are ingame and press a button to get to the menu ingame.
**/
#include "GUI\GUISet.h"
#include "GUI\GUIEvent.h"
#include "GUI\NoEvent.h"
#include "GUI\ChangeResEvent.h"
#include "GraphicsEngine.h"
#include "stdafx.h"

enum INGAMEMENU {IGNBACKGROUND,
	INGAMEMENU,
	IGNRESUME,
	IGNOPTIONS,
	IGNQUIT};

class InGameMenu
{
private:
	int mNrOfSets;
	int mCurrentSet;
	bool mIsRunning;

	GraphicsEngine* mGe;
	GUISet* mSets;

	bool Initialize();
public:
	InGameMenu();
	InGameMenu(GraphicsEngine* ge);
	virtual ~InGameMenu();

	/*! This function will start to draw the in game menu and will go back when Resume is clicked*/
	bool Run();

};