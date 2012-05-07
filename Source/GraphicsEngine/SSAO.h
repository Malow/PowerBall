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
		D3DXVECTOR3*				mUniRndVectors; /*! Array of 14 uniformally distributed vectors. */
		ID3D11ShaderResourceView*	mRndVectorsSRV; /*! Texture to store randomized 3D-vectors with xyz ranging from [-1,1]*/
		

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