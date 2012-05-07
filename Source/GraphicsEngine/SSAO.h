#pragma once

/*!
	Class for using screen space ambient occlusion (SSAO)
*/

#include "TextureManager.h"
#include "Shader.h"
#include "GraphicsEngineParameters.h"
#include "Camera.h"

class SSAO
{
	private:		
		UINT						mNrOfSamples;	/*! The number of samples to use. 8 is default. */
		float						mRadius;		/*! Radius of sphere (max length of sample vectors). */
		float						mAngleBias;		/*! Angle in radian from plane of pixel to ignore samples. 0.0 is default. */
		//ID3D11ShaderResourceView*	mRndTex;		/*! Texture to store randomized 3D-vectors x,y,z[-root((radius^2 / 3),root((radius^2 / 3)], length[0,radius]. */
		//ID3D11Device* tmp;
		D3DXVECTOR3*				mUniRndVectors;
		ID3D11ShaderResourceView*	mRndVectorsSRV;

	public:
		SSAO();
		SSAO(UINT nrOfSamples, float radius, float angleBias);
		virtual ~SSAO();
		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		/*! Sets all SSAO settings to the shader. */
		void PreRender(Shader* shader, GraphicsEngineParams engParams, Camera* cam);

		/*! Cleans up the random-texture. */
		void PostRender(Shader* shader);
};