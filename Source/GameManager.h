#pragma once

/* 
Gets control of the main thread from MainMenu after play is initiated. 
Takes care of playing a single game and upon the end of the game returns the thread back to MainMenu.
*/

#include "stdafx.h"

class GameManager
{
private:

public:
	GameManager();
	virtual ~GameManager();

	/*! Initiates a game and runs it untill it's over, after which it will return. Parameters are balls etc */
	bool Play();
};