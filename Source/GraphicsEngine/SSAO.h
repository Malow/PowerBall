#pragma once

//!-------------------------------------------------------------------
//!	Class for using screen space ambient occlusion (SSAO)
//!-------------------------------------------------------------------

#include "stdafx.h"
#include "MaloWLib\Shader.h" //**
#include "GraphicsEngine.h"

class SSAO
{
	private:
		ID3D11Device*			gDevice;
		ID3D11DeviceContext*	gDeviceContext;

		Shader*	mShader; //**

	public:
		SSAO();
		virtual ~SSAO();
		HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		HRESULT InitShader(char* filename, const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int numElements); //**

		void Render();
};