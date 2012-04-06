#include "SSAO.h"

SSAO::SSAO()
{
	this->mShader = new Shader();
}
SSAO::~SSAO()
{
	SAFE_DELETE(this->mShader);
}

HRESULT SSAO::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr = S_OK;

	this->gDevice = device;
	this->gDeviceContext = deviceContext;

	return hr;
}

HRESULT SSAO::InitShader(char* filename, const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int numElements)
{
	HRESULT hr = S_OK;
	
	hr = this->mShader->Init(this->gDevice, this->gDeviceContext, filename, inputElementDesc, numElements);

	return hr;
}

void SSAO::Render()
{	
	//**
	GraphicsEngine* eng = GetGraphicsEngine();
	GraphicsEngineParams engParams = eng->GetEngineParameters();
	this->mShader->SetInt("width", engParams.windowWidth);
	this->mShader->SetInt("height", engParams.windowHeight);
	this->mShader->SetMatrix("viewProjMatrix", eng->GetCamera()->GetViewMatrix() * eng->GetCamera()->GetProjectionMatrix());
}