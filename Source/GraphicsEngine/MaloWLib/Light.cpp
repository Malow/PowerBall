#include "Light.h"

using namespace MaloW;

Light::Light()
{
	this->pos = D3DXVECTOR3(0, 0, 0);
	this->type = POINT_LIGHT;
	this->UseShadowMap = false;
	this->intensity = 10.0f;

		///////// Change these
		this->lookAt = D3DXVECTOR3(1, 0, 0);
		this->up = D3DXVECTOR3(0, 1, 0);

	for(int i = 0; i < 5; i++)
	{
		this->shadowMap[i] = NULL;
		this->shadowMapDepthView[i] = NULL;
		this->shadowMapSRView[i] = NULL;
	}
}

Light::Light(D3DXVECTOR3 pos, LightType type)
{
	this->pos = pos;
	this->type = type;
	this->UseShadowMap = false;
	this->intensity = 10.0f;

		///////// Change these
		this->lookAt = D3DXVECTOR3(1, 0, 0);
		this->up = D3DXVECTOR3(0, 1, 0);

	for(int i = 0; i < 5; i++)
	{
		this->shadowMap[i] = NULL;
		this->shadowMapDepthView[i] = NULL;
		this->shadowMapSRView[i] = NULL;
	}
}

Light::~Light() 
{ 
	for(int i = 0; i < 5; i++)
	{
		if(this->shadowMapSRView[i])
			this->shadowMapSRView[i]->Release();
		if(this->shadowMap[i])
			this->shadowMap[i]->Release();
		if(this->shadowMapDepthView[i])
			this->shadowMapDepthView[i]->Release();
	}
}

HRESULT Light::InitShadowMap(ID3D11Device* g_Device)
{
	this->UseShadowMap = true;
	// Shadow Maps
	//create the depth stencil
	for(int i = 0; i < 5; i++)
	{
		int width = (int)(256 * pow(2.0f, i));
		int height = (int)(256 * pow(2.0f, i));
		// Shadow Maps
		//create the depth stencil
		D3D11_TEXTURE2D_DESC shadowMapDescDepth;
		ZeroMemory(&shadowMapDescDepth, sizeof(shadowMapDescDepth));
		shadowMapDescDepth.Width = width;
		shadowMapDescDepth.Height = height;
		shadowMapDescDepth.MipLevels = 1;
		shadowMapDescDepth.ArraySize = 1;
		shadowMapDescDepth.Format = DXGI_FORMAT_R32_TYPELESS;
		shadowMapDescDepth.SampleDesc.Count = 1;
		shadowMapDescDepth.SampleDesc.Quality = 0;
		shadowMapDescDepth.Usage = D3D11_USAGE_DEFAULT;
		shadowMapDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		shadowMapDescDepth.CPUAccessFlags = 0;
		shadowMapDescDepth.MiscFlags = 0;
	
		if(FAILED(g_Device->CreateTexture2D(&shadowMapDescDepth, NULL, &shadowMap[i])))
			return E_FAIL;


		D3D11_DEPTH_STENCIL_VIEW_DESC descDSVShadowMap;
		ZeroMemory(&descDSVShadowMap, sizeof(descDSVShadowMap));
		descDSVShadowMap.Format = DXGI_FORMAT_D32_FLOAT;
		descDSVShadowMap.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSVShadowMap.Texture2D.MipSlice = 0;

		if(FAILED(g_Device->CreateDepthStencilView(shadowMap[i], &descDSVShadowMap, &shadowMapDepthView[i])))
			return E_FAIL;

		
		// Create the shader-resource view from the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
		ZeroMemory(&srDesc, sizeof(srDesc));
		srDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srDesc.Texture2D.MostDetailedMip = 0;
		srDesc.Texture2D.MipLevels = 1;
	
		if(FAILED(g_Device->CreateShaderResourceView(shadowMap[i], &srDesc, &shadowMapSRView[i])))
			return E_FAIL;


		ShadowMapViewPort[i].Width = (float)width;
		ShadowMapViewPort[i].Height = (float)height;
		ShadowMapViewPort[i].MinDepth = 0.0f;
		ShadowMapViewPort[i].MaxDepth = 1.0f;
		ShadowMapViewPort[i].TopLeftX = 0;
		ShadowMapViewPort[i].TopLeftY = 0;	
	}

	return S_OK;
}

D3DXMATRIX Light::GetViewProjMatrix()
{ 
	// Recreate LightViewProj
	D3DXMATRIX LightView;
	D3DXMatrixLookAtLH(&LightView, &this->pos, &this->lookAt, &this->up);
	D3DXMATRIX LightProj;
	D3DXMatrixPerspectiveFovLH(&LightProj, (float)D3DX_PI * 0.35f, 1, 5.0f, 100.0f);	//////////////////////
	this->ViewProj = LightView * LightProj;
	return this->ViewProj; 
}