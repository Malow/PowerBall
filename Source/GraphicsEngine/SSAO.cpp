#include "SSAO.h"

SSAO::SSAO()
{
	this->mNrOfSamples = 8;
	this->mRadius = 1.0f; 
	this->mAngleBias = 0.0f;
	//this->mRndTex = NULL;
	this->mUniRndVectors = new D3DXVECTOR3[14];
	this->mUniRndVectors[0] = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	this->mUniRndVectors[1] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	this->mUniRndVectors[2] = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	this->mUniRndVectors[3] = D3DXVECTOR3(+1.0f, -1.0f, -1.0f);

	this->mUniRndVectors[4] = D3DXVECTOR3(+1.0f, +1.0f, -1.0f);
	this->mUniRndVectors[5] = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);

	this->mUniRndVectors[6] = D3DXVECTOR3(-1.0f, +1.0f, -1.0f);
	this->mUniRndVectors[7] = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);

	this->mUniRndVectors[8] = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	this->mUniRndVectors[9] = D3DXVECTOR3(+1.0f, 0.0f, 0.0f);

	this->mUniRndVectors[10] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	this->mUniRndVectors[11] = D3DXVECTOR3(0.0f, +1.0f, 0.0f);

	this->mUniRndVectors[12] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	this->mUniRndVectors[13] = D3DXVECTOR3(0.0f, 0.0f, +1.0f);

	this->mRndVectorsSRV = NULL;
}
SSAO::SSAO(UINT nrOfSamples, float radius, float angleBias)
{
	this->mNrOfSamples = nrOfSamples;
	this->mRadius = radius;
	this->mAngleBias = angleBias;
	//this->mRndTex = NULL;
	this->mUniRndVectors = new D3DXVECTOR3[14];
	this->mUniRndVectors[0] = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	this->mUniRndVectors[1] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	this->mUniRndVectors[2] = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	this->mUniRndVectors[3] = D3DXVECTOR3(+1.0f, -1.0f, -1.0f);

	this->mUniRndVectors[4] = D3DXVECTOR3(+1.0f, +1.0f, -1.0f);
	this->mUniRndVectors[5] = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);

	this->mUniRndVectors[6] = D3DXVECTOR3(-1.0f, +1.0f, -1.0f);
	this->mUniRndVectors[7] = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);

	this->mUniRndVectors[8] = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	this->mUniRndVectors[9] = D3DXVECTOR3(+1.0f, 0.0f, 0.0f);

	this->mUniRndVectors[10] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	this->mUniRndVectors[11] = D3DXVECTOR3(0.0f, +1.0f, 0.0f);

	this->mUniRndVectors[12] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	this->mUniRndVectors[13] = D3DXVECTOR3(0.0f, 0.0f, +1.0f);

	this->mRndVectorsSRV = NULL;
}
SSAO::~SSAO()
{
	//SAFE_RELEASE(this->mRndTex);
	SAFE_DELETE_ARRAY(this->mUniRndVectors);
}

void SSAO::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
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
	device->CreateTexture2D(&texDesc, &initData, &tex);
	device->CreateShaderResourceView(tex, 0, &this->mRndVectorsSRV);
	
	// view saves a reference.
	SAFE_RELEASE(tex);



	//**old**
	//TextureManager texMgr = TextureManager();
	//texMgr.Init(device, deviceContext);

	//calculate min and max values for x,y,z [0,radius]
	// radius = |(x,y,z)
	// = root(radius^2) 
	// = root((radius^2 / 3) + (radius^2 / 3) + (radius^2 / 3))
	// = root(root((radius^2 / 3)^2) + root((radius^2 / 3)^2) + root((radius^2 / 3)^2))
	// x,y,z = root(root((radius^2 / 3)^2)
	// = root((radius^2 / 3)

	//float minMax = sqrt((this->mRadius * this->mRadius) / 3.0f);

	//this->mRndTex = texMgr.CreateRndTex1DVec3(this->mNrOfSamples, -minMax, minMax);
}
					


void SSAO::PreRender(Shader* shader, GraphicsEngineParams engParams, Camera* cam)
{	
	shader->SetResource("rndVectorsTex", this->mRndVectorsSRV);
	this->mNrOfSamples = 32; //**tmp**
	shader->SetInt("nrOfSamples", this->mNrOfSamples);
	//shader->SetFloat("radius", this->mRadius); **
	shader->SetFloat("angleBias", this->mAngleBias);
	shader->SetMatrix("projMatrix", cam->GetProjectionMatrix());
	shader->SetMatrix("viewMatrix", cam->GetViewMatrix());
	shader->SetFloatVectorArray("uniRndVectors", this->mUniRndVectors, 14);
}

void SSAO::PostRender(Shader* shader)
{
	shader->SetResource("gRandomVecMap", NULL);
}