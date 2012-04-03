#include "stdafx.h"
#include "MainMenu.h"
#include "GraphicsEngine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	MaloW::ClearDebug();
	// Create parameters for the graphics engine
	GraphicsEngineParams params;
	params.windowHeight = 900;
	params.windowWidth = 1500;
	params.CamType = FPS;

	// Create the graphics engine
	GraphicsEngine* ge = new GraphicsEngine(params, hInstance, nCmdShow);


	/*
	// Example of GE useage
	Mesh* testBall = ge->CreateMesh("sun.obj", D3DXVECTOR3(10, 10, 10));
	Image* testImg = ge->CreateImage(D3DXVECTOR2(-0.9f, -0.9f), D3DXVECTOR2(0.2f, 0.2f), "sun.png");
	Light* testLight = ge->CreateLight(D3DXVECTOR3(10, 20, 10));
	while(ge->isRunning())	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		float diff = ge->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.
		if(ge->GetKeyListener()->IsPressed('W'))
			ge->GetCamera()->moveForward(diff);
	}
	*/


	// Create the MainMenu and send the graphics engine, and then run Run();
	MainMenu mm(ge);
	mm.Run();


	// Delete graphics engine
	delete ge;
	return 0;
}