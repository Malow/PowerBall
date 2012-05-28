#include "DxManager.h"

DxManager::DxManager(HWND g_hWnd, GraphicsEngineParams params, Camera* cam)
{
	this->StartRender = false;
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
	this->Shader_Text = NULL;
	this->Shader_ShadowMapAnimated = NULL;

	this->Shader_BillBoard = NULL;

	for(int i = 0; i < NrOfRenderTargets; i++)
	{
		this->Dx_GbufferTextures[i] = NULL;
		this->Dx_GbufferRTs[i] = NULL;
		this->Dx_GbufferSRVs[i] = NULL;
	}

	this->Shader_DeferredGeometry = NULL;
	this->Shader_DeferredLightning = NULL;
	this->Shader_InvisibilityEffect = NULL;

	this->Shader_DeferredQuad = NULL;
	this->Shader_DeferredTexture = NULL;
	this->Shader_DeferredAnimatedGeometry = NULL;

	this->Dx_DeferredTexture = NULL;
	this->Dx_DeferredQuadRT = NULL;
	this->Dx_DeferredSRV = NULL;
	this->LavaTexture = NULL;
	this->skybox = NULL;
	this->Shader_Skybox = NULL;

	this->ssao = NULL;
	this->fxaa = NULL;

	this->framecount = 0;
	this->TriangleCount = 0;

	this->camera = cam;
	if(FAILED(this->Init()))
		MaloW::Debug("Failed to init DxManager");

	this->TimerAnimation = 0.0f;
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		MaloW::Debug("QueryPerformanceFrequency Failed!, High resolution performance counter not available?");

	this->PCFreq = float(li.QuadPart)/1000.0f;
	QueryPerformanceCounter(&li);
	this->prevTimeStamp = li.QuadPart;
}

DxManager::~DxManager()
{
	if(this->camera)
		delete this->camera;

	if(this->Shader_ForwardRendering)
		delete this->Shader_ForwardRendering;

	if(this->Shader_ShadowMap)
		delete this->Shader_ShadowMap;

	SAFE_DELETE(this->Shader_ShadowMapAnimated);

	if(this->Shader_BillBoard)
		delete this->Shader_BillBoard;

	if(this->Shader_DeferredGeometry)
		delete this->Shader_DeferredGeometry;

	if(this->Shader_DeferredLightning)
		delete this->Shader_DeferredLightning;

	SAFE_DELETE(this->Shader_InvisibilityEffect);

	if(this->Shader_DeferredQuad)
		delete this->Shader_DeferredQuad;

	if(this->Shader_DeferredTexture)
		delete this->Shader_DeferredTexture;

	if(this->Shader_DeferredAnimatedGeometry)
		delete this->Shader_DeferredAnimatedGeometry;

	if(this->Dx_DeferredTexture)
		this->Dx_DeferredTexture->Release();
	if(this->Dx_DeferredQuadRT)
		this->Dx_DeferredQuadRT->Release();
	if(this->Dx_DeferredSRV)
		this->Dx_DeferredSRV->Release();

	if(this->LavaTexture)
		this->LavaTexture->Release();

	SAFE_DELETE(this->skybox);

	if(this->Shader_Skybox)
		delete this->Shader_Skybox;

	if(this->Shader_Text)
		delete this->Shader_Text;

	SAFE_DELETE(this->ssao);
	SAFE_DELETE(this->fxaa);
	SAFE_DELETE(this->Shader_Fxaa);

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

	while(0 < this->animations.size())
		delete this->animations.getAndRemove(0);

	while(0 < this->lights.size())
		delete this->lights.getAndRemove(0);

	while(0 < this->texts.size())
		delete this->texts.getAndRemove(0);
}

void DxManager::CreateStaticMesh(StaticMesh* mesh)
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
	
	MeshEvent* re = new MeshEvent("Add Mesh", mesh, NULL);
	this->PutEvent(re);
}

void DxManager::CreateAnimatedMesh(AnimatedMesh* mesh)
{
	MaloW::Array<KeyFrame*>* kfs = mesh->GetKeyFrames();
	
	for(int j = 0; j < kfs->size(); j++)
	{

		MaloW::Array<MeshStrip*>* strips = kfs->get(j)->strips;

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
	}

	mesh->RecreateWorldMatrix(); 
	
	
	MeshEvent* re = new MeshEvent("Add AniMesh", NULL, mesh);
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
		loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
	RendererEvent* re = new RendererEvent("Create SmokeEffect");
	this->PutEvent(re);
}

void DxManager::DeleteStaticMesh(StaticMesh* mesh)
{
	MeshEvent* re = new MeshEvent("Delete Mesh", mesh, NULL);
	this->PutEvent(re);
}

void DxManager::DeleteAnimatedMesh(AnimatedMesh* mesh)
{
	MeshEvent* re = new MeshEvent("Delete AniMesh", NULL, mesh);
	this->PutEvent(re);
}

void DxManager::DeleteLight(Light* light)
{
	LightEvent* re = new LightEvent("Delete Light", light);
	this->PutEvent(re);
}

