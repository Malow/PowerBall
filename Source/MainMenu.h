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
#include "GameHandler.h"
#include "GraphicsEngine.h"
#include "stdafx.h"
#include "Ball.h"
#include "Platform.h"

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
	OPTIONS,
	OPTIONS_SOUND,
	OPTIONS_GRAPHICS, 
	CREDIT,
	EXIT};

class MainMenu
{
private:
	int mNrOfSets;
	int mCurrentSet;
	int mSubSet;
	bool mRunning;

	SoundSong* mSong;
	GraphicsEngine* mGe;
	GameManager* mGm;
	GameHandler* mGh;
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
	void CreateScene();
	void DeleteScene();

	/*Needed for the Scene*/
	StaticMesh* mBall1;
	StaticMesh* mBall2;
	StaticMesh* mPlatform;
	CamRecording* mCamRec;
	Light* mLight1;
	Light* mLight2;
public:
	/*! This function will create the main menu and init all buttons etc.*/
	MainMenu(GraphicsEngine* ge);
	virtual ~MainMenu();

	/*! This function will start to use and draw the main menu*/
	bool Run();
};