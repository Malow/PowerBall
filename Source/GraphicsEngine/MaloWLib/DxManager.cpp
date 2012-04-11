#include "DxManager.h"

DxManager::DxManager(HWND g_hWnd, GraphicsEngineParams params, Camera* cam)
{
	this->params = params;
	this->hWnd = g_hWnd;

	this->Dx_DeviceContext = NULL;
	this->Dx_DepthStencilView = NULL;
	this->Dx_DepthStencil = NULL;
	this->Dx_RenderTargetView = NULL;
	this->Shader_ForwardRendering = NULL;
	this->Dx_SwapChain = NULL;
	this->Dx_Device = NULL;

	this->Shader_ShadowMap = NULL;

	this->Shader_BillBoard = NULL;

	this->currentShadowMapSize = 0;
	for(int i = 0; i < NrOfRenderTargets; i++)
	{
		this->Dx_GbufferTextures[i] = NULL;
		this->Dx_GbufferRTs[i] = NULL;
		this->Dx_GbufferSRVs[i] = NULL;
	}

	this->Shader_DeferredGeometry = NULL;
	this->Shader_DeferredLightning = NULL;

	this->Shader_DeferredQuad = NULL;
	this->Shader_DeferredTexture = NULL;

	this->Dx_DeferredTexture = NULL;
	this->Dx_DeferredQuadRT = NULL;
	this->Dx_DeferredSRV = NULL;

	this->framecount = 0;
	this->TriangleCount = 0;

	this->camera = cam;
	if(FAILED(this->Init()))
		MaloW::Debug("Failed to init DxManager");
}

DxManager::~DxManager()
{
	if(this->camera)
		delete this->camera;

	if(this->Shader_ForwardRendering)
		delete this->Shader_ForwardRendering;

	if(this->Shader_ShadowMap)
		delete this->Shader_ShadowMap;

	if(this->Shader_BillBoard)
		delete this->Shader_BillBoard;

	if(this->Shader_DeferredGeometry)
		delete this->Shader_DeferredGeometry;

	if(this->Shader_DeferredLightning)
		delete this->Shader_DeferredLightning;

	if(this->Shader_DeferredQuad)
		delete this->Shader_DeferredQuad;

	if(this->Shader_DeferredTexture)
		delete this->Shader_DeferredTexture;

	if(this->Dx_DeferredTexture)
		this->Dx_DeferredTexture->Release();
	if(this->Dx_DeferredQuadRT)
		this->Dx_DeferredQuadRT->Release();
	if(this->Dx_DeferredSRV)
		this->Dx_DeferredSRV->Release();

	if(this->Dx_DeviceContext)
		this->Dx_DeviceContext->Release();
	if(this->Dx_DepthStencilView)
		this->Dx_DepthStencilView->Release();
	if(this->Dx_DepthStencil)
		this->Dx_DepthStencil->Release();
	if(this->Dx_RenderTargetView)
		this->Dx_RenderTargetView->Release();
	if(this->Dx_SwapChain)
		this->Dx_SwapChain->Release();
	if(this->Dx_Device)
		this->Dx_Device->Release();

	for(int i = 0; i < NrOfRenderTargets; i++)
	{
		if(this->Dx_GbufferTextures[i])
			this->Dx_GbufferTextures[i]->Release();
		if(this->Dx_GbufferRTs[i])
			this->Dx_GbufferRTs[i]->Release();
		if(this->Dx_GbufferSRVs[i])
			this->Dx_GbufferSRVs[i]->Release();
	}

	while(0 < this->images.size())
		delete this->images.getAndRemove(0);

	while(0 < this->objects.size())
		delete this->objects.getAndRemove(0);

	while(0 < this->lights.size())
		delete this->lights.getAndRemove(0);
}

