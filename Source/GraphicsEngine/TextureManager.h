//!-----------------------------------------------------
//! Singleton class for creating and managing textures
//!-----------------------------------------------------
#pragma once

#include "stdafx.h"

class TextureManager
{
	private:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;

	private:
		ID3D11ShaderResourceView* CreateRndTex2(UINT size, float min, float max);
		ID3D11ShaderResourceView* CreateRndTex3(UINT size, float min, float max);
		ID3D11ShaderResourceView* CreateRndTex4(UINT size, float min, float max);

	public:
		TextureManager();
		~TextureManager();

		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		//! Creates a texture1D of given size with vectors of given dimension, containing values between [min, max]
		ID3D11ShaderResourceView* CreateRndTex(UINT size, UINT dimension, float min, float max);
};