#pragma once

//!-------------------------------------------------------------------
//!	Class for using screen space ambient occlusion (SSAO)
//!-------------------------------------------------------------------

#include "TextureManager.h"
#include "MaloWLib\Shader.h" //**
#include "GraphicsEngine.h"

class SSAO
{
	private:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;
		
		UINT						mNrOfSamples;	//! The number of samples to use. 8 is default.
		float						mRadius;		//! Length multiplier of random vectors. 1.0 is default.
		float						mAngleBias;		//! Angle in radian from plane of pixel to ignore samples. 0.0 is default.
		ID3D11ShaderResourceView*	mRndTex;		//! Texture to store randomized 3D-vectors x,y,z[-1,1] = length[0,3].
		Shader*						mShader; //**

	public:
		SSAO();
		SSAO(UINT nrOfSamples, float radius, float angleBias);
		virtual ~SSAO();
		HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		HRESULT InitShader(char* filename, const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int numElements); //**

		void Render();
};