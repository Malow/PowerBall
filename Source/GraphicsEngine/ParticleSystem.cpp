//#include "ParticleSystem.h"
//
//
//HRESULT ParticleSystem::CreateRndTex(int size)
//{
//	HRESULT hr = S_OK;
//
//	D3DXVECTOR4* randomValues = new D3DXVECTOR4[size];
//	for(int i = 0; i < size; i++)
//	{
//		randomValues[i].x = RndFloat(-1.0f, 1.0f);
//		randomValues[i].y = RndFloat(-1.0f, 1.0f);
//		randomValues[i].z = RndFloat(-1.0f, 1.0f);
//		randomValues[i].w = RndFloat(-1.0f, 1.0f);
//	}
//	
//    D3D11_SUBRESOURCE_DATA subresData;
//    subresData.pSysMem = randomValues; //**derefferera?
//	subresData.SysMemPitch = sizeof(D3DXVECTOR4) * size;
//    subresData.SysMemSlicePitch = sizeof(D3DXVECTOR4) * size;
//	
//    D3D11_TEXTURE1D_DESC texDesc;
//    texDesc.Width = size;
//    texDesc.MipLevels = 1;
//    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
//    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//    texDesc.CPUAccessFlags = 0;
//    texDesc.MiscFlags = 0;
//    texDesc.ArraySize = 1;
//
//	ID3D11Texture1D* rndTex = NULL;
//	hr = this->gDevice->CreateTexture1D(&texDesc, &subresData, &rndTex);
//	if(FAILED(hr))
//	{
//		MessageBox(0, "Failed to create random texture", "CreateTexture1D", MB_ICONERROR);
//	}
//	
//    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
//	viewDesc.Format = texDesc.Format;
//    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
//    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
//	viewDesc.Texture1D.MostDetailedMip = 0;
//	
//	hr = this->gDevice->CreateShaderResourceView(rndTex, &viewDesc, &this->mRndTexSRV);
//	if(FAILED(hr))
//	{
//		MessageBox(0, "Failed to create shader resource view", "CreateShaderResourceView", MB_ICONERROR);
//	}
//
//	rndTex->Release();
//	delete [] randomValues;
//
//	return hr;
//}
//HRESULT ParticleSystem::CreateTexArray(const vector<string>& fileNames)
//{
//	HRESULT hr = S_OK;
//	//**parametrar**
//	/*
//	vector<wstring> flares;
//	flares.push_back(L"flare0.dds");					//v--wstring arrayName, const StringVector& filenames--v
//	ID3D10ShaderResourceView* texArray = GetTextureMgr().createTexArray(L"flares", flares); */
//
//
//	//
//	// Has this texture already been created?
//	//
//	/*
//	for(size_t i = 0; i < mTextureRVs.size(); ++i)
//		if( mTextureNames[i] == arrayName )
//			return mTextureRVs[i];*/
//
//
//	// Load the texture elements individually from file.  These textures
//	// won't be used by the GPU (0 bind flags), they are just used to 
//	// load the image data from file.  We use the STAGING usage so the
//	// CPU can read the resource.
//	//const StringVector& filenames;
//	//UINT arraySize = (UINT)fileNames.size();
//	this->mNrOfTextures = fileNames.size();
//
//	vector<ID3D11Texture2D*> srcTex(this->mNrOfTextures, 0);
//	for(UINT i = 0; i < this->mNrOfTextures; i++) 
//	{
//		D3DX11_IMAGE_LOAD_INFO loadInfo;
//
//        loadInfo.Width  = D3DX11_FROM_FILE;
//        loadInfo.Height = D3DX11_FROM_FILE;
//        loadInfo.Depth  = D3DX11_FROM_FILE;
//        loadInfo.FirstMipLevel = 0;
//        loadInfo.MipLevels = D3DX11_FROM_FILE;
//        loadInfo.Usage = D3D11_USAGE_STAGING;
//        loadInfo.BindFlags = 0;
//        loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
//        loadInfo.MiscFlags = 0;
//        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//        loadInfo.Filter = D3DX11_FILTER_NONE;
//        loadInfo.MipFilter = D3DX11_FILTER_NONE;
//		loadInfo.pSrcInfo  = 0;
//
//		hr = D3DX11CreateTextureFromFile(this->gDevice, fileNames[i].c_str(), &loadInfo, 0, (ID3D11Resource**)&srcTex[i], 0);
//		if(FAILED(hr))
//		{
//			MessageBox(0, "Couldn't load particle texture(s)", "Create texture from file error", MB_ICONERROR); //**nummer**
//			return hr;
//		}
//	}
//
//	// Create the texture array.  Each element in the texture 
//	// array has the same format/dimensions.
//	D3D11_TEXTURE2D_DESC texElementDesc;
//	srcTex[0]->GetDesc(&texElementDesc);
//
//	D3D11_TEXTURE2D_DESC texArrayDesc;
//	texArrayDesc.Width              = texElementDesc.Width;
//	texArrayDesc.Height             = texElementDesc.Height;
//	texArrayDesc.MipLevels          = texElementDesc.MipLevels;
//	texArrayDesc.ArraySize          = this->mNrOfTextures; //nr of texture elements to store
//	texArrayDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
//	texArrayDesc.SampleDesc.Count   = 1;
//	texArrayDesc.SampleDesc.Quality = 0;
//	texArrayDesc.Usage              = D3D11_USAGE_DEFAULT;
//	texArrayDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
//	texArrayDesc.CPUAccessFlags     = 0;
//	texArrayDesc.MiscFlags          = 0;
//
//	ID3D11Texture2D* texArray = 0;
//	hr = this->gDevice->CreateTexture2D( &texArrayDesc, 0, &texArray);
//	if(FAILED(hr))
//	{
//		MessageBox(0, "Couldn't create particle texture(s)", "Create texture2d error", MB_ICONERROR);
//		return hr;
//	}
//
//	//**mapping är devicecontext**
//	/*
//	// Copy individual texture elements into texture array.
//	// for each texture element...
//	for(UINT i = 0; i < this->mNrOfTextures; i++)
//	{
//		// for each mipmap level...
//		for(UINT j = 0; j < texElementDesc.MipLevels; ++j)
//		{
//			D3D10_MAPPED_TEXTURE2D mappedTex2D;
//			srcTex[i]->Map(j, D3D10_MAP_READ, 0, &mappedTex2D);
//                    
//            this->gDevice->UpdateSubresource(texArray, 
//				D3D10CalcSubresource(j, i, texElementDesc.MipLevels),
//                0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);
//
//            srcTex[i]->Unmap(j);
//		}
//	}*/	
//
//	// Create a resource view to the texture array.
//	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
//	viewDesc.Format = texArrayDesc.Format;
//	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
//	viewDesc.Texture2DArray.MostDetailedMip = 0;
//	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
//	viewDesc.Texture2DArray.FirstArraySlice = 0;
//	viewDesc.Texture2DArray.ArraySize = this->mNrOfTextures;
//
//	//ID3D10ShaderResourceView* texArrayRV = 0;
//	hr = this->gDevice->CreateShaderResourceView(texArray, &viewDesc, &this->mTexArraySRV);
//
//	// Cleanup--we only need the resource view.
//	//ReleaseCOM(texArray);
//
//	for(UINT i = 0; i < this->mNrOfTextures; i++)
//	{
//		if(srcTex[i])
//		{
//			srcTex[i]->Release();
//		}
//	}
//
//	//mTextureNames.push_back(arrayName); //arrayname - parameter
//	//mTextureRVs.push_back(texArrayRV); //medlemsvariabel
//
//	//return texArrayRV;
//
//
//	return hr;
//}
//HRESULT ParticleSystem::BuildBuffers()
//{
//	HRESULT hr = S_OK;
//
//	//initVB
//    D3D11_BUFFER_DESC bufferDesc;
//    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    bufferDesc.ByteWidth = sizeof(Particle); 
//    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.CPUAccessFlags = 0;
//    bufferDesc.MiscFlags = 0;
//	//only attributes/data applying for an emitter particle is type and age
//	Particle particle;
//	ZeroMemory(&particle, sizeof(Particle));
//	particle.type = 0; 
//	particle.age  = 0.0f;
//    D3D11_SUBRESOURCE_DATA subresData;
//    subresData.pSysMem = &particle;
//	hr = this->gDevice->CreateBuffer(&bufferDesc, &subresData, &this->mInitVB);
//	if(FAILED(hr))
//	{
//		MessageBox(0, "Failed to create initVB", "Create Vertex buffer error", MB_ICONERROR);
//	}
//	
//	//create buffers for drawing and stream out
//	bufferDesc.ByteWidth = sizeof(Particle) * this->mCapacity; //size of the buffers must match
//    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT; //buffers will be used as both vertex buffer and stream output
//	//drawVB
//    hr = this->gDevice->CreateBuffer(&bufferDesc, 0, &this->mDrawVB);
//	if(FAILED(hr))
//	{
//		MessageBox(0, "Failed to create drawVB", "Create Vertex buffer error", MB_ICONERROR);
//	}
//	//streamoutVB
//	hr = this->gDevice->CreateBuffer(&bufferDesc, 0, &mStreamOutVB);
//	if(FAILED(hr))
//	{
//		MessageBox(0, "Failed to create streamoutVB", "Create Vertex buffer error", MB_ICONERROR);
//	}
//
//	return hr;
//}
////**copy-konst**
////**tilldenings-operator**
//
//ParticleSystem::ParticleSystem(int capacity)
//{
//	this->gDevice = NULL;
//	this->gDeviceContext = NULL;
//	this->gParticleShader = NULL;
//
//	this->mCapacity = capacity;
//	this->mFirstRun = true;
//	this->mDeltaTime = 0.0f;
//	this->mGameTime = 0.0f;
//	this->mAge = 0.0f;
//	this->mEyePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
//	this->mEmitterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
//	this->mEmitterDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f); //direction can't be nullvector
//		
//	this->mInitVB = NULL;
//	this->mDrawVB = NULL; 
//	this->mStreamOutVB = NULL; 
//	this->mRndTexSRV = NULL; 
//	this->mTexArraySRV = NULL; 
//} 
//ParticleSystem::ParticleSystem(int capacity, D3DXVECTOR3 emitterPos, D3DXVECTOR3 emitterDir)
//{
//	this->gDevice = NULL;
//	this->gDeviceContext = NULL;
//	this->gParticleShader = NULL;
//
//	this->mCapacity = capacity;
//	this->mFirstRun = true;
//	this->mDeltaTime = 0.0f;
//	this->mGameTime = 0.0f;
//	this->mAge = 0.0f;
//	this->mEyePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
//	this->mEmitterPos = emitterPos;  //**const &**
//	this->mEmitterDir = emitterDir; //direction can't be nullvector //**const &**
//		
//	this->mInitVB = NULL;
//	this->mDrawVB = NULL; 
//	this->mStreamOutVB = NULL; 
//	this->mRndTexSRV = NULL; 
//	this->mTexArraySRV = NULL; 
//} 
//ParticleSystem::~ParticleSystem()
//{
//	if(this->mInitVB) this->mInitVB->Release();
//	if(this->mDrawVB) this->mDrawVB->Release(); 
//	if(this->mStreamOutVB) this->mStreamOutVB->Release(); 
//	if(this->mRndTexSRV) this->mRndTexSRV->Release(); 
//	if(this->mTexArraySRV) this->mTexArraySRV->Release(); 
//}
//
//
//float ParticleSystem::GetAge() const
//{
//	return this->mAge;
//}
//
//void ParticleSystem::SetEyePos(const D3DXVECTOR3& eyePos)
//{
//	//this->mEyePos = eyePos;
//	this->mEyePos = D3DXVECTOR3(eyePos.x, eyePos.y, eyePos.z);//**
//}
//void ParticleSystem::SetEmitterPos(const D3DXVECTOR3& emitterPos)
//{
//	this->mEmitterPos = D3DXVECTOR3(emitterPos.x, emitterPos.y, emitterPos.z); //**
//}
//void ParticleSystem::SetEmitterDir(const D3DXVECTOR3& emitterDir)
//{
//	this->mEmitterDir = D3DXVECTOR3(emitterDir.x, emitterDir.y, emitterDir.z);//**
//}
//
//HRESULT ParticleSystem::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParticleShader* particleShader, const vector<string>& fileNames)
//{
//	HRESULT hr = S_OK;
//
//	this->gDevice = device;
//	this->gDeviceContext = deviceContext;
//	this->gParticleShader = particleShader;
//
//	hr = ParticleSystem::CreateRndTex(1024); //**this->mRndTexSRV** - texturemanager**
//	if(FAILED(hr)) return hr;
//	
//	hr = ParticleSystem::CreateTexArray(fileNames); //**this->mTexArraySRV**  - texturemanager**
//	if(FAILED(hr)) return hr;
//
//	hr = ParticleSystem::BuildBuffers();
//	if(FAILED(hr)) return hr;
//
//	return hr;
//}
//void ParticleSystem::Reset()
//{
//	this->mFirstRun = true;
//	this->mAge = 0.0f;
//}
//
//void ParticleSystem::Move(float deltaTime)
//{
//	this->mEmitterPos += this->mEmitterDir * deltaTime;
//}
//void ParticleSystem::Update(float deltaTime, float gameTime)
//{
//	this->mDeltaTime = deltaTime;
//	this->mGameTime = gameTime;
//	this->mAge += deltaTime;
//}
///*
//void ParticleSystem::Draw(Camera* camera) 
//{
//	//set effect-variables
//	D3DXMATRIX view = camera->GetViewMatrix();//**"global"**
//	D3DXMATRIX proj = camera->GetProjMatrix();//**"global"**
//	//global
//	this->gParticleShader->SetInt("gNrOfTextures", this->mNrOfTextures);
//	this->gParticleShader->SetResource("gRndTex", this->mRndTexSRV); 
//	this->gParticleShader->SetResource("gTexArray", this->mTexArraySRV);
//	//constant buffer (per frame)		//**funktion, eller nedanstående?**
//	//this->gParticleShader->GetEffect()->GetConstantBufferByName("PerFrame")->GetMemberByName("pf_GameTime")->AsScalar()->SetFloat(this->mGameTime); 
//	this->gParticleShader->SetFloat("pf_GameTime", this->mGameTime);
//	this->gParticleShader->SetFloat("pf_DeltaTime", this->mDeltaTime);
//	this->gParticleShader->SetFloat3("pf_EyePosW", this->mEyePos);
//	this->gParticleShader->SetFloat3("pf_EmitterPosW", this->mEmitterPos);
//	this->gParticleShader->SetFloat3("pf_EmitterDirW", this->mEmitterDir);
//	this->gParticleShader->SetMatrix("pf_ViewProj", view * proj); 
//
//	//set input assembler
//	this->gDevice->IASetInputLayout(this->gParticleShader->GetInputLayout()); //**med APply(technique, pass??)**
//	this->gDevice->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//	UINT stride = sizeof(Particle);
//	UINT offset = 0;
//	//on first pass, use the initialization vertex buffer
//	if(this->mFirstRun)
//	{
//		this->gDevice->IASetVertexBuffers(0, 1, &this->mInitVB, &stride, &offset);
//	}
//	else //else use the vertex buffer containing the particles
//	{
//		this->gDevice->IASetVertexBuffers(0, 1, &this->mDrawVB, &stride, &offset);
//	}
//	
//	//draw:
//	//stream output technique
//	//bind stream output buffer to the stream output stage to update the particles
//	this->gDevice->SOSetTargets(1, &this->mStreamOutVB, &offset); 
//	D3D11_TECHNIQUE_DESC techDesc;
//	this->gParticleShader->GetStreamOutputEffectTechnique()->GetDesc(&techDesc); //**
//	for(int i = 0; i < (int)techDesc.Passes; i++)
//	{
//		this->gParticleShader->GetStreamOutputEffectTechnique()->GetPassByIndex(i)->Apply(0); //**
//		//this->gParticleShader->Apply(0, i);//**
//		if(this->mFirstRun) //on first, pass draw the emitter particle
//		{
//			this->gDevice->Draw(1, 0);
//			this->mFirstRun = false;
//		}
//		else
//		{
//			this->gDevice->DrawAuto(); 
//		}
//	}
//	//unbind buffer
//	ID3D11Buffer* bufferArray[1] = {NULL};
//	this->gDevice->SOSetTargets(1, bufferArray, &offset);
//
//	//swap draw & streamout buffers (like front & back buffer)
//	std::swap(this->mDrawVB, this->mStreamOutVB);
//	
//
//	//draw technique
//	//bind draw buffer to draw the updated particles
//	this->gDevice->IASetVertexBuffers(0, 1, &this->mDrawVB, &stride, &offset);
//	this->gParticleShader->GetDrawEffectTechnique()->GetDesc(&techDesc); //**
//	for(int i = 0; i < (int)techDesc.Passes; i++)
//	{
//		this->gParticleShader->GetDrawEffectTechnique()->GetPassByIndex(i)->Apply(0); //**
//		//this->gParticleShader->Apply(1, i);//**
//		this->gDevice->DrawAuto();
//	}
//}*/