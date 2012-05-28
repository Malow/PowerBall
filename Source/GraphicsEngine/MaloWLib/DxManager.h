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
#include "StaticMesh.h"
#include "AnimatedMesh.h"

/* Process events for adding to rendering */

class RendererEvent : public MaloW::ProcessEvent
{
protected:
	string message;
	bool deleteSelf;

public:
	RendererEvent(string message = "") 
	{ 
		this->message = message; 
		this->deleteSelf = true;
	}
	virtual ~RendererEvent() 
	{ 
	}
	string getMessage() { return this->message; }
};


class MeshEvent : public RendererEvent
{
private:
	StaticMesh* mesh;
	AnimatedMesh* ani;

public:
	MeshEvent(string message, StaticMesh* mesh, AnimatedMesh* ani) : RendererEvent(message)
	{
		this->mesh = mesh; 
		this->ani = ani;
	}
	virtual ~MeshEvent() 
	{
		if(this->deleteSelf && this->message.substr(0, 6) != "Delete")
		{
			if(this->mesh)
				delete this->mesh;
			if(this->ani)
				delete this->ani;
		}
	}

	StaticMesh* GetStaticMesh() { this->deleteSelf = false; return this->mesh; }
	AnimatedMesh* GetAnimatedMesh() { this->deleteSelf = false; return this->ani; }
};

class LightEvent : public RendererEvent
{
private:
	Light* light;

public:
	LightEvent(string msg, Light* light) : RendererEvent(msg)
	{
		this->light = light;
	}
	virtual ~LightEvent() 
	{
		if(this->deleteSelf && this->message.substr(0, 6) != "Delete")
		{
			if(this->light)
				delete this->light;
		}
	}
	Light* GetLight() { this->deleteSelf = false; return this->light; }
};

class ImageEvent : public RendererEvent
{
private:
	Image* img;

public:
	ImageEvent(string msg, Image* img) : RendererEvent(msg)
	{
		this->img = img;
	}
	virtual ~ImageEvent() 
	{
		if(this->deleteSelf && this->message.substr(0, 6) != "Delete")
		{
			if(this->img)
				delete this->img;
		}
	}
	Image* GetImage() { this->deleteSelf = false; return this->img; }
};

class TextEvent : public RendererEvent
{
private:
	Text* txt;

public:
	TextEvent(string msg, Text* txt) : RendererEvent(msg)
	{
		this->txt = txt;
	}
	virtual ~TextEvent() 
	{
		if(this->deleteSelf && this->message.substr(0, 6) != "Delete")
		{
			if(this->txt)
				delete this->txt;
		}
	}
	Text* GetText() { this->deleteSelf = false; return this->txt; }
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
	MaloW::Array<StaticMesh*> objects;
	MaloW::Array<AnimatedMesh*> animations;

	MaloW::Array<Image*> images;
	MaloW::Array<Text*> texts;

	ID3D11ShaderResourceView* LavaTexture;
	float LavaWavesOuterRadius;
	SkyBox* skybox;
	Shader* Shader_Skybox;

	// Lightning
	MaloW::Array<Light*> lights;
	
	// Shadow map:
	Shader* Shader_ShadowMap;
	Shader* Shader_BillBoard;
	Shader* Shader_Text;
	Shader* Shader_ShadowMapAnimated;
	
	// Deferred Rendering
	// Gbuffer:
	static const int NrOfRenderTargets = 4;
	ID3D11Texture2D* Dx_GbufferTextures[NrOfRenderTargets];
	ID3D11RenderTargetView* Dx_GbufferRTs[NrOfRenderTargets];
	ID3D11ShaderResourceView* Dx_GbufferSRVs[NrOfRenderTargets];
	Shader* Shader_DeferredGeometry;
	Shader* Shader_DeferredLightning;
	Shader* Shader_InvisibilityEffect;

	Shader* Shader_DeferredQuad;
	Shader* Shader_DeferredTexture;
	ID3D11Texture2D* Dx_DeferredTexture;
	ID3D11RenderTargetView* Dx_DeferredQuadRT;
	ID3D11ShaderResourceView* Dx_DeferredSRV;

	Shader* Shader_DeferredAnimatedGeometry;

	
	SSAO* ssao;
	FXAA* fxaa;
	Shader* Shader_Fxaa;

	bool invisibleGeometry;

	// Timer
	float PCFreq;
	__int64 prevTimeStamp;

	float TimerAnimation;

	void RenderForward();
	void RenderDeferredGeometry();
	void RenderDeferredPerPixel();
	void RenderInvisibilityEffect();
	void RenderParticles();
	void RenderShadowMap();
	void RenderImages();
	void RenderQuadDeferred();
	void RenderDeferredTexture();
	void RenderDeferredSkybox();
	void RenderAntiAliasing();
	void RenderText();
	
	HRESULT Init();

	int TriangleCount;
	

public:
	bool StartRender;
	DxManager(HWND g_hWnd, GraphicsEngineParams params, Camera* cam);
	virtual ~DxManager();

	void HandleMeshEvent(MeshEvent* me);
	void HandleLightEvent(LightEvent* le);
	void HandleImageEvent(ImageEvent* ie);
	void HandleTextEvent(TextEvent* te);
	virtual void Life();
	HRESULT Render();
	HRESULT Update(float deltaTime);

	void CreateSmokeEffect();

	void CreateStaticMesh(StaticMesh* mesh);
	void CreateAnimatedMesh(AnimatedMesh* mesh);
	Object3D* createParticleObject(ParticleMesh* mesh);
	Light* CreateLight(D3DXVECTOR3 pos, bool UseShadowMap);
	void CreateImage(Image* image, string texture);
	void CreateText(Text* text, string font);
	void CreateSkyBox(string texture);

	long GetFrameCount() const { return this->framecount; }

	void DeleteStaticMesh(StaticMesh* mesh);
	void DeleteAnimatedMesh(AnimatedMesh* mesh);
	void DeleteLight(Light* light);
	void DeleteImage(Image* image);
	void DeleteText(Text* text);

	void SetCamera(Camera* cam) { this->camera = cam; }
	Camera* GetCamera() const { return this->camera; }

	void SetLavaWavesOuterRadius(float outerRadius) { this->LavaWavesOuterRadius = outerRadius; }
	float GetLavaHeightAt(float x, float z);

	int GetTriangleCount() { return this->TriangleCount; }
};

#endif