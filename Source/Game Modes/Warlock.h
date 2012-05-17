#pragma once
/**
* Authors: Jerry Rahmqvist
* Network: Didrik Axelsson
* Create Date: 09/05/2012
* 
* This Class is responible for all things in the Warlock game 
* from start to end.
*
**/

#include "..\Game Modes\GameMode.h"
#include "..\Network\GameNetwork.h"
#include "..\Spells\Spell.h"
#include "..\Spells\ChargeSpell.h"
#include "..\Spells\SprintSpell.h"
#include "..\Spells\HardenSpell.h"
#include "..\Spells\InvisibilitySpell.h"
#include "..\Spells\JumpSpell.h"
#include "..\GUI\ProgressBar.h"

class Warlock : public GameMode
{
	private:
			ProgressBar** mProgressBars;

	public:
			Warlock();
			Warlock(GraphicsEngine* ge, GameNetwork* net, ServerInfo server);
			virtual ~Warlock();
			virtual void Initialize();
			virtual void Intro();
			virtual void PlaySpecific();
			virtual void ShowStats();
			virtual void ShowHud();
			virtual bool checkWinConditions(float dt);
			virtual void AddBall();

	
};