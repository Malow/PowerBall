#pragma once
/**
* Author: Jerry Rahmqvist
* Create Date: 09/05/2012
* 
* This class handles the creation of a game.
* The following game modes are available 
*			
*		* Knockout	-	1 on 1 in Hotseat (2 playing at the same PC)
*		* DM		-	Death Match
*		* CTF		-	Capture the Flag
*		* KOTH		-	King of the Hill
*		* WARLOCK	-	Warlock game mode when a ball can use spells to improve the the properties of the ball.
*		* Maze		-	You control the ball by steering it as usual
*		* Maze2		-   You control the ball by tilting the labyrinth board
*
**/


#include "GraphicsEngine.h"
#include "Network\GameNetwork.h"
class GameMode;

class GameHandler
{
	private:
			/* This is a pointer to an abstract class. You need to instantiate a game mode. Do not try to instantiate this abstract one...  */
			GameMode*		mGameMode;
			GraphicsEngine* mGe;
			GameNetwork*	mNet;
		
	public:
			GameHandler();
			GameHandler(GraphicsEngine* ge);
			~GameHandler();

			/* This start the game. */
			bool Start();

			/*! Create a game*/
			bool CreateGame(int gameMode, ServerInfo server);

			/* Creates a Knockout game for hotseat. */
			bool CreateKnockoutGame(int numberOfPlayers, int numberOfRounds);

			/* Creates a Warlock game for Network. */
			bool CreateWarlockGame(ServerInfo server);

			/* Creates a King of the Hill game for Network. */
			bool CreateKingOfTheHill(ServerInfo server);

			/* Creates a Capture the Flag game for Network. */
			bool CreateCaptureTheFlag(ServerInfo server);

			/* Creates a Maze game. You control the ball by steering it as usual. */
			bool CreateMazeGame();

			/* Creates a Maze2 game. You control the ball by tilting the labyrinth board. */
			bool CreateMazeGame2();
			
			/* Deletes the game. */
			bool DeleteCreatedGame();

			/*! Pointer to the Game Network class. */
			GameNetwork* GetLanPointer();

			/*! Starts the credit gamemode 1. Random tilt */
			//bool PlayCredits();

			/*! Starts the credit gamemode 2. Oldschool you turn the maze. */
			//bool PlayCredits2();

			/*! Starts LAN game with the assigned amount of players. */
			//bool PlayLAN(ServerInfo server);

};