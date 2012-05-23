#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 09/05/2012
* 
* This Class is responible for all things in the Maze II game 
* from start to end.
*
**/

#include "..\Game Modes\GameMode.h"


class Maze2 : public GameMode
{
	private:
			Map* mBox;
			Text* mText;
	public:
			Maze2();
			Maze2(GraphicsEngine* ge);
			virtual ~Maze2();
			virtual void Initialize();
			virtual void Intro();
			virtual bool PlaySpecific();
			virtual void ShowStats();
			virtual bool checkWinConditions(float dt);
			virtual void ShowHud();
			bool checkRespownConditions();
			void ResetMaze();
};