#include "FXAA.h"

FXAA::FXAA()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;
	this->gSwapChain = NULL;

	this->mPreset = 0;
}
FXAA::~FXAA()
{

}
void FXAA::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain)
{
	this->gDevice = device;
	this->gDeviceContext = deviceContext;
	this->gSwapChain = swapChain;
}

UINT FXAA::GetPreset() const
{
	return this->mPreset;
}

void FXAA::SetPreset(UINT preset)
{
	if(preset > -1 && preset < 7)
	{
		this->mPreset = preset;
	}
	else
	{
		this->mPreset = 0;
	}
}

void FXAA::PreRender(Shader* shader, GraphicsEngineParams engParams)
{
	HRESULT hr = S_OK;

	//get the surface/texture from the swap chain
	ID3D11Texture2D* backBufferTex;
	this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTex);
	
	//get texture description
	D3D11_TEXTURE2D_DESC texDesc;
	backBufferTex->GetDesc(&texDesc);
	
	//change bindflag for use as a shader resource
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; 

	//create texture
	ID3D11Texture2D* sceneTex;
	hr = this->gDevice->CreateTexture2D(&texDesc, NULL, &sceneTex);	
	if(FAILED(hr))
	{
		MaloW::Debug("FXAA: Failed to create texture");
	}
	
	//get resource from the render target view of the backbuffer
	ID3D11RenderTargetView* backBufferRTV = NULL;
	this->gDeviceContext->OMGetRenderTargets(1, &backBufferRTV, NULL);
	ID3D11Resource* backBufferRTVResource = NULL;
	backBufferRTV->GetResource(&backBufferRTVResource);

	//copy data from the resource to the scene texture
	this->gDeviceContext->CopyResource(sceneTex, backBufferRTVResource);

	//create shader resource view
	ID3D11ShaderResourceView* sceneSRV;
	hr = this->gDevice->CreateShaderResourceView(sceneTex, NULL, &sceneSRV);
	if(FAILED(hr))
	{
		MaloW::Debug("FXAA: Failed to create shader resource view");
	}

	//set shader variables
	shader->SetResource("sceneTex", sceneSRV);
	shader->SetInt("FXAA_PRESET", this->mPreset);
	shader->SetFloat4("rcpFrame", D3DXVECTOR4(1.0f / engParams.windowWidth, 1.0f / engParams.windowHeight, 0.0f, 0.0f));
	

	//release **** 
	SAFE_RELEASE(backBufferTex); 
	SAFE_RELEASE(sceneTex);
	SAFE_RELEASE(backBufferRTV);
	SAFE_RELEASE(backBufferRTVResource);
	SAFE_RELEASE(sceneSRV);
}
void FXAA::PostRender(Shader* shader)
{
	shader->SetResource("sceneTex", NULL);
}
