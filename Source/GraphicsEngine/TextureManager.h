//!-----------------------------------------------------
//! Singleton class for creating and managing textures
//!-----------------------------------------------------
#pragma once

#include <ctime>
#include "stdafx.h"

class TextureManager
{
	private:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;

	public:
		TextureManager();
		~TextureManager();

		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		/*! Returns a texture1D of given size containing randomized 3D-vectors with values between [min, max] */
		ID3D11ShaderResourceView* CreateRndTex1DVec3(UINT size, float min, float max);
		/*! Returns a texture2D of given width and height containing randomized 3D-vectors with values between [min, max] */
		ID3D11ShaderResourceView* CreateRndTex2DVec3(UINT width, UINT height, float min, float max);
};