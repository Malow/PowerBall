#pragma once

/* 
Gets control of the main thread from MainMenu after play is initiated. 
Takes care of playing a single game and upon the end of the game returns the thread back to MainMenu.
*/

#include "stdafx.h"
#include "Platform.h"
#include "Ball.h"
#include "GraphicsEngine.h"
#include "GameNetwork.h"

class GameManager
{
private:
	int				mNumPlayers;
	Platform*		mPlatform;
	Ball**			mBalls; //size = mNumPlayers (intialized in Initialize();)
	GameNetwork*	mNet; 
	GraphicsEngine* mGe;
	Light*			mLights[5];
public:
	//constructors and destructors
				GameManager(GraphicsEngine* ge);
	virtual		~GameManager();

	/*! Starts the game with the assigned amount of players. */
	bool		Play(const int numPlayers);

	/*! Starts LAN game with the assigned amount of players. */
	bool		PlayLAN();

private:

	/*! Initializes the game. (First function-call in Play) */
	void		Initialize();

};