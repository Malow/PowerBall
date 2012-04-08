#pragma once

//!-------------------------------------------------------------------
//!	Class for using screen space ambient occlusion (SSAO)
//!-------------------------------------------------------------------

#include "TextureManager.h"
#include "Shader.h"
#include "GraphicsEngineParameters.h"
#include "Camera.h"

class SSAO
{
	private:		
		UINT						mNrOfSamples;	//! The number of samples to use. 8 is default.
		float						mRadius;		//! Length multiplier of random vectors. 1.0 is default.
		float						mAngleBias;		//! Angle in radian from plane of pixel to ignore samples. 0.0 is default.
		ID3D11ShaderResourceView*	mRndTex;		//! Texture to store randomized 3D-vectors x,y,z[-1,1] = length[0,3].

	public:
		SSAO();
		SSAO(UINT nrOfSamples, float radius, float angleBias);
		virtual ~SSAO();
		HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		/*! Sets all SSAO settings to the shader. */
		void PreRender(Shader* shader, GraphicsEngineParams engParams, Camera* cam);

		/*! Cleans up the random-texture. */
		void PostRender(Shader* shader);
};