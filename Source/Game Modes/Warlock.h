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

class Warlock : public GameMode
{
	private:
			Text* hudR1;
			Text* hudR2;
			Text* hudR3;
			Text* hudR4;
			Text* hudR5;
			Text* hudR6;
			Text* hudR7;
			Text* hudR8;
			Text* hudR9;
			Text* hudR10;
			Text* hudR11;
			Text* hudR12;
			Text* hudR13;
			Text* hudR14;
			Text* hudR15;
			Text* hudR16;
	public:
			Warlock();
			Warlock(GraphicsEngine* ge, GameNetwork* net, ServerInfo server);
			virtual ~Warlock();
			virtual void Initialize();
			virtual void Intro();
			virtual void Play();
			virtual void ShowStats();
			virtual bool checkWinConditions(float dt);
			void ShowHud();
			void AddBall();

	
};