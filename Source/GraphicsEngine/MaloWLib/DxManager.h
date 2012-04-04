#ifndef DXMANAGER_H
#define DXMANAGER_H

#include "stdafx.h"
#include "Camera.h"
#include "Shader.h"
#include "Buffer.h"
#include "Array.h"
#include "Mesh.h"
#include "Object3D.h"
#include "GraphicsEngineParameters.h"
#include "Light.h"
#include "Process.h"
#include "Image.h"



class RendererEvent : public MaloW::ProcessEvent
{
private:
	string message;
	Mesh* mesh;
	Light* light;
	Image* image;
	bool deleteSelf;

public:
	RendererEvent(string message = "", Mesh* mesh = NULL, Light* light = NULL, Image* image = NULL) 
	{ 
		this->message = message; 
		this->mesh = mesh; 
		this->light = light;
		this->image = image;
		this->deleteSelf = true;
	}
	virtual ~RendererEvent() 
	{ 
		if(this->deleteSelf && this->message.substr(0, 6) != "Delete")
		{
			if(this->mesh)
				delete this->mesh;
			if(this->light)
				delete this->light;
			if(this->image)
				delete this->image;
		}
	}
	string getMessage() { return this->message; }
	Mesh* GetMesh() { this->deleteSelf = false; return this->mesh; }
	Light* GetLight() { this->deleteSelf = false; return this->light; }
	Image* GetImage() { this->deleteSelf = false; return this->image; }
};

class DxManager : public MaloW::Process
{
private:
	// Standard stuff
	ID3D11DeviceContext* Dx_DeviceContext;
	ID3D11DepthStencilView* Dx_DepthStencilView;
	ID3D11Texture2D* Dx_DepthStencil;
	ID3D11RenderTargetView* Dx_RenderTargetView;
	IDXGISwapChain* Dx_SwapChain;
	ID3D11Device* Dx_Device;
	Shader*	Shader_ForwardRendering;
	HWND hWnd;
	D3D11_VIEWPORT Dx_Viewport;

	//Own systems
	long framecount;
	GraphicsEngineParams params;
	Camera* camera;
	MaloW::Array<Mesh*> objects;

	MaloW::Array<Image*> images;

	// Lightning
	MaloW::Array<Light*> lights;
	
	// Shadow map:
	int currentShadowMapSize;
	Shader* Shader_ShadowMap;
	Shader* Shader_BillBoard;

	// Deferred Rendering
	// Gbuffer:
	static const int NrOfRenderTargets = 4;
	ID3D11Texture2D* Dx_GbufferTextures[NrOfRenderTargets];
	ID3D11RenderTargetView* Dx_GbufferRTs[NrOfRenderTargets];
	ID3D11ShaderResourceView* Dx_GbufferSRVs[NrOfRenderTargets];
	Shader* Shader_DeferredGeometry;
	Shader* Shader_DeferredLightning;

	void RenderForward();
	void RenderDeferredGeometry();
	void RenderDeferredPerPixel();
	void RenderParticles();
	void RenderShadowMap();
	void RenderImages();
	
	HRESULT Init();

	int TriangleCount;
	

public:
	DxManager(HWND g_hWnd, GraphicsEngineParams params, Camera* cam);
	virtual ~DxManager();

	virtual void Life();
	HRESULT Render();
	HRESULT Update(float deltaTime);

	void CreateSmokeEffect();

	void createObject(Mesh* mesh);
	Object3D* createParticleObject(ParticleMesh* mesh);
	Light* CreateLight(D3DXVECTOR3 pos);
	void CreateImage(Image* image, string texture);

	long GetFrameCount() const { return this->framecount; }

	void deleteObject(Mesh* mesh);
	void DeleteLight(Light* light);
	void DeleteImage(Image* image);

	void SetCamera(Camera* cam) { this->camera = cam; }
	Camera* GetCamera() const { return this->camera; }
	
	void MoveLight(int id, D3DXVECTOR3 moveBy) { if(id < this->lights.size()) this->lights[id]->Move(moveBy); }
	void ChangeShadowMapSize(int index) { if(index < 5 && index > -1) this->currentShadowMapSize = index; }

	int GetTriangleCount() { return this->TriangleCount; }
};

#endif