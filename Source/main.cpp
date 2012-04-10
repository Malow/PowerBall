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
	params.CamType = RTS;

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
	Mesh* bth = eng->CreateMesh("Media/bth.obj", D3DXVECTOR3(5, 20, 15));
	bth->Scale(0.1f);
	Image* testImg = eng->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	Light* testLight = eng->CreateLight(D3DXVECTOR3(13, 20, 13));
	Light* testLight2 = eng->CreateLight(D3DXVECTOR3(3, 20, 3));
	testLight->SetLookAt(testBall->GetPosition());
	testLight2->SetLookAt(testBall->GetPosition());

	while(eng->isRunning())	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		float diff = eng->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.

		testBall->Rotate(D3DXVECTOR3(2*PI, 0, 0) * (diff/1000.0f)); // Divide diff by 1000 to get seconds since diff is in milliseconds.

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