#ifndef LIGHT
#define LIGHT

#include "stdafx.h"
/*
struct Light
{
	D3DXVECTOR3 lookat;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 color;
	float intensity;
	D3DXMATRIX ViewProj;
};
*/
enum LightType
{
	POINT_LIGHT = 0,
};

//namespace MaloW
//{
	class Light
	{
	private:
		D3DXVECTOR3 pos;
		D3DXVECTOR3 lookAt;
		D3DXVECTOR3 up;

		LightType type;
		D3DXVECTOR3 color;
		float intensity;


		// Shadow Map
		bool UseShadowMap;

		int ShadowMapWidth;
		int ShadowMapHeight;
		D3DXMATRIX ViewProj;

		ID3D11Texture2D* shadowMap[5];
		ID3D11DepthStencilView* shadowMapDepthView[5];
		ID3D11ShaderResourceView* shadowMapSRView[5];
		D3D11_VIEWPORT ShadowMapViewPort[5];

	public:
		Light();
		Light(D3DXVECTOR3 pos, LightType type = POINT_LIGHT);
		virtual ~Light();

		HRESULT InitShadowMap(ID3D11Device* g_Device);

		D3DXMATRIX GetViewProjMatrix();
		void SetViewProjMatrix(D3DXMATRIX vp) { this->ViewProj = vp; }


		void SetColor(D3DXVECTOR3 col) { this->color = col; }
		D3DXVECTOR3 GetColor() const { return this->color; }
		void SetPosition(D3DXVECTOR3 pos) { this->pos = pos; }
		D3DXVECTOR3 GetPosition() const { return this->pos; }
		void Move(D3DXVECTOR3 moveBy) { this->pos += moveBy; }
		void SetLookAt(D3DXVECTOR3 la) { this->lookAt = la; }
		D3DXVECTOR3 GetLookAt() const { return this->lookAt; }
		void SetUp(D3DXVECTOR3 up) { this->up = up; }
		D3DXVECTOR3 GetUp() const { return this->up; }

		void SetIntensity(float intensity) { this->intensity = intensity; }
		float GetIntensity() const { return this->intensity; }


		bool IsUsingShadowMap() const { return this->UseShadowMap; }
		D3D11_VIEWPORT GetShadowMapViewPort(int pos) const { return this->ShadowMapViewPort[pos]; }
		ID3D11ShaderResourceView* GetShadowMapSRV(int pos) const { return this->shadowMapSRView[pos]; }
		ID3D11DepthStencilView* GetShadowMapDSV(int pos) const { return this->shadowMapDepthView[pos]; }
		ID3D11Texture2D* GetShadowMap(int pos) const { return this->shadowMap[pos]; }
	};
//};



#endif