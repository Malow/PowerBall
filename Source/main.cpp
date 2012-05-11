#include "stdafx.h"
#include "MainMenu.h"
#include "GraphicsEngine.h"
#include "SoundEngine\SoundEngine.h"

void test();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	
#if defined(DEBUG) || defined(_DEBUG)
	myInitMemoryCheck();
#endif
	MaloW::ClearDebug();
	//_CrtSetBreakAlloc(1537);
	// Create parameters for the graphics engine, LOAD THEM FROM .cfg-FILE later on!
	GraphicsEngineParams params;
	params.windowWidth = 1600;
	params.windowHeight = 900;
	params.FXAAQuality = 1;			// 0 - 4 
	params.ShadowMapSettings = 2;	// 0 - 10 (works with higher but VERY consuming)
	params.CamType = FPS;

	// RunAgain for changing resolution etc.
	/*
	bool RunAgain = true;
	while(RunAgain)
	{
		RunAgain = false;*/
		// Create the graphics engine
		GraphicsEngine* ge = new GraphicsEngine(params, hInstance, nCmdShow);
		gfxeng::eng = ge; // Set the global eng to our engine so that GetGraphicsEngine(); can work.
		ge->CreateSkyBox("Media/skymap.dds");
	
		
		//test();	// Instead of ifndef lol

		
		// Create the MainMenu and send the graphics engine, and then run Run();
		MainMenu* mm = new MainMenu(ge);
		/*RunAgain = */mm->Run();
		delete mm;
		// Delete graphics engine
		delete ge;
	//}
	#if defined(DEBUG) || defined(_DEBUG)
		myDumpMemoryLeaks();
	#endif
	
	return 0;
}




