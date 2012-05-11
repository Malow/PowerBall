#include "FXAA.h"

FXAA::FXAA()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;
	this->gSwapChain = NULL;

	this->mAppliedPresetToShader = false;
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

int FXAA::GetPreset() const
{
	return this->mPreset;
}

void FXAA::SetPreset(unsigned int preset)
{
	if(0 <= preset && preset <= 4)
	{
		this->mPreset = preset;
	}
	else
	{
		this->mPreset = 0;
	}
	this->mAppliedPresetToShader = false;
}

void FXAA::PreRender(Shader* shader)
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

	if(!this->mAppliedPresetToShader)
	{
		switch(this->mPreset)
		{
			case 1:
				shader->SetFloat("FXAA_EDGE_THRESHOLD", (1.0/4.0));
				shader->SetFloat("FXAA_EDGE_THRESHOLD_MIN", (1.0/12.0));
				shader->SetInt("FXAA_SEARCH_STEPS", 2);
				shader->SetInt("FXAA_SEARCH_ACCELERATION", 4);
				shader->SetFloat("FXAA_SEARCH_THRESHOLD", (1.0/4.0));
				shader->SetInt("FXAA_SUBPIX", 1);
				shader->SetInt("FXAA_SUBPIX_FASTER", 1);
				shader->SetFloat("FXAA_SUBPIX_CAP", (2.0/3.0));
				shader->SetFloat("FXAA_SUBPIX_TRIM", (1.0/4.0));
				shader->SetFloat("FXAA_SUBPIX_TRIM_SCALE", (1.0/(1.0 - (1.0/4.0))));
				break;
			case 2:
				shader->SetFloat("FXAA_EDGE_THRESHOLD", (1.0/8.0));
				shader->SetFloat("FXAA_EDGE_THRESHOLD_MIN", (1.0/16.0));
				shader->SetInt("FXAA_SEARCH_STEPS", 4);
				shader->SetInt("FXAA_SEARCH_ACCELERATION", 3);
				shader->SetFloat("FXAA_SEARCH_THRESHOLD", (1.0/4.0));
				shader->SetInt("FXAA_SUBPIX", 1);
				shader->SetInt("FXAA_SUBPIX_FASTER", 0);
				shader->SetFloat("FXAA_SUBPIX_CAP", (3.0/4.0));
				shader->SetFloat("FXAA_SUBPIX_TRIM", (1.0/4.0));
				shader->SetFloat("FXAA_SUBPIX_TRIM_SCALE", (1.0/(1.0 - (1.0/4.0))));
				break;
			case 3:
				shader->SetFloat("FXAA_EDGE_THRESHOLD", (1.0/8.0));
				shader->SetFloat("FXAA_EDGE_THRESHOLD_MIN", (1.0/24.0));
				shader->SetInt("FXAA_SEARCH_STEPS", 8);
				shader->SetInt("FXAA_SEARCH_ACCELERATION", 2);
				shader->SetFloat("FXAA_SEARCH_THRESHOLD", (1.0/4.0));
				shader->SetInt("FXAA_SUBPIX", 1);
				shader->SetInt("FXAA_SUBPIX_FASTER", 0);
				shader->SetFloat("FXAA_SUBPIX_CAP", (3.0/4.0));
				shader->SetFloat("FXAA_SUBPIX_TRIM", (1.0/4.0));
				shader->SetFloat("FXAA_SUBPIX_TRIM_SCALE", (1.0/(1.0 - (1.0/4.0))));
				break;
			case 4:
				shader->SetFloat("FXAA_EDGE_THRESHOLD", (1.0/8.0));
				shader->SetFloat("FXAA_EDGE_THRESHOLD_MIN", (1.0/24.0));
				shader->SetInt("FXAA_SEARCH_STEPS", 16);
				shader->SetInt("FXAA_SEARCH_ACCELERATION", 1);
				shader->SetFloat("FXAA_SEARCH_THRESHOLD", (1.0/4.0));
				shader->SetInt("FXAA_SUBPIX", 1);
				shader->SetInt("FXAA_SUBPIX_FASTER", 0);
				shader->SetFloat("FXAA_SUBPIX_CAP", (3.0/4.0));
				shader->SetFloat("FXAA_SUBPIX_TRIM", (1.0/4.0));
				shader->SetFloat("FXAA_SUBPIX_TRIM_SCALE", (1.0/(1.0 - (1.0/4.0))));
				break;
			default: break;
		}

		this->mAppliedPresetToShader = true;
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
