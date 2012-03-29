#include "stdafx.h"
#include "MainMenu.h"
#include "GraphicsEngine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	/*
	Create and intitiate the singleton in the graphics engine and have it initiate the window, and also start it's thread so that it starts rendering.

	Initiate the key-listener's singleton
	*/

	MainMenu mm;
	mm.Run();

	/*
	Stop the thread in the graphics engine and wait for it to finish the current frame before returning and closing the program.
	*/
	return 0;
}


/*
Put WndProc() here and send events to keylistener?

*/