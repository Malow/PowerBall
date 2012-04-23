#include "GraphicsEngine.h"

bool GraphicsEngine::initDone = false;
DxManager* GraphicsEngine::dx = NULL;
HINSTANCE GraphicsEngine::hInstance = NULL;
HWND GraphicsEngine::hWnd = NULL;
MaloW::KeyListener* GraphicsEngine::kl = NULL;

GraphicsEngine* gfxeng::eng = NULL;

bool CursorControl::visable = true;

GraphicsEngine::GraphicsEngine(GraphicsEngineParams params, HINSTANCE hInstance, int nCmdShow)
{
	if(!this->initDone)
	{
		this->parameters = params;
		this->cam = NULL;
		this->dx = NULL;
		this->hInstance = NULL;
		this->hWnd = NULL;

		this->keepRunning = true;
		this->loading = false;

		LARGE_INTEGER li;
		if(!QueryPerformanceFrequency(&li))
			MaloW::Debug("QueryPerformanceFrequency Failed!, High resolution performance counter not available?");

		this->PCFreq = float(li.QuadPart)/1000.0f;
		QueryPerformanceCounter(&li);
		this->prevTimeStamp = li.QuadPart;



		this->prevFrameCount = 0;
		this->fpsLast = 0;
		this->fpsTimer = 0;
	}
	this->InitWindow(hInstance, nCmdShow);

	this->Start();
}

GraphicsEngine::~GraphicsEngine()
{
	// Close self thread.
	this->Close();
	this->WaitUntillDone();

	// Close DxManager thread.
	this->dx->Close();
	this->dx->WaitUntillDone();
	
	if(this->dx)
		delete this->dx;
	if(this->kl)
		delete this->kl;
}

LRESULT CALLBACK GraphicsEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message) 
	{
		case WM_KEYDOWN:
			if(kl)
				kl->KeyDown(wParam);
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			break;
		
		case WM_KEYUP:
			if(kl)
				kl->KeyUp(wParam);

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		// Mouse
		case WM_LBUTTONDOWN:
			if(kl)
				kl->MouseDown(1);
			break;
		case WM_LBUTTONUP:
			if(kl)
				kl->MouseUp(1);
			break;

		case WM_RBUTTONDOWN:
			if(kl)
				kl->MouseDown(2);
			break;
		case WM_RBUTTONUP:
			if(kl)
				kl->MouseUp(2);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT GraphicsEngine::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	this->hInstance = hInstance;

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = this->WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = this->hInstance;
	wcex.hIcon          = 0;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = "GraphicsEngine";
	wcex.hIconSm        = 0;
	if( !RegisterClassEx(&wcex) )
		return E_FAIL;

	// Create window
	RECT rc = { 0, 0, this->parameters.windowWidth, this->parameters.windowHeight };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	

	this->hWnd = CreateWindow("GraphicsEngine", "GraphicsEngine - Direct3D 11.0", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, this->hInstance, NULL);
	if(!this->hWnd)
		return E_FAIL;

	ShowWindow(this->hWnd, nCmdShow);
	MoveWindow(this->hWnd, 0, 0, rc.right - rc.left, rc.bottom - rc.top, false);

	this->InitObjects();

	this->initDone = true;

	return S_OK;
}

void GraphicsEngine::InitObjects()
{
	this->dx = new DxManager(this->hWnd, this->parameters, this->cam);
	this->kl = new MaloW::KeyListener(this->hWnd);

	if(this->parameters.CamType == FPS)
	{
		this->cam = new FPSCamera(this->hWnd, this->parameters);
	}
	else if(this->parameters.CamType == RTS)
	{
		this->cam = new RTSCamera(this->hWnd, this->parameters);
	}
	this->dx->SetCamera(this->cam);

	this->dx->Start();
}

StaticMesh* GraphicsEngine::CreateStaticMesh(string filename, D3DXVECTOR3 pos)
{
	StaticMesh* mesh = new StaticMesh(pos);

	LoadMeshEvent* re = new LoadMeshEvent(filename, mesh, NULL);
	this->PutEvent(re);

	return mesh;
}

StaticMesh* GraphicsEngine::CreateStaticMesh(string filename, D3DXVECTOR3 pos, MaterialType material)
{
	Material* mat = new Material(material);
	return this->CreateStaticMesh(filename, pos, mat);
}

StaticMesh* GraphicsEngine::CreateStaticMesh(string filename, D3DXVECTOR3 pos, Material* material)
{
	StaticMesh* mesh = new StaticMesh(pos);

	LoadMeshEvent* re = new LoadMeshEvent(filename, mesh, material);
	this->PutEvent(re);

	return mesh;
}