void DxManager::createObject(Mesh* mesh)
{
	MaloW::Array<MeshStrip*>* strips = mesh->GetStrips();

	for(int i = 0; i < strips->size(); i++)
	{
		MeshStrip* strip = strips->get(i);

		BUFFER_INIT_DESC bufferDesc;
		bufferDesc.ElementSize = sizeof(Vertex);
		bufferDesc.InitData = strip->getVerts();

	
		// Last face black, should +1 this to solve it.
		bufferDesc.NumElements = strip->getNrOfVerts();

		bufferDesc.Type = VERTEX_BUFFER;
		bufferDesc.Usage = BUFFER_DEFAULT;
	
		Buffer* verts = new Buffer();
		if(FAILED(verts->Init(Dx_Device, Dx_DeviceContext, bufferDesc)))
			MaloW::Debug("Initiate Buffer Failed in DxManager");

		Buffer* inds = NULL;
		if(strip->getIndicies())
		{
			BUFFER_INIT_DESC bufferInds;
			bufferInds.ElementSize = sizeof(int);
			bufferInds.InitData = strip->getIndicies();
			bufferInds.NumElements = strip->getNrOfIndicies();
			bufferInds.Type = INDEX_BUFFER;
			bufferInds.Usage = BUFFER_DEFAULT;
	
			inds = new Buffer();
			if(FAILED(inds->Init(Dx_Device, Dx_DeviceContext, bufferInds)))
				MaloW::Debug("CreateIndsBuffer Failed");
		}

		ID3D11ShaderResourceView* texture = NULL;
		if(strip->GetTexturePath() != "")
		{
			D3DX11_IMAGE_LOAD_INFO loadInfo;
			ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
			loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			loadInfo.Format = DXGI_FORMAT_BC1_UNORM;
			if(FAILED(D3DX11CreateShaderResourceViewFromFile(Dx_Device, strip->GetTexturePath().c_str(), &loadInfo, NULL, &texture, NULL)))
				MaloW::Debug("Failed to load texture " + strip->GetTexturePath());
		}

		Object3D* obj = new Object3D(verts, inds, texture, mesh->GetTopology()); 
		strip->SetRenderObject(obj);
	}

	mesh->RecreateWorldMatrix(); 
	
	RendererEvent* re = new RendererEvent("Add Mesh", mesh, NULL);
	this->PutEvent(re);
}

Object3D* DxManager::createParticleObject(ParticleMesh* mesh)
{
	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(ParticleVertex);
	bufferDesc.InitData = mesh->getVerts();

	
	// Last face black, should +1 this to solve it.
	bufferDesc.NumElements = mesh->getNrOfVerts();

	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;
	
	Buffer* verts = new Buffer();
	if(FAILED(verts->Init(Dx_Device, Dx_DeviceContext, bufferDesc)))
		MaloW::Debug("Initiate Buffer Failed in DxManager");

	Buffer* inds = NULL;

	ID3D11ShaderResourceView* texture = NULL;
	if(mesh->GetTexturePath() != "")
	{
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
		loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		loadInfo.Format = DXGI_FORMAT_BC1_UNORM;
		if(FAILED(D3DX11CreateShaderResourceViewFromFile(Dx_Device, mesh->GetTexturePath().c_str(), &loadInfo, NULL, &texture, NULL)))
			MaloW::Debug("Failed to load texture " + mesh->GetTexturePath());
	}


	Object3D* obj = new Object3D(verts, inds, texture, mesh->GetTopology());
	mesh->RecreateWorldMatrix(); 
	if(mesh->GetRenderObject())
		delete mesh->GetRenderObject();
	mesh->SetRenderObject(obj);

	return obj;
}

void DxManager::CreateSmokeEffect()
{
	RendererEvent* re = new RendererEvent("Create SmokeEffect", NULL, NULL);
	this->PutEvent(re);
}

void DxManager::deleteObject(Mesh* mesh)
{
	RendererEvent* re = new RendererEvent("Delete Mesh", mesh, NULL);
	this->PutEvent(re);
}

void DxManager::DeleteLight(Light* light)
{
	RendererEvent* re = new RendererEvent("Delete Light", NULL, light);
	this->PutEvent(re);
}

Light* DxManager::CreateLight(D3DXVECTOR3 pos)
{
	Light* light = new Light(pos);

	RendererEvent* re = new RendererEvent("Add Light", NULL, light);
	this->PutEvent(re);

	return light;
}

void DxManager::CreateImage(Image* image, string texture)
{
	ID3D11ShaderResourceView* text = NULL;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_BC1_UNORM;
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(this->Dx_Device, texture.c_str(), &loadInfo, NULL, &text, NULL)))
		MaloW::Debug("Failed to load texture " + texture);
	
	image->SetTexture(text);
	
	RendererEvent* re = new RendererEvent("Add Image", NULL, NULL, image);
	this->PutEvent(re);
}

void DxManager::DeleteImage(Image* image)
{
	RendererEvent* re = new RendererEvent("Delete Image", NULL, NULL, image);
	this->PutEvent(re);
}

void DxManager::DeleteText(Text* text)
{

}

void DxManager::CreateText(Text* text, string font)
{
	ID3D11ShaderResourceView* texture = NULL;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_BC1_UNORM;
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(this->Dx_Device, font.c_str(), &loadInfo, NULL, &texture, NULL)))
		MaloW::Debug("Failed to load texture " + font);
	
	Font newFont;
	newFont.texture = texture;
	font.replace(font.size() - 3, font.size(), "txt");

	// load char texture coords from .txt file to newFont.whatever, create shader for it and a renderer function. ->draw(text.size()); and expand every vertex to a quad. Use 2 int-arrays in shader to represent the string and the coords for every char.

	text->SetFont(newFont);
}