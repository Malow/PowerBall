#include "TextureManager.h"

TextureManager::TextureManager()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;
}
TextureManager::~TextureManager()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;
}


//public
void TextureManager::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->gDevice = device;
	this->gDeviceContext = deviceContext;
	
	srand((unsigned int)(time(NULL)));
}


ID3D11ShaderResourceView* TextureManager::CreateRndTex1DVec3(UINT size, float min, float max)
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



ID3D11ShaderResourceView* TextureManager::CreateRndTex2DVec3(UINT width, UINT height, float min, float max)
{
	HRESULT hr = S_OK;

	UINT size = width * height;
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
	
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = size;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	ID3D11Texture2D* rndTex = NULL;
	hr = this->gDevice->CreateTexture2D(&texDesc, &subresData, &rndTex);
	if(FAILED(hr)) return NULL;
	
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture2D.MostDetailedMip = 0;
	
	ID3D11ShaderResourceView* rndTexSRV = NULL;
	hr = this->gDevice->CreateShaderResourceView(rndTex, &viewDesc, &rndTexSRV);
	if(FAILED(hr)) return NULL;

	rndTex->Release();
	delete [] randomValues;
	
	return rndTexSRV;
}
