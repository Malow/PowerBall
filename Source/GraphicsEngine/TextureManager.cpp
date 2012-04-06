#include "TextureManager.h"

//private
TextureManager* TextureManager::mInstance = NULL;

TextureManager::TextureManager()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;

	this->mInstance = NULL;
	this->mNrOfTextures = 0;
}
TextureManager::~TextureManager()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;

	this->mInstance = NULL;

	for(UINT i = 0; i < this->mNrOfTextures; i++)
	{
		SAFE_RELEASE(this->mTextureSRVs[i]);
	}
}


ID3D11ShaderResourceView* TextureManager::CreateRndTex2(UINT size, float min, float max)
{
	HRESULT hr = S_OK;

	D3DXVECTOR2* randomValues = new D3DXVECTOR2[size];
	for(UINT i = 0; i < size; i++)
	{
		randomValues[i].x = RndFloat(min, max);
		randomValues[i].y = RndFloat(min, max);
	}
	
    D3D11_SUBRESOURCE_DATA subresData;
    subresData.pSysMem = randomValues; 
	subresData.SysMemPitch = sizeof(D3DXVECTOR2) * size;
    subresData.SysMemSlicePitch = sizeof(D3DXVECTOR2) * size;
	
    D3D11_TEXTURE1D_DESC texDesc;
    texDesc.Width = size;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	ID3D11Texture1D* rndTex = NULL;
	hr = this->gDevice->CreateTexture1D(&texDesc, &subresData, &rndTex);
	if(FAILED(hr)) return NULL;
	
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	
	ID3D11ShaderResourceView* rndTexSRV = NULL;
	hr = this->gDevice->CreateShaderResourceView(rndTex, &viewDesc, &rndTexSRV);
	if(FAILED(hr)) return NULL;

	rndTex->Release();
	delete [] randomValues;
	
	return rndTexSRV;
}
ID3D11ShaderResourceView* TextureManager::CreateRndTex3(UINT size, float min, float max)
{
	HRESULT hr = S_OK;

	D3DXVECTOR3* randomValues = new D3DXVECTOR3[size];
	for(UINT i = 0; i < size; i++)
	{
		randomValues[i].x = RndFloat(min, max);
		randomValues[i].y = RndFloat(min, max);
		randomValues[i].z = RndFloat(min, max);
	}
	
    D3D11_SUBRESOURCE_DATA subresData;
    subresData.pSysMem = randomValues; 
	subresData.SysMemPitch = sizeof(D3DXVECTOR3) * size;
    subresData.SysMemSlicePitch = sizeof(D3DXVECTOR3) * size;
	
    D3D11_TEXTURE1D_DESC texDesc;
    texDesc.Width = size;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	ID3D11Texture1D* rndTex = NULL;
	hr = this->gDevice->CreateTexture1D(&texDesc, &subresData, &rndTex);
	if(FAILED(hr)) return NULL;
	
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	
	ID3D11ShaderResourceView* rndTexSRV = NULL;
	hr = this->gDevice->CreateShaderResourceView(rndTex, &viewDesc, &rndTexSRV);
	if(FAILED(hr)) return NULL;

	rndTex->Release();
	delete [] randomValues;
	
	return rndTexSRV;
}
ID3D11ShaderResourceView* TextureManager::CreateRndTex4(UINT size, float min, float max)
{
	HRESULT hr = S_OK;

	D3DXVECTOR4* randomValues = new D3DXVECTOR4[size];
	for(UINT i = 0; i < size; i++)
	{
		randomValues[i].x = RndFloat(min, max);
		randomValues[i].y = RndFloat(min, max);
		randomValues[i].z = RndFloat(min, max);
		randomValues[i].w = RndFloat(min, max);
	}
	
    D3D11_TEXTURE1D_DESC texDesc;
    texDesc.Width = size;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;
	
    D3D11_SUBRESOURCE_DATA subresData;
    subresData.pSysMem = randomValues; 
	subresData.SysMemPitch = sizeof(D3DXVECTOR4) * size;
    subresData.SysMemSlicePitch = sizeof(D3DXVECTOR4) * size;

	ID3D11Texture1D* rndTex = NULL;
	hr = this->gDevice->CreateTexture1D(&texDesc, &subresData, &rndTex);
	if(FAILED(hr)) return NULL;
	
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	
	ID3D11ShaderResourceView* rndTexSRV = NULL;
	hr = this->gDevice->CreateShaderResourceView(rndTex, &viewDesc, &rndTexSRV);
	if(FAILED(hr)) return NULL;

	rndTex->Release();
	delete [] randomValues;
	
	return rndTexSRV;
}

//public
void TextureManager::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->gDevice = device;
	this->gDeviceContext = deviceContext;
}
TextureManager* TextureManager::GetInstance()
{
	if(!mInstance)
	{
		mInstance = new TextureManager();
	}
	return mInstance;
}
void TextureManager::Cleanup()
{
	this->~TextureManager();
}
		
ID3D11ShaderResourceView* TextureManager::CreateRndTex(UINT size, UINT dimension, float min, float max)
{
	ID3D11ShaderResourceView* rndTexSRV = NULL;

	switch(dimension)
	{
		case 2: rndTexSRV = CreateRndTex2(size, min, max); break;
		case 3: rndTexSRV = CreateRndTex3(size, min, max); break; 
		case 4: rndTexSRV = CreateRndTex4(size, min, max); break;
		default: MaloW::Debug("Dimension of vector is outside the range of 2-4"); break;
	}
	
	return rndTexSRV;
}

