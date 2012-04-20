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
#include "SSAO.h"
#include "Text.h"
#include "SkyBox.h"
#include "FXAA.h"



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
	MaloW::Array<Text*> texts;

	ID3D11ShaderResourceView* LavaTexture;
	SkyBox* skybox;
	Shader* Shader_Skybox;

	// Lightning
	MaloW::Array<Light*> lights;
	
	// Shadow map:
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

	Shader* Shader_DeferredQuad;
	Shader* Shader_DeferredTexture;
	ID3D11Texture2D* Dx_DeferredTexture;
	ID3D11RenderTargetView* Dx_DeferredQuadRT;
	ID3D11ShaderResourceView* Dx_DeferredSRV;

	
	SSAO* ssao;
	FXAA* fxaa;
	Shader* Shader_Fxaa;

	// Timer
	float PCFreq;
	__int64 prevTimeStamp;

	float TimerAnimation;

	void RenderForward();
	void RenderDeferredGeometry();
	void RenderDeferredPerPixel();
	void RenderParticles();
	void RenderShadowMap();
	void RenderImages();
	void RenderQuadDeferred();
	void RenderDeferredTexture();
	void RenderDeferredSkybox();
	void RenderAntiAliasing();
	
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
	Light* CreateLight(D3DXVECTOR3 pos, bool UseShadowMap);
	void CreateImage(Image* image, string texture);
	void CreateText(Text* text, string font);
	void CreateSkyBox(string texture);

	long GetFrameCount() const { return this->framecount; }

	void deleteObject(Mesh* mesh);
	void DeleteLight(Light* light);
	void DeleteImage(Image* image);
	void DeleteText(Text* text);

	void SetCamera(Camera* cam) { this->camera = cam; }
	Camera* GetCamera() const { return this->camera; }

	int GetTriangleCount() { return this->TriangleCount; }
};

#endif