Light* GraphicsEngine::CreateLight(D3DXVECTOR3 pos, bool UseShadowMap)
{
	return this->dx->CreateLight(pos, UseShadowMap);
}

Terrain* GraphicsEngine::CreateTerrain(D3DXVECTOR3 position, D3DXVECTOR3 dimension, std::string texture, string heightmap, int vertexSize)
{
	Terrain* terrain = new Terrain(position, dimension, texture, heightmap, vertexSize);
	this->dx->CreateStaticMesh(terrain);

	return terrain;
}

Image* GraphicsEngine::CreateImage(D3DXVECTOR2 position, D3DXVECTOR2 dimensions, string texture)
{
	Image* img = new Image(position, dimensions);
	this->dx->CreateImage(img, texture);
	return img;
}

bool GraphicsEngine::DeleteImage(Image* delImage)
{
	this->dx->DeleteImage(delImage);
	return true;
}

Text* GraphicsEngine::CreateText(string text, D3DXVECTOR2 position, float size, string fontTexturePath)
{
	Text* textobj = new Text(text, position, size);
	this->dx->CreateText(textobj, fontTexturePath);
	return textobj;
}

bool GraphicsEngine::DeleteText(Text* delText)
{

	return true;
}

float GraphicsEngine::Update()
{
	MSG msg = {0};
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))		// changed from if to while, cuz I wanna clear the msg log because inputs are more important than FPS.
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message == WM_QUIT)
			this->keepRunning = false;
	}
	
	// Timer
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	float diff = (li.QuadPart - prevTimeStamp) / this->PCFreq;
	this->prevTimeStamp = li.QuadPart;

	this->fpsTimer += diff;
	if(this->fpsTimer > 1000.0f)
	{
		int framecount = this->dx->GetFrameCount();
		this->fpsLast = framecount - prevFrameCount;
		this->prevFrameCount = framecount;
		this->fpsTimer = 0;
	}
	
	this->dx->Update(diff);

	std::string txt = "FPS: " + MaloW::convertNrToString((float)this->fpsLast) + " - "; 
	txt += "Camera Pos: " + MaloW::convertNrToString(this->dx->GetCamera()->getPosition().x) + " " + 
		MaloW::convertNrToString(this->dx->GetCamera()->getPosition().y) + " " + 
		MaloW::convertNrToString(this->dx->GetCamera()->getPosition().z) + "    -    Triangle Count: " + MaloW::convertNrToString((float)this->dx->GetTriangleCount());

	SetWindowText(this->hWnd, txt.c_str());

	return diff;
}

bool GraphicsEngine::isRunning()
{
	return this->keepRunning;
}

void GraphicsEngine::Life()
{
	while(this->stayAlive)
	{
		if(MaloW::ProcessEvent* ev = this->WaitEvent())
		{
			this->loading = true;
			if(dynamic_cast<LoadMeshEvent*>(ev) != NULL)
			{
				string filename = ((LoadMeshEvent*)ev)->GetFileName();
				StaticMesh* mesh = ((LoadMeshEvent*)ev)->GetMesh();

				mesh->LoadFromFile(filename);
				this->dx->CreateStaticMesh(mesh);

				if(Material* material = ((LoadMeshEvent*)ev)->GetMaterial())
				{
					MaloW::Array<MeshStrip*>* strips = mesh->GetStrips();
					for(int i = 0; i < strips->size(); i++)
					{
						strips->get(i)->SetMaterial(material);
						if(i+1 < strips->size())
							material = new Material(material);
					}
				}
			}

			delete ev;

			if(this->GetEventQueueSize() == 0)
				this->loading = false;
		}
	}
}

void GraphicsEngine::CreateSkyBox(string texture)
{
	this->dx->CreateSkyBox(texture);
}

void GraphicsEngine::LoadingScreen(string BackgroundTexture, string ProgressBarTexture)
{
	Image* bg = this->CreateImage(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2((float)this->parameters.windowWidth, (float)this->parameters.windowHeight), BackgroundTexture);
	Image* pb = this->CreateImage(D3DXVECTOR2((this->parameters.windowWidth / 4.0f), ((this->parameters.windowHeight * 3.0f) / 4.0f)), D3DXVECTOR2(0, this->parameters.windowHeight / 10.0f), ProgressBarTexture);

	int TotalItems = this->GetEventQueueSize();

	float dx = (this->parameters.windowWidth / 2.0f) / TotalItems;
	float y = this->parameters.windowHeight / 10.0f;


	bool go = true;
	while(go)
	{
		this->Update();

		int ItemsToGo = this->GetEventQueueSize();
		
		pb->SetDimensions(D3DXVECTOR2(dx * (TotalItems - ItemsToGo), y));
		if(!this->loading)
			go = false;
	}

	this->DeleteImage(pb);
	this->DeleteImage(bg);
}