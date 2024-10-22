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
#include "..\GUI\CheckBox.h"


class Maze : public GameMode
{
	private:
			float mTargetX;
			float mTargetZ;
			bool mRunning;
			float mWindowWidth;
			float mWindowHeight;
			float mCreditSpeed;
			float mDiff;
			float mDelayTimer;
			Map* mBox;
			void PlayMazeV1();
			void PlayMazeV2();
			CheckBox* mCb;

	public:
			Maze();
			Maze(GraphicsEngine* ge);
			virtual ~Maze();
			virtual void Initialize();
			virtual void Intro();
			virtual bool PlaySpecific();
			virtual void ShowStats();
			virtual void ShowHud();
			virtual void HideHud();
			virtual bool checkWinConditions(float dt);
			bool checkRespownConditions();
			void ResetMaze();
};