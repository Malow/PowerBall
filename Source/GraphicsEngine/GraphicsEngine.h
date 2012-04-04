#pragma once

/*
Wrapper-class for the graphics engine. Contains the interface that user use from the outside.
Singleton.
*/

#include "DxManager.h"
#include "KeyListener.h"
#include "GraphicsEngineParameters.h"
#include "FPSCamera.h"
#include "RTSCamera.h"
#include "Image.h"




// Class for communication between processes for loading meshes
class LoadMeshEvent : public MaloW::ProcessEvent
{
private:
	string fileName;
	Mesh* mesh;
	Material* mat;
	bool selfdelete;

public:
	LoadMeshEvent(string fileName, Mesh* mesh, Material* mat) 
	{ 
		this->fileName = fileName; 
		this->mesh = mesh; 
		this->mat = mat;
		this->selfdelete = true;
	}
	virtual ~LoadMeshEvent() 
	{
		if(this->selfdelete) 
		{
			if(this->mesh) 
				delete this->mesh;
			if(this->mat)
				delete this->mat;
		}
	}
	string GetFileName() { return this->fileName; }
	Mesh* GetMesh() { this->selfdelete = false; return this->mesh; }
	Material* GetMaterial() { this->selfdelete = false; return this->mat; }
};

class GraphicsEngine : public MaloW::Process
{
private:
	static bool initDone;
	static DxManager* dx;
	static HINSTANCE hInstance;
	static HWND hWnd;
	static MaloW::KeyListener* kl;
	Camera* cam;

	int fpsLast;
	int prevFrameCount;
	float fpsTimer;

	bool keepRunning;

	GraphicsEngineParams parameters;

	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	void InitObjects();


	float PCFreq;
	__int64 prevTimeStamp;


public:
	GraphicsEngine(GraphicsEngineParams params, HINSTANCE hInstance, int nCmdShow);
	virtual ~GraphicsEngine();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/*! Creates a Mesh and starts rendering it every frame. Return is a pointer to the Mesh created. To stop rendering it call DeleteMesh on the engine with the Mesh as parameter. */
	Mesh* CreateMesh(string filename, D3DXVECTOR3 pos, MaterialType material);
	Mesh* CreateMesh(string filename, D3DXVECTOR3 pos, Material* material);
	Mesh* CreateMesh(string filename, D3DXVECTOR3 pos);


	Light* CreateLight(D3DXVECTOR3 pos);
	Terrain* CreateTerrain(D3DXVECTOR3 position, D3DXVECTOR3 dimension, std::string texture, string heightmap, int vertexSize = 256);

	/*! Stops rendering the Mesh and internally deletes it and the pointer will become NULL. Return depends on if the Mesh was sucessfully removed. */
	void DeleteMesh(Mesh* mesh) { this->dx->deleteObject(mesh); }

	/*! Create an Image and starts rendering it every frame. Return is a pointer to the image created. Coordinates are in screen-space. 
	To stop rendering it call DeleteImage on the engine with the image as parameter. */
	Image* CreateImage(D3DXVECTOR2 position, D3DXVECTOR2 dimensions, string texture);

	/*! Stops rendering the Image and internally deletes it and the pointer will become NULL. Return depends on if the Image was sucessfully removed. */
	bool DeleteImage(Image* delImage);

	float Update();
	bool isRunning();

	// Get's
	Camera* GetCamera() const { return this->dx->GetCamera(); }
	MaloW::KeyListener* GetKeyListener() const { return this->kl; }
	HWND GetWindowHandle() const { return this->hWnd; }

	void CreateSmokeEffect() { this->dx->CreateSmokeEffect(); }

	void MoveLight(int id, D3DXVECTOR3 moveBy) { this->dx->MoveLight(id, moveBy); }
	void SetShadowMapQuality(int quality) { this->dx->ChangeShadowMapSize(quality); }
	virtual void Life();
};

// Used as an easy single-ton.
struct gfxeng
{
	static GraphicsEngine* eng;
};

inline GraphicsEngine* GetGraphicsEngine()
{
	return gfxeng::eng;
}