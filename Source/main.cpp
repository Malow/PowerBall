#include "stdafx.h"
#include "MainMenu.h"
#include "GraphicsEngine.h"
#include "SoundEngine\SoundEngine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
#if defined(DEBUG) || defined(_DEBUG)
	myInitMemoryCheck();
#endif
	
	MaloW::ClearDebug();
	// Create parameters for the graphics engine, LOAD THEM FROM .cfg-FILE later on!
	GraphicsEngineParams params;
	params.windowHeight = 900;
	params.windowWidth = 1500;
	params.FXAAQuality = 0;			// 0 - 4
	params.ShadowMapSettings = 0;	// 0 - 10 (works with higher but VERY consuming)
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
	StaticMesh* testBall = eng->CreateStaticMesh("Media/Ball.obj", D3DXVECTOR3(8, 16, 8));
	StaticMesh* testCylinder = eng->CreateStaticMesh("Media/Cylinder.obj", D3DXVECTOR3(10, 10, 10));
	StaticMesh* bth = eng->CreateStaticMesh("Media/bth.obj", D3DXVECTOR3(5, 20, 15));

	AnimatedMesh* ani = eng->CreateAnimatedMesh("Media/AniTest.ani", D3DXVECTOR3(12, 16, 12));
	//ani->LoopNormal();
	ani->LoopSeamless();

	/*
	SoundEngine* seng = eng->GetSoundEngine();
	seng->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_ball.mp3", false);
	seng->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_wall.mp3", false);
	seng->LoadSong("Media/Sounds/Songs/america_fuck_yeah.mp3", true);
	seng->SetMasterVolume(0.1f);
	seng->PlaySong(0);
	*/

	eng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");			// going to LoadingScreen to load the above meshes
	bth->Scale(0.1f);
	//Image* testImg = eng->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	Light* testLight = eng->CreateLight(D3DXVECTOR3(8, 20, 8));
	//testLight->SetPosition(testBall->GetPosition() + D3DXVECTOR3(0, 5, 0));
	//testLight->SetLookAt(testLight->GetPosition() - D3DXVECTOR3(0, 5, 0));
	//Light* testLight2 = eng->CreateLight(D3DXVECTOR3(3, 20, 3));
	Text* text = eng->CreateText("Lol ", D3DXVECTOR2(500, 500), 1.0f, "Media/Fonts/1");

	CamRecording* camRec = new CamRecording(2000);	// How many milliseconds between each way point
	camRec->Init(eng->GetCamera());
	camRec->AddCameraWaypoint(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(0, 30, 0), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(30, 30, 30), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(-20, 20, 30), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(0, 50, 0), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(0, 20, 0), D3DXVECTOR3(8, 16, 8));

	bool sw = true;
		
	while(eng->isRunning())	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		float diff = eng->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.

		//testBall->Rotate(D3DXVECTOR3(2*PI, 0, 0) * (diff/1000.0f)); // Divide diff by 1000 to get seconds since diff is in milliseconds.
		testBall->RotateAxis(D3DXVECTOR3(2, 0, 0),  2* PI * (diff/1000.0f)); // Divide diff by 1000 to get seconds since diff is in milliseconds.
		CursorControl cc;

		if(eng->GetKeyListener()->IsPressed('W'))
			eng->GetCamera()->moveForward(diff);
		if(eng->GetKeyListener()->IsPressed(VK_RETURN))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			cc.SetVisibility(true);
		if(eng->GetKeyListener()->IsPressed('A'))
			cc.SetVisibility(false);
		if(eng->GetKeyListener()->IsPressed('S'))	
			eng->GetCamera()->moveBackward(diff);
		if(eng->GetKeyListener()->IsPressed('D'))	
			eng->GetCamera()->moveRight(diff);

		if(eng->GetKeyListener()->IsPressed(VK_BACK))
		{
			if(sw)
				text->DeleteFromEnd(1);
			sw = false;
		}
		else
			sw = true;
		
		if(eng->GetKeyListener()->IsClicked(1))
		{
			text->AppendText("LoL ");
		}

		if(eng->GetKeyListener()->IsClicked(2))
		{
			ge->DeleteText(text);
		}


		if(eng->GetKeyListener()->IsPressed('G'))	
			camRec->Play();						// Play to start moving the camera along the path
		camRec->Update(diff);					// update needed to move the camera when play is initialized.

		/*
		// Testing sound engine
		if(eng->GetKeyListener()->IsPressed('V'))	
			//seng->PlaySoundEffect(0);
			seng->MuteSongChannel();
		if(eng->GetKeyListener()->IsPressed('B'))	
			//seng->PlaySoundEffect(1); 
			seng->UnmuteSongChannel();
		if(eng->GetKeyListener()->IsPressed('Z'))	
			seng->PlaySong(0);
		if(eng->GetKeyListener()->IsPressed('X'))	
			seng->PauseSongChannel(); 
		if(eng->GetKeyListener()->IsPressed('C'))	
			seng->UnpauseSongChannel();
			*/
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
	// Delete sound engine
	//delete se;

	return 0;
}