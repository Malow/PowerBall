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
	this->tmp = device;

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

	D3DXMATRIX viewInverseTranspose, invProj, viewToTex;
	D3DXMatrixInverse(&viewInverseTranspose, NULL, &cam->GetViewMatrix());
	D3DXMatrixTranspose(&viewInverseTranspose, &viewInverseTranspose);
	shader->SetMatrix("viewInverseTranspose", viewInverseTranspose);
	shader->SetMatrix("projMatrix", cam->GetProjectionMatrix());
	D3DXMatrixInverse(&invProj, NULL, &cam->GetProjectionMatrix());
	shader->SetMatrix("invProjMatrix", invProj);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	static const D3DXMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	D3DXMatrixMultiply(&viewToTex, &cam->GetProjectionMatrix(), &T);
	shader->SetMatrix("viewToTex", viewToTex);


	D3DXVECTOR3 uniRndVec[14];
	uniRndVec[0] = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	uniRndVec[1] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	uniRndVec[2] = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	uniRndVec[3] = D3DXVECTOR3(+1.0f, -1.0f, -1.0f);

	uniRndVec[4] = D3DXVECTOR3(+1.0f, +1.0f, -1.0f);
	uniRndVec[5] = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);

	uniRndVec[6] = D3DXVECTOR3(-1.0f, +1.0f, -1.0f);
	uniRndVec[7] = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);

	uniRndVec[8] = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	uniRndVec[9] = D3DXVECTOR3(+1.0f, 0.0f, 0.0f);

	uniRndVec[10] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	uniRndVec[11] = D3DXVECTOR3(0.0f, +1.0f, 0.0f);

	uniRndVec[12] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	uniRndVec[13] = D3DXVECTOR3(0.0f, 0.0f, +1.0f);

	shader->SetFloatVectorArray("gOffsetVectors", uniRndVec, 14);







	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initData = {0};
	initData.SysMemPitch = 256 * sizeof(D3DXVECTOR3);

	D3DXVECTOR3 color[256*256];
	for(int i = 0; i < 256; ++i)
	{
		for(int j = 0; j < 256; ++j)
		{
			D3DXVECTOR3 v(RndFloat(), RndFloat(), RndFloat());

			color[i*256+j] = D3DXVECTOR3(v.x, v.y, v.z);
		}
	}

	initData.pSysMem = color;
	
	ID3D11Texture2D* tex = 0;
	this->tmp->CreateTexture2D(&texDesc, &initData, &tex);
	ID3D11ShaderResourceView* srv;
	this->tmp->CreateShaderResourceView(tex, 0, &srv);
	shader->SetResource("gRandomVecMap", srv);
	
	// view saves a reference.
	SAFE_RELEASE(tex);



}

void SSAO::PostRender(Shader* shader)
{
	shader->SetResource("rndTex", NULL);

	shader->SetResource("gRandomVecMap", NULL);
}