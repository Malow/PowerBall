#include "SSAO.h"

SSAO::SSAO()
{
	this->mNrOfSamples = 8;
	this->mRadius = 1.0f; 
	this->mAngleBias = 0.0f;
	this->mRndTex = NULL;
	this->mShader = new Shader();
}
SSAO::SSAO(UINT nrOfSamples, float radius, float angleBias)
{
	this->mNrOfSamples = nrOfSamples;
	this->mRadius = radius;
	this->mAngleBias = angleBias;
	this->mRndTex = NULL;
	this->mShader = new Shader();
}
SSAO::~SSAO()
{
	SAFE_RELEASE(this->mRndTex);
	SAFE_DELETE(this->mShader);
}

HRESULT SSAO::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr = S_OK;

	this->gDevice = device;
	this->gDeviceContext = deviceContext;
	
	this->mRndTex = TextureManager::GetInstance()->CreateRndTex(this->mNrOfSamples, 3, -1.0f, 1.0f);

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
	this->mShader->SetInt("nrOfSamples", this->mNrOfSamples);
	this->mShader->SetInt("width", engParams.windowWidth);
	this->mShader->SetInt("height", engParams.windowHeight);
	this->mShader->SetFloat("radius", this->mRadius);
	this->mShader->SetFloat("angleBias", this->mAngleBias);
	this->mShader->SetMatrix("projMatrix", eng->GetCamera()->GetProjectionMatrix());
	D3DXMATRIX invProj;
	D3DXMatrixInverse(&invProj, NULL, &eng->GetCamera()->GetProjectionMatrix());
	this->mShader->SetMatrix("invProjMatrix", invProj);
}