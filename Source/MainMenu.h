#pragma once

/*
Main menu of the game, gets the thread from main() function in the Run() function.
Takes care of the game untill either Play is pressed and then GameManager takes over or exit is pressed and it returns to main.
*/

#include "GameManager.h"
#include "stdafx.h"

class MainMenu
{
private:

public:
	MainMenu();
	virtual ~MainMenu();

	/*! Creates a menu and lets the user choose between exiting, where it returns, playing, where the GameManager's play is called, and Options where the user can change options. */
	void Run();

};