void test()
{
	GraphicsEngine* ge = GetGraphicsEngine();

	// Example of GE useage
	GraphicsEngine* eng = GetGraphicsEngine();
	eng->GetCamera()->setPosition(D3DXVECTOR3(0, 15, -15.6));
	eng->GetCamera()->LookAt(D3DXVECTOR3(30, 10, 10));
	StaticMesh* testBall = eng->CreateStaticMesh("Media/Ball.obj", D3DXVECTOR3(8, 15, 8));
	StaticMesh* testCylinder = eng->CreateStaticMesh("Media/Cylinder.obj", D3DXVECTOR3(10, 10, 10));
	StaticMesh* bth = eng->CreateStaticMesh("Media/bth.obj", D3DXVECTOR3(5, 20, 15));
	
	AnimatedMesh* flag = eng->CreateAnimatedMesh("Media/FlagRed.ani",  D3DXVECTOR3(8, 15, 8));
	flag->LoopSeamless();

	AnimatedMesh* flagb = eng->CreateAnimatedMesh("Media/FlagBlue.ani",  D3DXVECTOR3(15, 15, 8));
	flagb->LoopSeamless();
	/*
	StaticMesh* flag1 = eng->CreateStaticMesh("Media/FlagBlue1.obj", D3DXVECTOR3(8, 15, 8));
	StaticMesh* flag2 = eng->CreateStaticMesh("Media/FlagBlue2.obj", D3DXVECTOR3(10, 15, 8));
	StaticMesh* flag3 = eng->CreateStaticMesh("Media/FlagBlue3.obj", D3DXVECTOR3(12, 15, 8));
	StaticMesh* flag4 = eng->CreateStaticMesh("Media/FlagBlue4.obj", D3DXVECTOR3(14, 15, 8));
	StaticMesh* flag5 = eng->CreateStaticMesh("Media/FlagBlue5.obj", D3DXVECTOR3(16, 15, 8));
	*/

	bth->Scale(0.1f);
	Light* testLight = eng->CreateLight(D3DXVECTOR3(15, 20, 15));
	
	//testBall->SetSpecialColor(RED_COLOR);
	//testCylinder->SetSpecialColor(GREEN_COLOR);
	
	AnimatedMesh* ani = eng->CreateAnimatedMesh("Media/AniTest.ani", D3DXVECTOR3(12, 16, 12));
	//ani->LoopNormal();
	ani->LoopSeamless();

	
	SoundEngine* seng = eng->GetSoundEngine();
	SoundEffect* se1 = seng->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_ball.mp3", false);
	//SoundEffect* se2 = seng->LoadSoundEffect("Media/Sounds/SoundEffects/ball_vs_wall.mp3", false);
	//SoundSong* ss1 = seng->LoadSong("Media/Sounds/Songs/america_fuck_yeah.mp3", true);
	seng->SetMasterVolume(0.5f);
	//ss1->Play();
	
	
	//eng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");			// going to LoadingScreen to load the above meshes
	//eng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 0.0f, 1.0f, 1.0f, 1.0f);
	
	eng->LoadingScreen("Media/LoadingScreen/StartScreen.png", "", 0.0f, 1.0f, 1.0f, 1.0f);
	Text* text = eng->CreateText("Lol ", D3DXVECTOR2(500, 500), 1.0f, "Media/Fonts/1");
	
	//Image* testImg = eng->CreateImage(D3DXVECTOR2(50, 50), D3DXVECTOR2(500, 75), "Media/PowerBall.png");
	
	//testLight->SetPosition(testBall->GetPosition() + D3DXVECTOR3(0, 5, 0));
	//testLight->SetLookAt(testLight->GetPosition() - D3DXVECTOR3(0, 5, 0));
	//Light* testLight2 = eng->CreateLight(D3DXVECTOR3(3, 20, 3));
	

	CamRecording* camRec = new CamRecording(2000, true);	// How many milliseconds between each way point
	camRec->Init(eng->GetCamera());
	
	/*camRec->AddCameraWaypoint(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(0, 30, 0), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(30, 30, 30), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(-20, 20, 30), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(0, 50, 0), D3DXVECTOR3(8, 16, 8));
	camRec->AddCameraWaypoint(D3DXVECTOR3(0, 20, 0), D3DXVECTOR3(8, 16, 8));*/
	//camRec->Load(CIRCLE_AROUND);
	camRec->CircleAround(true, 50, 1000, 0, D3DXVECTOR3(30, 50, 0), D3DXVECTOR3(0,0,30));
	
	bool sw = true;
	float size = 1.0f;

	bool go = true;
		
	while(eng->isRunning() && go)	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
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
			eng->GetCamera()->moveLeft(diff);
		if(eng->GetKeyListener()->IsPressed('S'))	
			eng->GetCamera()->moveBackward(diff);
		if(eng->GetKeyListener()->IsPressed('D'))	
			eng->GetCamera()->moveRight(diff);

		if(eng->GetKeyListener()->IsPressed(VK_ESCAPE))
			go = false;

		if(eng->GetKeyListener()->IsPressed(VK_BACK))
		{
			if(sw)
			{
				//se1->Play();
				text->DeleteFromEnd(1);
			}
			sw = false;
		}
		else
			sw = true;
		
		if(eng->GetKeyListener()->IsClicked(1))
		{
			size += diff * 0.001f;
			text->SetSize(size);
			//ss1->SetVolume(0.5f);
			text->AppendText("LoL ");
			//ss1->Play();
		}
		//else
			//se1->Play();

		if(eng->GetKeyListener()->IsClicked(2))
		{
			testBall->UseInvisibilityEffect(true);
		}
		else 
			testBall->UseInvisibilityEffect(false);

		if(eng->GetKeyListener()->IsPressed('G'))	
		{
			static bool once = false;
			if(!once)
			{
				once = true;
				camRec->Play();						// Play to start moving the camera along the path
			}
		}
		camRec->Update(diff);					// update needed to move the camera when play is initialized.
	
		//Testing sound engine
		if(eng->GetKeyListener()->IsPressed('B'))	
		{
			for(int i = 0; i < 200; i++)
			{
				se1->Play();
			}
		}
		
	}
	// Delete camera recording
	delete camRec;
}