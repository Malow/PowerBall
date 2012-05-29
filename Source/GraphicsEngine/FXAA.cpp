#include "FXAA.h"

FXAA::FXAA()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;
	this->gSwapChain = NULL;

	this->mPreviousPreset = 0;
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

void FXAA::PreRender(Shader* shader, unsigned int preset)
{
	if(!preset)
	{
		return;
	}
	HRESULT hr = S_OK;

	//get the surface/texture from the swap chain
	ID3D11Texture2D* backBufferTex = NULL;
	hr = this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTex);
	if(FAILED(hr))
	{
		MaloW::Debug("FXAA: Failed to get buffer from swapchain");
		return;
	}
	//get texture description
	D3D11_TEXTURE2D_DESC texDesc;
	backBufferTex->GetDesc(&texDesc);
	
	//change bindflag for use as a shader resource
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; 

	//create texture
	ID3D11Texture2D* sceneTex = NULL;
	hr = this->gDevice->CreateTexture2D(&texDesc, NULL, &sceneTex);	
	if(FAILED(hr))
	{
		MaloW::Debug("FXAA: Failed to create texture");
		return;
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
		return;
	}

	//set variables
	shader->SetResource("sceneTex", sceneSRV);

	if((int)preset != this->mPreviousPreset)
	{
		this->mPreviousPreset = preset;

		switch(preset)
		{
			case 1:
				shader->SetFloat("FXAA_EDGE_THRESHOLD", (1.0f / 4.0f));
				shader->SetFloat("FXAA_EDGE_THRESHOLD_MIN", (1.0f / 12.0f));
				shader->SetInt("FXAA_SEARCH_STEPS", 2);
				shader->SetInt("FXAA_SEARCH_ACCELERATION", 4);
				shader->SetFloat("FXAA_SEARCH_THRESHOLD", (1.0f / 4.0f));
				shader->SetInt("FXAA_SUBPIX", 1);
				shader->SetInt("FXAA_SUBPIX_FASTER", 1);
				shader->SetFloat("FXAA_SUBPIX_CAP", (2.0f / 3.0f));
				shader->SetFloat("FXAA_SUBPIX_TRIM", (1.0f / 4.0f));
				shader->SetFloat("FXAA_SUBPIX_TRIM_SCALE", (1.0f /(1.0f - (1.0f /4.0f))));
				break;
			case 2:
				shader->SetFloat("FXAA_EDGE_THRESHOLD", (1.0f / 8.0f));
				shader->SetFloat("FXAA_EDGE_THRESHOLD_MIN", (1.0f / 16.0f));
				shader->SetInt("FXAA_SEARCH_STEPS", 4);
				shader->SetInt("FXAA_SEARCH_ACCELERATION", 3);
				shader->SetFloat("FXAA_SEARCH_THRESHOLD", (1.0f / 4.0f));
				shader->SetInt("FXAA_SUBPIX", 1);
				shader->SetInt("FXAA_SUBPIX_FASTER", 0);
				shader->SetFloat("FXAA_SUBPIX_CAP", (3.0f / 4.0f));
				shader->SetFloat("FXAA_SUBPIX_TRIM", (1.0f / 4.0f));
				shader->SetFloat("FXAA_SUBPIX_TRIM_SCALE", (1.0f /(1.0f - (1.0f / 4.0f))));
				break;
			case 3:
				shader->SetFloat("FXAA_EDGE_THRESHOLD", (1.0f / 8.0f));
				shader->SetFloat("FXAA_EDGE_THRESHOLD_MIN", (1.0f / 24.0f));
				shader->SetInt("FXAA_SEARCH_STEPS", 8);
				shader->SetInt("FXAA_SEARCH_ACCELERATION", 2);
				shader->SetFloat("FXAA_SEARCH_THRESHOLD", (1.0f / 4.0f));
				shader->SetInt("FXAA_SUBPIX", 1);
				shader->SetInt("FXAA_SUBPIX_FASTER", 0);
				shader->SetFloat("FXAA_SUBPIX_CAP", (3.0f / 4.0f));
				shader->SetFloat("FXAA_SUBPIX_TRIM", (1.0f / 4.0f));
				shader->SetFloat("FXAA_SUBPIX_TRIM_SCALE", (1.0f / (1.0f - (1.0f / 4.0f))));
				break;
			case 4:
				shader->SetFloat("FXAA_EDGE_THRESHOLD", (1.0f / 8.0f));
				shader->SetFloat("FXAA_EDGE_THRESHOLD_MIN", (1.0f / 24.0f));
				shader->SetInt("FXAA_SEARCH_STEPS", 16);
				shader->SetInt("FXAA_SEARCH_ACCELERATION", 1);
				shader->SetFloat("FXAA_SEARCH_THRESHOLD", (1.0f / 4.0f));
				shader->SetInt("FXAA_SUBPIX", 1);
				shader->SetInt("FXAA_SUBPIX_FASTER", 0);
				shader->SetFloat("FXAA_SUBPIX_CAP", (3.0f / 4.0f));
				shader->SetFloat("FXAA_SUBPIX_TRIM", (1.0f / 4.0f));
				shader->SetFloat("FXAA_SUBPIX_TRIM_SCALE", (1.0f / (1.0f - (1.0f / 4.0f))));
				break;
			default: 
				MaloW::Debug("FXAA: Warning: Preset value can only be between 0 and 4.");
				break;
		}
	}

	//apply pass
	shader->Apply(0);

	//release
	SAFE_RELEASE(backBufferTex); 
	SAFE_RELEASE(sceneTex);
	SAFE_RELEASE(backBufferRTV);
	SAFE_RELEASE(backBufferRTVResource);
	SAFE_RELEASE(sceneSRV);
}
void FXAA::PostRender(Shader* shader)
{
	shader->SetResource("sceneTex", NULL);
	shader->Apply(0);
}
