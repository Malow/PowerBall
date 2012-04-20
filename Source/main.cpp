#include "stdafx.h"
#include "MainMenu.h"
#include "GraphicsEngine.h"
#include "InGameMenu.h"

#include "CamRecording.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
#if defined(DEBUG) || defined(_DEBUG)
	myInitMemoryCheck();
#endif
	
	MaloW::ClearDebug();
	// Create parameters for the graphics engine, LOAD THEM FROM .cfg-FILE later on!
	GraphicsEngineParams params;
	params.windowHeight = 900;
	params.windowWidth = 1600;
	params.FXAAQuality = 3;			// 0 - 6
	params.ShadowMapSettings = 6;	// 0 - 10 (works with higher but VERY consuming)
	params.CamType = RTS;
	
	// Create the graphics engine
	GraphicsEngine* ge = new GraphicsEngine(params, hInstance, nCmdShow);
	gfxeng::eng = ge; // Set the global eng to our engine so that GetGraphicsEngine(); can work.
	ge->CreateSkyBox("Media/skymap.dds");
	
	//#define LOLTEST
	#ifdef LOLTEST
	
	// Example of GE useage
	GraphicsEngine* eng = GetGraphicsEngine();
	eng->GetCamera()->setPosition(D3DXVECTOR3(0, 15, -15.6));
	eng->GetCamera()->LookAt(D3DXVECTOR3(30, 10, 10));
	Mesh* testBall = eng->CreateMesh("Media/Ball.obj", D3DXVECTOR3(8, 15, 8));
	Mesh* testCylinder = eng->CreateMesh("Media/Cylinder.obj", D3DXVECTOR3(10, 10, 10));
	Mesh* bth = eng->CreateMesh("Media/bth.obj", D3DXVECTOR3(5, 20, 15));
	eng->LoadingScreen("Media/LoadingScreenBG.png", "Media/LoadingScreenPB.png");			// going to LoadingScreen to load the above meshes
	bth->Scale(0.1f);
	Image* testImg = eng->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	Light* testLight = eng->CreateLight(D3DXVECTOR3(8, 20, 8));
	//testLight->SetPosition(testBall->GetPosition() + D3DXVECTOR3(0, 5, 0));
	//testLight->SetLookAt(testLight->GetPosition() - D3DXVECTOR3(0, 5, 0));
	//Light* testLight2 = eng->CreateLight(D3DXVECTOR3(3, 20, 3));
	//Text* text = eng->CreateText("LolAwesome", D3DXVECTOR2(300, 300), 20, "Media/Fonts/1.png");

	CamRecording* camRec = new CamRecording();
	camRec->Init(eng->GetCamera());
	//camRec->Open("spline.txt");

	while(eng->isRunning())	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		float diff = eng->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.

		testBall->Rotate(D3DXVECTOR3(2*PI, 0, 0) * (diff/1000.0f)); // Divide diff by 1000 to get seconds since diff is in milliseconds.
		//testBall->RotateAxis(D3DXVECTOR3(1, 0, 0),  2* PI * (diff/1000.0f)); // Divide diff by 1000 to get seconds since diff is in milliseconds.
		CursorControl cc;

		if(eng->GetKeyListener()->IsPressed('W'))
			eng->GetCamera()->moveForward(diff);
		if(eng->GetKeyListener()->IsPressed(VK_RETURN))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			cc.SetVisibility(true);
		if(eng->GetKeyListener()->IsPressed('A'))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			cc.SetVisibility(false);
		if(eng->GetKeyListener()->IsPressed('S'))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			eng->GetCamera()->moveBackward(diff);
		if(eng->GetKeyListener()->IsPressed('D'))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			eng->GetCamera()->moveRight(diff);
		if(eng->GetKeyListener()->IsClicked(1))
			eng->GetCamera()->moveBackward(diff);


		if(eng->GetKeyListener()->IsPressed('R'))	
			camRec->Record(true);
		if(eng->GetKeyListener()->IsPressed('T'))	
			camRec->Record(false);
		camRec->Update(0.0034f);
	}
	// Delete camera recording
	delete camRec;

	#endif
	// Create the MainMenu and send the graphics engine, and then run Run();
	
	MainMenu* mm = new MainMenu(ge);
	mm->Run();
	delete mm;
	// Delete graphics engine
	delete ge;
	return 0;
}