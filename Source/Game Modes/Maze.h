#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 09/05/2012
* 
* This Class is responible for all things in the Maze game 
* from start to end.
*
**/

#include "..\Game Modes\GameMode.h"


class Maze : public GameMode
{
	private:

	public:
			Maze();
			Maze(GraphicsEngine* ge);
			virtual ~Maze();
			virtual void Initialize();
			virtual void Intro();
			virtual void Play();
			virtual void ShowStats();
			virtual bool checkWinConditions(float dt);
};