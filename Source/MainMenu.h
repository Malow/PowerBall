#pragma once
/**
* Auther: Rikard Johansson
* Create Date: 28/03/2012
*
* This class handels the main menu of the game
* It starts the "game" and its the menu you get back to
* when you finish a game.
**/

//Includes
#include "GUI\GUISet.h"
#include "GUI\Event.h"
#include "GameManager.h"
#include "GraphicsEngine.h"
#include "stdafx.h"
#include "GUI\SimpleButton.h"
#include "GUI\GUIPicture.h"

class MainMenu
{
private:
	int mNrOfSets;
	int mCurrentSet;
	bool mRunning;

	GraphicsEngine* mGe;
	GameManager* mGm;
	GUISet* mSets;

	/*! Creates the sets etc, has to be called before run.*/
	bool Initialize();
public:
	/*! This function will create the main menu and init all buttons etc.*/
	MainMenu(GraphicsEngine* ge);
	virtual ~MainMenu();

	/*! This function will start to use and draw the main menu*/
	bool Run();
};