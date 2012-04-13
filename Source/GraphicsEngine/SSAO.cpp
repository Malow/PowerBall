#include "SSAO.h"

SSAO::SSAO()
{
	this->mNrOfSamples = 8;
	this->mRadius = 1.0f; 
	this->mAngleBias = 0.0f;
	this->mRndTex = NULL;
}
SSAO::SSAO(UINT nrOfSamples, float radius, float angleBias)
{
	this->mNrOfSamples = nrOfSamples;
	this->mRadius = radius;
	this->mAngleBias = angleBias;
	this->mRndTex = NULL;
}
SSAO::~SSAO()
{
	SAFE_RELEASE(this->mRndTex);
}

void SSAO::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	TextureManager texMgr = TextureManager();
	texMgr.Init(device, deviceContext);

	//calculate min and max values for x,y,z [0,radius]
	// radius = |(x,y,z)
	// = root(radius^2) 
	// = root((radius^2 / 3) + (radius^2 / 3) + (radius^2 / 3))
	// = root(root((radius^2 / 3)^2) + root((radius^2 / 3)^2) + root((radius^2 / 3)^2))
	// x,y,z = root(root((radius^2 / 3)^2)
	// = root((radius^2 / 3)

	float minMax = sqrt((this->mRadius * this->mRadius) / 3.0f);

	this->mRndTex = texMgr.CreateRndTex1DVec3(this->mNrOfSamples, -minMax, minMax);
}
					


void SSAO::PreRender(Shader* shader, GraphicsEngineParams engParams, Camera* cam)
{	
	shader->SetResource("rndTex", this->mRndTex);

	shader->SetInt("nrOfSamples", this->mNrOfSamples);
	shader->SetInt("width", engParams.windowWidth);
	shader->SetInt("height", engParams.windowHeight);
	shader->SetFloat("radius", this->mRadius);
	shader->SetFloat("angleBias", this->mAngleBias);
	D3DXMATRIX proj = cam->GetProjectionMatrix();
	shader->SetMatrix("projMatrix", proj);
	D3DXMATRIX invProj;
	D3DXMatrixInverse(&invProj, NULL, &proj);
	shader->SetMatrix("invProjMatrix", invProj);
}

void SSAO::PostRender(Shader* shader)
{
	shader->SetResource("rndTex", NULL);
}