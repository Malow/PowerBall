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
#include "InGameMenu.h"
#include "Flag.h"
#include "Spells\Spell.h"
#include "Spells\ChargeSpell.h"
#include "Spells\SprintSpell.h"
#include "Spells\HardenSpell.h"
#include "Spells\InvisibilitySpell.h"



class GameManager
{
private:
	int				mNumPlayers;
	int				mRounds;
	float           mMaxTime;
	Platform*		mPlatform;
	Ball**			mBalls; 
	GameNetwork*	mNet; 
	GraphicsEngine* mGe;
	InGameMenu*		mIGM;
	Light*			mLights[5];
	int				mGameMode;
	Flag*			mEnemyFlag;
	Flag*			mFriendlyFlag;
	float counter;
public:
	//constructors and destructors
				GameManager(GraphicsEngine* ge);
	virtual		~GameManager();

	/*! Starts the game with the assigned amount of players. */
	bool		Play(const int numPlayers, int lifes, int rounds);

	/*! Starts LAN game with the assigned amount of players. */
	bool		PlayLAN(ServerInfo server);

	/*! Retrieves pointer to the game network class. */
	GameNetwork* GetLanPointer() { return this->mNet; }

private:

	/*! Initializes the game. (First function-call in Play) */
	void		Initialize();

	bool		CaptureTheFlag();

	bool		KingOfTheHill(float dt);

	bool		WarLock(float dt, float& warlockTimer);

	void		AddBall();

	void		AddBallWarlock();

	void		InputKnockout(float diff, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& roundsLeft, bool& quitByMenu);



	void		ClientKeyPress(float diff, const int index, char key); 

	void		InputKeysPressedSelf(float diff, int index, bool& zoomOutPressed, bool& zoomInPressed, bool& running, bool& quitByMenu);


	void		SendKeyInputs(const int clientIndex, float diff);
	void		HandleClientKeyInputs(const int clientIndex, float diff);
};