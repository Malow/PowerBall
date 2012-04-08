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

		static TextureManager*				mInstance;
		UINT								mNrOfTextures;
		vector<string>						mTexNames;
		vector<ID3D11ShaderResourceView*>	mTextureSRVs;

	private:
		TextureManager();
		~TextureManager();
		
		ID3D11ShaderResourceView* CreateRndTex2(UINT size, float min, float max);
		ID3D11ShaderResourceView* CreateRndTex3(UINT size, float min, float max);
		ID3D11ShaderResourceView* CreateRndTex4(UINT size, float min, float max);

	public:
		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		//! returns the instance of the class
		static TextureManager* GetInstance();
		//! deletes and releases used variables
		void Cleanup();
		
		//! Creates a texture1D of given size with vectors of given dimension, containing values between [min, max]
		//! **Todo: implement support for dimension other than 4**
		ID3D11ShaderResourceView* CreateRndTex(UINT size, UINT dimension, float min, float max);
};