Light* DxManager::CreateLight(D3DXVECTOR3 pos, bool UseShadowMap)
{
	Light* light = new Light(pos);

	if(UseShadowMap)
	{
		LightEvent* re = new LightEvent("Add Light with shadows", light);
		this->PutEvent(re);
	}
	else
	{
		LightEvent* re = new LightEvent("Add Light", light);
		this->PutEvent(re);
	}

	return light;
}

void DxManager::CreateImage(Image* image, string texture)
{
	ID3D11ShaderResourceView* text = NULL;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(this->Dx_Device, texture.c_str(), &loadInfo, NULL, &text, NULL)))
		MaloW::Debug("Failed to load texture " + texture);
	
	image->SetTexture(text);
	
	ImageEvent* re = new ImageEvent("Add Image", image);
	this->PutEvent(re);
}

void DxManager::DeleteImage(Image* image)
{
	ImageEvent* re = new ImageEvent("Delete Image", image);
	this->PutEvent(re);
}

void DxManager::DeleteText(Text* text)
{
	TextEvent* te = new TextEvent("Delete Text", text);
	this->PutEvent(te);
}

void DxManager::CreateText(Text* text, string font)
{
	ID3D11ShaderResourceView* texture = NULL;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(this->Dx_Device, (font + ".png").c_str(), &loadInfo, NULL, &texture, NULL)))
		MaloW::Debug("Failed to load texture " + font);
	
	Font* newFont = text->GetFont();
	newFont->texture = texture;

	/* Font .txt structure:
	char in int
	chartex
	charwidth

	char in int
	chartex
	charwidth

	example:
	91
	55
	10

	92
	65
	8

	*/


	font += ".txt";
	ifstream file;
	file.open(font);
	while(!file.eof())
	{
		string line = "";

		getline(file, line);
		int character = atoi(line.c_str());

		getline(file, line);
		int texpos = atoi(line.c_str());

		getline(file, line);
		int width = atoi(line.c_str());
		width = width - texpos;

		newFont->charTexCoords[character] = texpos;
		newFont->charWidth[character] = width;
		getline(file, line);
	}
	file.close();

	TextEvent* te = new TextEvent("Add Text", text);
	this->PutEvent(te);
}

void DxManager::CreateSkyBox(string texture)
{
	if(this->skybox)
		delete this->skybox;
		
	SkyBox* sb = new SkyBox(this->camera->getPosition(), 10, 10);
	MeshStrip* strip = sb->GetStrips()->get(0);

	// Create the desc for the buffer
	BUFFER_INIT_DESC BufferDesc;
	BufferDesc.ElementSize = sizeof(Vertex);
	BufferDesc.InitData = strip->getVerts();
	BufferDesc.NumElements = strip->getNrOfVerts();
	BufferDesc.Type = VERTEX_BUFFER;
	BufferDesc.Usage = BUFFER_DEFAULT;

	// Create the buffer
	Buffer* VertexBuffer = new Buffer();
	if(FAILED(VertexBuffer->Init(this->Dx_Device, this->Dx_DeviceContext, BufferDesc)))
		MaloW::Debug("Failed to init skybox");



	BUFFER_INIT_DESC indiceBufferDesc;
	indiceBufferDesc.ElementSize = sizeof(int);
	indiceBufferDesc.InitData = strip->getIndicies();
	indiceBufferDesc.NumElements = strip->getNrOfIndicies();
	indiceBufferDesc.Type = INDEX_BUFFER;
	indiceBufferDesc.Usage = BUFFER_DEFAULT;

	Buffer* IndexBuffer = new Buffer();

	if(FAILED(IndexBuffer->Init(this->Dx_Device, this->Dx_DeviceContext, indiceBufferDesc)))
		MaloW::Debug("Failed to init skybox");

	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* SMTexture = 0;
	D3DX11CreateTextureFromFile(this->Dx_Device, texture.c_str(), 
		&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);


	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;
	ID3D11ShaderResourceView* text;
	this->Dx_Device->CreateShaderResourceView(SMTexture, &SMViewDesc, &text);

	SMTexture->Release();

	Object3D* ro = new Object3D(VertexBuffer, IndexBuffer, text, sb->GetTopology());
	strip->SetRenderObject(ro);

	this->skybox = sb;
}

float DxManager::GetLavaHeightAt(float x, float z) 
{ 
	float L = 10.0f;
	float H = 10.0f;

	D3DXVECTOR2 xzVec = D3DXVECTOR2(x, z);
	float len = D3DXVec2Length(&xzVec); //length of xz-vector from origin to pixel in world space
	float scale = this->LavaWavesOuterRadius / H; //outer radius of which to start decreasing the wave height, scale is used to scale the result between 0 and H.
	if(len < this->LavaWavesOuterRadius)
	{	
		H -= (this->LavaWavesOuterRadius - len) / scale;
	}

	float timer = this->TimerAnimation * 0.001f;

	float bias = sin(timer * 0.2f) * sin(x * 0.2f) * sin(z * 0.2f);
	D3DXVECTOR3 temp = this->camera->getPosition() - D3DXVECTOR3(x, L, z);
	bias *= 1.2f - (D3DXVec3Length(&temp) / 100.0f);

	float lavaHeight = L + bias;
	return lavaHeight;
}