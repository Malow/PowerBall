#pragma once
/**
* Authors: Jerry Rahmqvist
* Network: Didrik Axelsson
* Create Date: 09/05/2012
* 
* This Class is responible for all things in the King of the Hill game 
* from start to end.
*
**/

#include "..\Game Modes\GameMode.h"
#include "..\Network\GameNetwork.h"
#include "ChooseTeamMenu.h"


class KingOfTheHill : public GameMode
{
	private:
		ChooseTeamMenu* mChooseTeamMenu;

	public:
			KingOfTheHill();
			KingOfTheHill(GraphicsEngine* ge, GameNetwork* net, ServerInfo server);
			virtual ~KingOfTheHill();
			virtual void Initialize();
			virtual void Intro();
			virtual void Play();
			void PlayRound(bool& roundsLeft, bool& zoomInPressed, bool& zoomOutPressed);
			virtual void ShowStats();
			virtual bool checkWinConditions(float dt);
			void AddBall();

	
};