#include "stdafx.h"
#include "MainMenu.h"
#include "GraphicsEngine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
#if defined(DEBUG) || defined(_DEBUG)
	myInitMemoryCheck();
#endif

	MaloW::ClearDebug();
	// Create parameters for the graphics engine
	GraphicsEngineParams params;
	params.windowHeight = 900;
	params.windowWidth = 1500;
	params.CamType = FPS;

	// Create the graphics engine
	GraphicsEngine* ge = new GraphicsEngine(params, hInstance, nCmdShow);
	gfxeng::eng = ge; // Set the global eng to our engine so that GetGraphicsEngine(); can work.

	/*
	// Example of GE useage
	GraphicsEngine* eng = GetGraphicsEngine();
	eng->GetCamera()->setPosition(D3DXVECTOR3(0, 15, 0));
	eng->GetCamera()->LookAt(D3DXVECTOR3(10, 10, 10));
	Mesh* testBall = eng->CreateMesh("Media/Ball.obj", D3DXVECTOR3(8, 15, 8));
	Mesh* testCylinder = eng->CreateMesh("Media/Cylinder.obj", D3DXVECTOR3(10, 10, 10));
	Image* testImg = eng->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	Light* testLight = eng->CreateLight(D3DXVECTOR3(10, 20, 10));
	while(eng->isRunning())	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		float diff = eng->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.
		testBall->Rotate(D3DXVECTOR3(diff, diff, 0) * 0.001f);
		if(eng->GetKeyListener()->IsPressed('W'))
			eng->GetCamera()->moveForward(diff);
		if(eng->GetKeyListener()->IsClicked(1))
			eng->GetCamera()->moveBackward(diff);
	}
	*/

	// Create the MainMenu and send the graphics engine, and then run Run();
	MainMenu mm(ge);
	mm.Run();

	
	// Delete graphics engine
	delete ge;
	return 0;
}